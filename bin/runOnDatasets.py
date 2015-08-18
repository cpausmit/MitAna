#!/usr/bin/env python

import sys
import os
import re
import time
import subprocess
import glob
import shutil
import socket

# experimental full local caching
FULL_LOCAL = False

#######################
## UTILITY FUNCTIONS ##
#######################

def yes(message, options = []):
    """
    Print the message and return true if the response is y.
    """

    print message

    if len(options) == 0:
        prompt = ' [y/N]:'
        print prompt
        optvals = {'y': True, 'N': False}
    else:
        optvals = {}
        prompt = []
        for opt, val in options:
            print (' (%s)' % opt[0]) + opt[1:]
            optvals[opt[0]] = val
            prompt.append(opt[0])

        prompt = ' [%s]:' % ('/'.join(prompt))

    while True:
        response = sys.stdin.readline().strip()
        try:
            return optvals[response]
        except KeyError:
            print prompt


def parseValueStr(value):
    strmatch = re.match('["\'](.*)["\']$', value)
    if strmatch:
        value = strmatch.group(1)
    else:
        try:
            value = int(value)
        except ValueError:
            try:
                value = float(value)
            except ValueError:
                pass

    return value


def makeValueStr(value):
    if type(value) is str:
        value = "'" + value + "'"

    return str(value)


def runSubproc(*args, **kwargs):
    """
    Run external process using subprocess.Popen
    """

    proc = subprocess.Popen(list(args), stdout = subprocess.PIPE, stderr = subprocess.PIPE, stdin = subprocess.PIPE)

    try:
        stdin = kwargs['stdin']
    except KeyError:
        stdin = None

    try:
        stdout = kwargs['stdout']
    except KeyError:
        stdout = sys.stdout

    try:
        stderr = kwargs['stderr']
    except KeyError:
        stderr = sys.stderr
            
    out, err = proc.communicate(stdin)

    if stdout is not None:
        if type(stdout) is list:
            if out.endswith('\n'):
                out = out[:-1]
            for line in out.split('\n'):
                stdout.append(line)

        elif out.strip():
            stdout.write(out)
            stdout.flush()

    if stderr is not None:
        if type(stderr) is list:
            if err.endswith('\n'):
                err = err[:-1]
            for line in err.split('\n'):
                stderr.append(line)

        elif err.strip():
            stderr.write(err)
            stderr.flush()

#############
## CLASSES ##
#############

class DatasetInfo(object):
    """
    A class that represents one line of the dataset list.
    """

    def __init__(self, book = '', dataset = ''):
        self.book = book
        self.dataset = dataset
        self.json = ''
        self.skim = ''
        self.custom = {}

        self.outDir = ''
        self.logDir = ''
        self.confDir = ''
        self.jobs = {}

    def isMC(self):
        """
        Return True if json is either a null string or ~.
        """
        return self.json == '' or self.json == '~'

    def jsonName(self):
        if self.json == '':
            return '~'
        else:
            return self.json

    def hasJsonFile(self):
        return self.jsonName() != '~' and self.jsonName() != '-'

    def skimName(self):
        """
        Return the name of the skim. If not defined, return -.
        """
        if self.skim == '' or self.skim == 'noskim':
            return '-'
        else:
            return self.skim

    def customKeys(self):
        return sorted(self.custom.keys())

    def key(self):
        return (self.book, self.dataset)

    def path(self):
        return self.book + '/' + self.dataset

    def setDirNames(self, env, condorConf = None):
        # job output (histograms) destination
        self.outDir = env.outDir + '/' + self.path()
        # .log, .out, and .err files
        self.logDir = env.logDir + '/' + self.path()
        # catalog and analysis macro
        self.confDir = env.workspace + '/' + self.path()

        if not os.path.exists(self.outDir):
            os.makedirs(self.outDir)
        if not os.path.exists(self.logDir):
            os.makedirs(self.logDir)
        if not os.path.exists(self.confDir):
            os.makedirs(self.confDir)

        if condorConf:
            outDirs = condorConf.outputDirList(self)
            for outDir in outDirs:
                if not os.path.exists(outDir):
                    os.makedirs(outDir)
        
    def __eq__(self, rhs):
        if self.book != rhs.book or self.dataset != rhs.dataset:
            return False
        if self.isMC() != rhs.isMC():
            return False
        if self.skimName() != rhs.skimName():
            return False
        for key, value in self.custom.items():
            if key not in rhs.custom:
                return False
            if value != rhs.custom[key]:
                return False

        return True

    def __ne__(self, rhs):
        return not self.__eq__(rhs)


class JobInfo(object):
    """
    A class that represents one job = one fileset.
    """

    #status
    SubmitReady = -1
    OutputExists = -2
    Skip = -3
    Unexpanded = 0
    Idle = 1
    Running = 2
    Removed = 3
    Completed = 4
    Held = 5
    SubmissionErr = 6

    def __init__(self, dataset, fileset, nentries = -1):
        self.dataset = dataset
        self.fileset = fileset
        self.nentries = nentries

        self.submitHost = ''
        self.jobId = ''
        self.status = JobInfo.SubmitReady

    def statusStr(self):
        if self.status == 0:
            statusStr = 'Unexpanded (U)'
        elif self.status == 1:
            statusStr = 'Idle (I)'
        elif self.status == 2:
            statusStr = 'Running (R)'
        elif self.status == 3:
            statusStr = 'Removed (X)'
        elif self.status == 4:
            statusStr = 'Completed (C)'
        elif self.status == 5:
            statusStr = 'Held (H)'
        elif self.status == 6:
            statusStr = 'Submission_err (E)'
        else:
            statusStr = 'Unknown'

        return statusStr


class CondorConfig(object):
    """
    Condor job description with preset parameter values.
    """

    def __init__(self, taskName):
        self.jdl = {
            'arguments': '"{book} {dataset} {fileset} {nentries}"',
            'initialdir': env.outDir + '/{book}/{dataset}',
            'output': env.logDir + '/{book}/{dataset}/{fileset}.out',
            'error': env.logDir + '/{book}/{dataset}/{fileset}.err',
            'log': env.logDir + '/{book}/{dataset}/{fileset}.log',     
            'universe': 'vanilla',
            'getenv': 'False',
            'input': '/dev/null',
            'should_transfer_files': 'YES',
            'when_to_transfer_output': 'ON_EXIT'
        }
        self.taskName = taskName

    def readFromFile(self, path, env):
        with open(path) as confFile:
            for line in confFile:
                if not line.strip() or line.strip().startswith('#'):
                    continue
    
                key, eq, value = line.partition('=')
                if key in self.jdl:
                    print ' Ignoring key ' + key + ' found in condor template.'
                    continue
    
                self.jdl[key.strip().lower()] = value.strip()

        if 'executable' not in self.jdl:
            self.jdl['executable'] = env.workspace + '/start.sh'

        if '{fileset}.root' in self.outputRemaps():
            print ' Remap of {fileset}.root is not supported.'
            sys.exit(1)
    
        try:
            inputList = map(str.strip, self.jdl['transfer_input_files'].split(','))
            listWasGiven = True
        except KeyError:
            inputList = []
            listWasGiven = False
    
        for inputFile in ['{book}/{dataset}/run.py', env.cmsswPack, 'env.sh', 'x509up']:
            fullPath = env.workspace + '/' + inputFile
            if inputFile == 'x509up' and not os.path.exists(fullPath):
                continue
            if fullPath not in inputList:
                if listWasGiven:
                    print ' Adding', fullPath, 'to transfer_input_files.'
    
                inputList.append(fullPath)
    
        self.jdl['transfer_input_files'] = ','.join(inputList)
    
    def writeToFile(self, path):
        with open(path, 'w') as jdlFile:
            for key, value in self.jdl.items():
                jdlFile.write(key + ' = ' + value + '\n')

    def outputRemaps(self):
        if 'transfer_output_remaps' not in self.jdl:
            return {}

        remapDict = {}
        remapDefs = map(str.strip, self.jdl['transfer_output_remaps'].strip('"').split(';'))
        for remapDef in remapDefs:
            source, eq, target = map(str.strip, remapDef.partition('='))
            remapDict[source] = target

        return remapDict

    def outputList(self, jobInfo):
        outPaths = map(str.strip, self.jdl['transfer_output_files'].split(','))
        remaps = self.outputRemaps()

        for iP in range(len(outPaths)):
            outPath = outPaths[iP]
            if outPath in remaps:
                outPaths[iP] = remaps[outPath]
                outPath = outPaths[iP]

            if not outPath.startswith('/'):
                outPaths[iP] = self.jdl['initialdir'] + '/' + outPath
                outPath = outPaths[iP]

            outPaths[iP] = self.format(outPath, jobInfo)

        return outPaths

    def outputDirList(self, datasetInfo):
        outDirs = []

        outPaths = map(str.strip, self.jdl['transfer_output_files'].split(','))
        remaps = self.outputRemaps()

        for outPath in outPaths:
            if outPath in remaps:
                outPath = remaps[outPath]

            if outPath.startswith('/'):
                outDir = os.path.dirname(outPath)
            else:
                outDir = self.jdl['initialdir']

            outDir = self.format(outDir, datasetInfo)
            if outDir not in outDirs:
                outDirs.append(outDir)

        return outDirs

    def format(self, s, info):
        if type(info) is JobInfo:
            return s.format(task = self.taskName, book = info.dataset.book, dataset = info.dataset.dataset, fileset = info.fileset, nentries = info.nentries)
        elif type(info) is DatasetInfo:
            return s.format(task = self.taskName, book = info.book, dataset = info.dataset)
        else:
            return s

    def jdlCommand(self, jobInfo):
        return '\n'.join([key + ' = ' + self.format(value, jobInfo) for key, value in self.jdl.items()]) + '\nqueue\n'


######################################################################
## MAIN EXECUTABLE FUNCTIONS (PROBABLY BETTER IN A CLASS IN FUTURE) ##
######################################################################

def setupWorkspace(env):
    """
    Set up the directory structure and create tarballs.
    """

    if not env.update and not env.inMacroPath:
        print ' No analysis configuration specified. Set by --analysis=config_path'
        return False

    if env.update:
        if not os.path.isdir(env.workspace) or not os.path.isdir(env.outDir) or not os.path.isdir(env.logDir):
            print 'Directory structures inconsistent. Cannot update task.'
            return False
    else:
        try:
            os.makedirs(env.workspace)
            os.makedirs(env.outDir)
            os.makedirs(env.logDir)

        except OSError:
            try:
                shutil.rmtree(env.workspace)
                shutil.rmtree(env.outDir)
                shutil.rmtree(env.logDir)
            except:
                pass

            print 'Failed to create workspace.'
            return False

        # write out environment variables
        with open(env.workspace + '/env.sh', 'w') as envFile:
            envFile.write('export SCRAM_ARCH="' + env.scramArch + '"\n')
            envFile.write('export CMSSW_RELEASE="' + env.release + '"\n')
            envFile.write('export CMSSW_NAME="' + env.cmsswname + '"\n')
            envFile.write('export MIT_DATA="' + env.mitdata + '"\n')


    if env.inMacroPath: # including update case
        shutil.copyfile(env.inMacroPath, env.workspace + '/macro.py')

    # copy the execution script
    if not env.update:
        shutil.copyfile(env.cmsswbase + '/src/MitAna/bin/start.sh', env.workspace + '/start.sh')

    # (create and) copy the library tarball
    if os.path.exists(env.cmsswdir + '/' + env.libPack):
        remakeLibPack = False
        packLastUpdate = os.path.getmtime(env.cmsswdir + '/' + env.libPack)
        for lib in glob.glob(env.cmsswbase + '/lib/' + env.scramArch + '/*'):
            if os.path.getmtime(lib) > packLastUpdate:
                remakeLibPack = True
                break
    else:
        remakeLibPack = True
    
    if remakeLibPack:
        print ' Creating library tarball.'
        runSubproc('tar', 'czf', env.cmsswdir + '/' + env.libPack, '-C', env.cmsswbase, 'lib')

    copyLibPack = False
    if not os.path.exists(env.workspace + '/' + env.libPack) or \
            os.path.getmtime(env.cmsswdir + '/' + env.libPack) > os.path.getmtime(env.workspace + '/' + env.libPack):
        copyLibPack = True
        shutil.copyfile(env.cmsswdir + '/' + env.libPack, env.workspace + '/' + env.libPack)

    # (create and) copy the headers tarball
    # header files needed until ROOT 6 libraries become position independent
    if os.path.exists(env.cmsswdir + '/' + env.hdrPack):
        remakeHdrPack = False
        packLastUpdate = os.path.getmtime(env.cmsswdir + '/' + env.hdrPack)
    else:
        packLastUpdate = 0
    
    headerPaths = []
    for package in os.listdir(env.cmsswbase + '/src'):
        if not os.path.isdir(env.cmsswbase + '/src/' + package):
            continue

        for module in os.listdir(env.cmsswbase + '/src/' + package):
            if not os.path.isdir(env.cmsswbase + '/src/' + package + '/' + module + '/interface'):
                continue

            for header in glob.glob(env.cmsswbase + '/src/' + package + '/' + module + '/interface/*'):
                if os.path.getmtime(header) > packLastUpdate:
                    remakeHdrPack = True
                    break

            headerPaths.append('src/' + package + '/' + module + '/interface')
    
    if remakeHdrPack:
        print ' Creating headers tarball.'
        runSubproc('tar', 'czf', env.cmsswdir + '/' + env.hdrPack, '-C', env.cmsswbase, *tuple(headerPaths))

    copyHdrPack = False
    if not os.path.exists(env.workspace + '/' + env.hdrPack) or \
            os.path.getmtime(env.cmsswdir + '/' + env.hdrPack) > os.path.getmtime(env.workspace + '/' + env.hdrPack):
        copyHdrPack = True
        shutil.copyfile(env.cmsswdir + '/' + env.hdrPack, env.workspace + '/' + env.hdrPack)

    # (create and) copy the MitAna/bin tarball
    if os.path.exists(env.cmsswdir + '/' + env.binPack):
        remakeBinPack = False
        packLastUpdate = os.path.getmtime(env.cmsswdir + '/' + env.binPack)
        for fileName in glob.glob(env.cmsswbase + '/src/MitAna/bin/*'):
            if os.path.getmtime(fileName) > packLastUpdate:
                remakeBinPack = True
                break
    else:
        remakeBinPack = True

    if remakeBinPack:
        print ' Creating MitAna/bin tarball.'
        runSubproc('tar', 'czf', env.cmsswdir + '/' + env.binPack, '-C', env.cmsswbase, 'src/MitAna/bin')

    copyBinPack = False
    if not os.path.exists(env.workspace + '/' + env.binPack) or \
            os.path.getmtime(env.cmsswdir + '/' + env.binPack) > os.path.getmtime(env.workspace + '/' + env.binPack):
        copyBinPack = True
        shutil.copyfile(env.cmsswdir + '/' + env.binPack, env.workspace + '/' + env.binPack)

    if copyLibPack or copyHdrPack or copyBinPack:
        print ' Packing tarballs up.'
    
        # decompress the packages, concatenate them into one package, and compress again
        cmsswTar = env.cmsswPack[:env.cmsswPack.rfind('.')]
        hdrTar = env.hdrPack[:env.hdrPack.rfind('.')]
        binTar = env.binPack[:env.binPack.rfind('.')]
        shutil.copyfile(env.workspace + '/' + env.libPack, env.workspace + '/' + env.cmsswPack)
        runSubproc('gunzip', env.workspace + '/' + env.cmsswPack, env.workspace + '/' + env.hdrPack, env.workspace + '/' + env.binPack)
        runSubproc('tar', 'Af', env.workspace + '/' + cmsswTar, env.workspace + '/' + hdrTar)
        runSubproc('tar', 'Af', env.workspace + '/' + cmsswTar, env.workspace + '/' + binTar)
        runSubproc('gzip', env.workspace + '/' + cmsswTar, env.workspace + '/' + hdrTar, env.workspace + '/' + binTar)

    if not env.update:
        # create an empty dataset list file
        open(env.workspace + '/datasets.list', 'w').close()

    return True


def readDatasetList(fileName):
    """
    Read datasets from the dataset configuration file.
    """

    datasets = []

    with open(fileName) as configFile:
        firstLine = True
        customKeys = []
        for line in configFile:
            if not re.search('[a-zA-Z0-9]', line):
                # skip non-alphanumeric lines
                continue
    
            if firstLine:
                # if column names are defined in the first line, pick them up
                firstLine = False
                columns = map(str.strip, line.lstrip('#').split())
                if columns[0] == 'book' and columns[1] == 'dataset':
                    if columns[2] != 'skim' or columns[-1] != 'json':
                        print ' Column names are given in', fileName, 'but they do not conform to the template.'
                        print ' The columns must be named "book dataset skim ... json".'
                        sys.exit(1)
    
                    customKeys = columns[3:-1]
                    continue
    
            matches = re.match('([^#][^ ]*) +([^ ]+) +([^ ]+)(| .*)$', line.strip())
            if not matches:
                continue
    
            datasetInfo = DatasetInfo(book = matches.group(1).strip(), dataset = matches.group(2).strip())
            datasetInfo.skim = matches.group(3).strip()

            # fourth paren captures the last column, which is currently assigned to JSON
            if matches.group(4):
                words = matches.group(4).split()
                if len(customKeys) != 0:
                    for iK, key in enumerate(customKeys):
                        try:
                            datasetInfo.custom[key] = parseValueStr(words[iK])
                        except IndexError:
                            print ' Variable', key, 'not defined for dataset', dataset
                            sys.exit(1)
    
                datasetInfo.json = words[-1]
            else:
                datasetInfo.json = ''
    
            datasets.append(datasetInfo)

    return datasets


def writeDatasetList(fileName, datasets):
    """
    Write the list of datasets into the dataset configuration file.
    """

    fileContent = readDatasetList(fileName)

    append = []
    remove = []

    for datasetInfo in datasets:
        for exDatasetInfo in fileContent:
            if datasetInfo.key() == exDatasetInfo.key():
                if datasetInfo != exDatasetInfo:
                    message = ' Specified skim/JSON configuration for ' + datasetInfo.path() + ' does not match the existing.\n'
                    message += ' Update configuration?\n'
                    message += ' ' + exDatasetInfo.skim + ' -> ' + datasetInfo.skim + '\n'
                    message += ' ' + exDatasetInfo.json + ' -> ' + datasetInfo.json
                    if len(exDatasetInfo.custom) != 0 or len(datasetInfo.custom) != 0:
                        message += '\n ' + str(exDatasetInfo.custom) + ' -> ' + str(datasetInfo.custom)
                    if yes(message):
                        remove.append(exDatasetInfo)
                        append.append(datasetInfo)
                
                break

        else: # if no matching exDatasetInfo was found
            append.append(datasetInfo)

    if len(append) != 0 or len(remove) != 0:
        for d in remove:
            fileContent.remove(d)
        for d in append:
            fileContent.append(d)

        with open(fileName, 'w') as configFile:
            firstLine = True
            for datasetInfo in fileContent:
                if firstLine:
                    configFile.write('book dataset skim ')
                    for key in datasetInfo.customKeys():
                        configFile.write(key + ' ')
                    configFile.write('json\n')
                    firstLine = False

                configFile.write(datasetInfo.book + ' ' + datasetInfo.dataset + ' ' + datasetInfo.skimName() + ' ')
                for key in datasetInfo.customKeys():
                    configFile.write(makeValueStr(datasetInfo.custom[key]) + ' ')
                configFile.write(datasetInfo.jsonName() + '\n')

        return True
    else:
        # no actual update took place
        return False


def writeCatalogs(catalogDir, datasets, filesPerFileset = 0):
    """
    Open the catalog and read out the list of filesets. Write out the catalog to
    the workspace (fileset size may be different depending on filesPerFileset).
    """

    invalid = []

    for datasetInfo in datasets:
        if not os.path.exists(datasetInfo.confDir + '/Files'):
            # catalog is not copied / created yet. Copy or write
            catalogSrc = catalogDir + '/' + datasetInfo.path()
            if datasetInfo.skimName() != '-':
                catalogSrc += '/' + datasetInfo.skimName()

            set0size = 0
            try:
                with open(catalogSrc + '/Files') as fileList:
                    for line in fileList:
                        if line.split()[0] == '0000':
                            set0size += 1
            except IOError:
                print ' Could not open catalog ' + catalogSrc + '. Skipping dataset.'
                invalid.append(datasetInfo)
                continue
    
            if filesPerFileset == 0 or filesPerFileset >= set0size:
                # using default fileset size - copy
                for fileName in os.listdir(catalogSrc):
                    if os.path.isfile(catalogSrc + '/' + fileName):
                        shutil.copyfile(catalogSrc + '/' + fileName, datasetInfo.confDir + '/' + fileName)
            else:
                # fileset size smaller than default - write own catalog
                with open(catalogSrc + '/Files') as src:
                    with open(datasetInfo.confDir + '/Files', 'w') as dst:
                        iLine = 0
                        for line in src:
                            dst.write('%04d ' % (iLine / filesPerFileset))
                            dst.write(' '.join(line.split()[1:]) + '\n')
                            iLine += 1

                nFilesets = iLine / filesPerFileset
                if iLine % filesPerFileset != 0:
                    nFilesets += 1
                
                with open(catalogSrc + '/Filesets') as src:
                    location = src.readline().split()[1]

                with open(datasetInfo.confDir + '/Filesets', 'w') as dst:
                    # write dummy data
                    for iSet in range(nFilesets):
                        dst.write('%04d %s 0 0 0 0 0 0\n' % (iSet, location))

    # clean up datasets that were invalid
    for entry in invalid:
        datasets.remove(entry)


def writeMacros(workspace, datasets, noOverwrite = True):
    """
    Parse the input macro and write a standalone python script for each dataset.
    """

    from MitAna.TreeMod.bambu import mithep, analysis
    from MitAna.PhysicsMod.runlumisel import goodLumiFilter

    # read from the orignal catalog created in workspace
    catalog = mithep.Catalog(workspace)

    invalid = []

    for datasetInfo in datasets:
        fileName = workspace + '/' + datasetInfo.path() + '/run.py'

        if os.path.exists(fileName) and noOverwrite:
            continue

        # get the dataset file list
        if FULL_LOCAL:
            ds = catalog.FindDataset(datasetInfo.book, datasetInfo.dataset, '', 3)
        else:
            ds = catalog.FindDataset(datasetInfo.book, datasetInfo.dataset, '', 1)

        pilotFile = str(ds.FileUrl(0))

        if pilotFile == '':
            print ' Dataset ' + datasetInfo.path() + ' appears to be empty.'
            invalid.append(datasetInfo)
            continue

        # pilot job never needs local caching
        pilotFile = pilotFile.replace('./store', '/mnt/hadoop/cms/store')

        print ' Writing analysis macro for ' + datasetInfo.path()

        # reset and build the analysis
        analysis.reset()

        analysis.isRealData = not datasetInfo.isMC()

        analysis.book = datasetInfo.book
        analysis.dataset = datasetInfo.dataset

        analysis.SetKeepHierarchy(False)

        for key, value in datasetInfo.custom.items():
            analysis.custom[key] = value

        execfile(workspace + '/macro.py')

        if datasetInfo.hasJsonFile():
            analysis._sequence = goodLumiFilter(datasetInfo.jsonName()) * analysis._sequence

        analysis.buildSequence()

        # would be much nicer if mithep.Dataset had an interface that exposes fileset names
        filesPerFileset = 0
        with open(workspace + '/' + datasetInfo.path() + '/Files') as fileList:
            for line in fileList:
                if line.split()[0] == '0000':
                    filesPerFileset += 1

        with open(fileName, 'w') as macro:
            macro.write('import sys\n')
            macro.write('import ROOT\n\n')
            macro.write('ROOT.gROOT.SetBatch(True)\n')
            macro.write('fileset = sys.argv[1]\n')
            macro.write('nentries = int(sys.argv[2])\n')

            for lib in mithep.loadedLibs:
                macro.write('ROOT.gSystem.Load(\'' + lib + '\')\n')

            macro.write('\nfiles = {\n')
            # first file for pilot job submission
            macro.write('    \'pilot\': [\'' + pilotFile + '\'],\n')

            iFile = 0
            for iFileset in range(ds.NFiles() / filesPerFileset + 1):
                macro.write('    \'%04d\': [\n' % iFileset)
                while iFile != (iFileset + 1) * filesPerFileset:
                    url = ds.FileUrl(iFile)
                    if url != '':
                        macro.write('        \'%s\',\n' % url)

                    iFile += 1

                macro.write('    ],\n')

            macro.write('}\n\n')

            macro.write('mithep = ROOT.mithep\n')
            macro.write('analysis = mithep.Analysis()\n\n')
            if FULL_LOCAL:
                macro.write('if fileset == \'pilot\':\n')
                macro.write('    analysis.SetUseCacher(1)\n')
                macro.write('else:\n')
                macro.write('    analysis.SetUseCacher(2)\n\n')
            else:
                macro.write('analysis.SetUseCacher(1)\n\n')
            macro.write('for f in files[fileset]:\n')
            macro.write('    analysis.AddFile(f)\n\n')
            macro.write('analysis.SetOutputName(fileset + \'.root\')\n\n')
            macro.write('if nentries > 0:\n')
            macro.write('    analysis.SetProcessNEvents(nentries)\n\n')
            macro.write(analysis.dumpPython(varName = 'analysis', withCtor = False, objects = {}))
            macro.write('\nanalysis.Run(False)\n')

    # remove invalid entries from datasets
    for entry in invalid:
        datasets.remove(entry)


def makeJobs(datasets, condorConf, limitTo = [], killStatus = []):
    """
    Create a list of jobs (= filesets) from the catalog and set their status
    """

    def setStatus(jobInfo):
        outPath = jobInfo.dataset.outDir + '/' + jobInfo.fileset + '.root'
        if os.path.exists(outPath) and os.stat(outPath).st_size != 0:
            jobInfo.status = JobInfo.OutputExists
        elif len(limitTo) == 0 or jobInfo.fileset in limitTo:
            jobInfo.status = JobInfo.SubmitReady
        else:
            jobInfo.status = JobInfo.Skip


    removePilotOutput = False

    # read catalog and add jobs to datasetInfo
    for datasetInfo in datasets:
        with open(datasetInfo.confDir + '/Filesets') as filesetList:
            for line in filesetList:
                filesetId = line.strip().split()[0]
                jobInfo = JobInfo(datasetInfo, filesetId)
                setStatus(jobInfo)
                
                datasetInfo.jobs[filesetId] = jobInfo

        pilot = JobInfo(datasetInfo, 'pilot', nentries = 0)
        setStatus(pilot)
        datasetInfo.jobs['pilot'] = pilot

        if pilot.status == JobInfo.OutputExists:
            if not removePilotOutput:
                message = 'Remove pilot output?'
                if yes(message):
                    removePilotOutput = True

        if removePilotOutput:
            pilotOutput = condorConf.outputList(pilot)
            for path in pilotOutput:
                try:
                    os.remove(path)
                except OSError:
                    print ' Failed to remove', path
                    pass

    # query condor to get real job status
    out = []
    err = []
    runSubproc('condor_q', '-global', '-long', '-attributes', 'Owner,ClusterId,ProcId,Iwd,Args,Arguments,JobStatus,GlobalJobId', stdout = out, stderr = err)

    if len(out) == 1 and out[0].strip() == 'All queues are empty':
        out = []

    for line in err:
        print line

    block = {}
    for line in out:
        if line.strip() != '':
            cfg = line.partition('=')
            block[cfg[0].strip()] = cfg[2].strip().strip('"')

        else:
            # one job block ended
            try:
                if block['Owner'] != os.environ['USER']:
                    raise Exception
    
                try:
                    book, dataset, fileset = block['Arguments'].split()[:3]
                except:
                    book, dataset, fileset = block['Args'].split()[:3]
    
                datasetInfo = next(info for info in datasets if info.key() == (book, dataset))
                    
                if block['Iwd'] != datasetInfo.outDir:
                    raise Exception
    
                jobInfo = datasetInfo.jobs[fileset]
    
                submitHost = block['GlobalJobId']
                jobInfo.submitHost = submitHost[:submitHost.find('#')]
                jobInfo.jobId = block['ClusterId'] + '.' + block['ProcId']
    
                jobInfo.status = int(block['JobStatus'])

                if jobInfo.status in killStatus:
                    killJob(jobInfo)

            except:
                pass

            block = {}


def killJob(jobInfo):
    datasetInfo = jobInfo.dataset
    if jobInfo.submitHost == socket.gethostname():
        print 'Killing job', jobInfo.jobId + ':', datasetInfo.book, datasetInfo.dataset, jobInfo.fileset
        runSubproc('condor_rm', jobInfo.jobId)
    else:
        print 'Killing job on', jobInfo.submitHost, jobInfo.jobId + ':', datasetInfo.book, datasetInfo.dataset, jobInfo.fileset
        runSubproc('ssh', jobInfo.submitHost, 'condor_rm', jobInfo.jobId)

    jobInfo.status = JobInfo.SubmitReady
    jobInfo.submitHost = ''
    jobInfo.jobId = ''


def submitJobs(env, datasets, condorConf):
    for datasetInfo in datasets:
        # loop over filesets and do actual submission
        for fileset in sorted(datasetInfo.jobs.keys()):
            jobInfo = datasetInfo.jobs[fileset]

            if fileset == 'pilot' and jobInfo.nentries == 0:
                continue

            # skip fileset if a job is in condor queue
            if jobInfo.jobId != '':
                print ' ' + jobInfo.statusStr() + ':', datasetInfo.dataset, fileset, '(' + jobInfo.submitHost + ':' + jobInfo.jobId + ')'
                continue

            # skip fileset if output exists
            if jobInfo.status == JobInfo.OutputExists:
                print ' Output exists:', datasetInfo.dataset, fileset
                continue

            if jobInfo.status == JobInfo.Skip:
                print ' Skipping (fileset not requested):', datasetInfo.dataset, fileset
                continue

            if env.noSubmit:
                print ' Skipping (no-submit = true):', datasetInfo.dataset, fileset
                continue

            print ' Submitting:', datasetInfo.dataset, fileset

            if env.submitFrom != '':
                runSubproc('ssh', env.submitFrom, 'condor_submit', stdin = condorConf.jdlCommand(jobInfo))
            else:
                runSubproc('condor_submit', stdin = condorConf.jdlCommand(jobInfo))


if __name__ == '__main__':
    from argparse import ArgumentParser, RawTextHelpFormatter

    if 'CMSSW_BASE' not in os.environ:
        print ' CMSSW_BASE not set.'
        sys.exit(1)
    
    description ='''Submit a BAMBU analysis to cluster.
    When a workspace name is specified and the workspace already exists in thee standard location, the workspace is recreated (--recreate) or checked for failed jobs. The dataset(s) to be processed can be passed from the command line (--book, --dataset, and --filesets (optional)) or listed in a configuration file, which is a plain text file with each row corresponding to a dataset. In this file, the book and dataset names must be in the first two columns, and the lumi list JSON file name (only the file name; the directory for the file is given by MIT_JSON_DIR environment variable). The list of datasets is stored in the workspace. When this script is invoked for an existing workspace with no dataset specification, all datasets in the stored configuration will be checked, and the failed jobs will be resubmitted.
    When a new workspace is created, tarballs of the binaries, scripts, and header files necessary for the job execution is copied into the workspace. These tarballs are then shipped to condor execution directory for each job. Thus, updates on the source code and scripts in the CMSSW release area will not affect the execution of the jobs. The --update flag can be used to renew the tarballs, although this is in general not recommended, as it can lead to inconsistencies between the outputs in the same workspace.
    '''
    
    argParser = ArgumentParser(description = description, formatter_class = RawTextHelpFormatter)
    
    argParser.add_argument('--cfg', '-c', metavar = 'FILE', dest = 'configFileName', help = 'A plain text file listing the book, the dataset, and the json file per row.')
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = '')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset')
    argParser.add_argument('--skim', '-k', metavar = 'SKIM', dest = 'skim', default = '-')
    argParser.add_argument('--filesets', '-s', metavar = 'FILESET', dest = 'filesets', nargs = '*', default = [])
    argParser.add_argument('--num-files', '-i', metavar = 'N', dest = 'numFiles', type = int, default = 0, help = 'Process N files per job (Job creation time only). Set to 0 for default value.')
    argParser.add_argument('--goodlumi', '-j', metavar = 'FILE', dest = 'goodlumiFile', default = '~')
    argParser.add_argument('--data', '-D', action = 'store_true', dest = 'realData', help = 'Process real data (sets the real-data flag on various modules).')
    argParser.add_argument('--custom', '-m', metavar = 'EXPR', dest = 'custom', nargs = '+', help = 'Custom variables passed to the analysis object. EXPR is any number of space-separated expressions of form key=value.')
    argParser.add_argument('--analysis', '-a', metavar = 'ANALYSIS', dest = 'macro', help = 'Analysis python script that sets up the execution sequence of the modules.')
    argParser.add_argument('--name', '-n', metavar = 'NAME', dest = 'taskName', default = '', help = 'Workspace name. If not given, set to the configuration file name if applicable, otherwise to current epoch time.')
    argParser.add_argument('--recreate', '-R', action = 'store_true', dest = 'recreate', help = 'Clear the existing workspace if there is one.')
    argParser.add_argument('--update', '-U', action = 'store_true', dest = 'update', help = 'Update the libraries / scripts / headers.')
    argParser.add_argument('--condor-template', '-t', metavar = 'FILE', dest = 'condorTemplatePath', default = '', help = 'Condor JDL file template. Strings {task}, {book}, {dataset}, and {fileset} can be used as placeholders in any of the lines.')
    argParser.add_argument('--pilot', '-p', metavar = 'N', dest = 'pilot', type = int, nargs = '?', default = 0, const = 1000, help = 'Submit a pilot job that processes N events from each dataset.')
    argParser.add_argument('--submit-from', '-f', metavar = 'HOST', dest = 'submitFrom', default = '', help = 'Submit the jobs from HOST.')
    argParser.add_argument('--no-submit', '-C', action = 'store_true', dest = 'noSubmit', help = 'Prepare the workspace without submitting jobs.')
    argParser.add_argument('--resubmit-held', '-H', action = 'store_true', dest = 'resubmitHeld', help = 'Resubmit held jobs automatically. By default held jobs count as running and are skipped.')
    argParser.add_argument('--kill', '-K', action = 'store_true', dest = 'kill', help = 'Kill running jobs of the task.')
    
    args = argParser.parse_args()
    sys.argv = []
    
    if args.macro and not os.path.exists(args.macro):
        print ' Analysis configuration file ' + args.macro + ' does not exist'
        sys.exit(1)

    if args.realData and args.goodlumiFile == '~':
        args.goodlumiFile = '-'
    
    if args.configFileName:
        if not os.path.exists(args.configFileName):
            print ' Task configuration file ' + args.configFileName + ' does not exist'
            sys.exit(1)

        if args.dataset or args.goodlumiFile:
            print ' Configuration file name is given. --dataset and --goodlumi options are ignored.'

    if args.update and not args.taskName:
        print ' Specify the task name to update.'
        sys.exit(1)

    # create a struct to hold various directory and file names
    class Env(object):
        pass

    env = Env()

    env.taskName = args.taskName
    if not env.taskName:
        if args.configFileName:
            env.taskName = os.path.basename(args.configFileName[:args.configFileName.find('.')])
        else:
            env.taskName = str(int(time.time()))
   
    try:
        env.workspace = os.environ['HOME'] + '/cms/condor/' + env.taskName
        env.outDir = os.environ['MIT_PROD_HIST'] + '/' + env.taskName
        env.logDir = os.environ['MIT_PROD_LOGS'] + '/' + env.taskName

        env.scramArch = os.environ['SCRAM_ARCH']
        env.cmsswbase = os.environ['CMSSW_BASE']
        env.release = os.path.basename(os.environ['CMSSW_RELEASE_BASE'])

        env.mitdata = os.environ['MIT_DATA']

        env.catalogDir = os.environ['MIT_CATALOG']
    
    except KeyError, err:
        print ' Environment', err.args[0], ' not set'
        sys.exit(1)

    env.inMacroPath = args.macro
    env.update = args.update
    env.condorTemplatePath = args.condorTemplatePath
    env.submitFrom = args.submitFrom
    env.noSubmit = args.noSubmit

    env.cmsswdir = os.path.dirname(env.cmsswbase)
    env.mitTag = os.path.basename(env.cmsswdir)
    env.cmsswname = os.path.basename(env.cmsswbase)
    env.cmsswPack = env.cmsswname + '.tar.gz'
    env.libPack = env.cmsswname + '.lib.tar.gz'
    env.hdrPack = env.cmsswname + '.inc.tar.gz'
    env.binPack = env.cmsswname + '.MitAna-bin.tar.gz'

    newTask = True

    if os.path.isdir(env.workspace):
        if args.recreate:
            message = ' Task ' + env.taskName + ' already exists.\n'
            message += ' You are requesting this to be a new production task. All existing files will be cleared.\n'
            message += ' Clear and continue?'
            resp = yes(message, [('yes', 1), ('no, save existing', 2), ('quit', 0)])
            if resp == 0:
                print ' Exiting.'
                sys.exit(0)

            else:
                if resp == 1:
                    print ' Removing directories:'
                    for directory in [env.workspace, env.logDir, env.outDir]:
                        print '  ' + directory
                        try:
                            shutil.rmtree(directory)
                        except:
                            pass
    
                elif resp == 2:
                    print ' Existing output will be moved to tag (default \'last\'):'
                    tag = sys.stdin.readline().strip()
                    if tag == '':
                        tag = 'last'
    
                    print ' Moving directories'
                    for directory in [env.workspace, env.logDir, env.outDir]:
                        print '  ' + directory + ' -> ' + directory + '-' + tag
                        try:
                            os.rename(directory, directory + '-' + tag)
                        except OSError:
                            print ' Move failed. Exiting.'
                            sys.exit(1)

        elif args.update:
            message = ' Updating ' + env.taskName + ' to the latest:\n'
            message += ' . Binaries\n'
            message += ' . Headers\n'
            message += ' . MitAna/bin\n'
            if env.inMacroPath:
                message += ' . ' + env.inMacroPath + '\n'
            if args.configFileName or args.dataset:
                message += ' . List of datasets\n'
            if args.condorTemplatePath:
                message += ' . Condor job description\n'

            message += ' The output of the task may become inconsistent with the existing ones after this operation.\n'
            message += ' Do you wish to continue?'
            if not yes(message):
                print ' Exiting.'
                sys.exit(0)

            newTask = False

        else:
            newTask = False
   
    if newTask:
        print ' Creating task', env.taskName

    if newTask or args.update:
        ready = setupWorkspace(env)
        if not ready:
            sys.exit(1)

    x509File = '/tmp/x509up_u' + str(os.getuid())
    if os.path.exists(x509File):
        shutil.copyfile(x509File, env.workspace + '/x509up')
#    else:
#        message = ' x509 proxy missing. You will not be able to download files from T2 in case T3 cache does not exist.\n'
#        message += ' Continue?'
#        if not yes(message):
#            print ' Exiting.'
#            sys.exit(0)
   
    # datasets: list of tuples (book, dataset, json)
    updateDatasetList = False

    if args.configFileName:
        if not newTask and not env.update:
            print ' A dataset list was given, but we are not updating the workspace. Use --update flag to append new datasets.'
            sys.exit(1)

        updateDatasetList = True
        datasets = readDatasetList(args.configFileName)
    else:
        currentList = readDatasetList(env.workspace + '/datasets.list')
        if args.dataset:
            datasets = []

            inDataset = DatasetInfo(book = args.book, dataset = args.dataset)

            inDataset.skim = args.skim
            inDataset.json = args.goodlumiFile
            if args.custom is not None:
                for expr in args.custom:
                    key, eq, value = expr.partition('=')
                    if not value:
                        continue

                    value = parseValueStr(value)
                    inDataset.custom[key] = value

            for datasetInfo in currentList:
                if datasetInfo.dataset == args.dataset:
                    if not inDataset.book:
                        inDataset.book = datasetInfo.book
                    if not inDataset.skim:
                        inDataset.skim = datasetInfo.skim
                    if not inDataset.json:
                        inDataset.json = datasetInfo.json
                    if len(inDataset.custom) == 0:
                        inDataset.custom.update(datasetInfo.custom)

                    datasets.append(inDataset)
                    if inDataset != datasetInfo:
                        updateDatasetList = True

            if len(datasets) == 0:
                if not newTask and not env.update:
                    print ' A dataset was given, but we are not updating the workspace. Use --update flag to append new datasets.'
                    sys.exit(1)

                if inDataset.book == '':
                    print ' Specify a book to add the dataset from.'
                    sys.exit(1)

                message = ' Add dataset config?\n'
                message += '  book    = ' + inDataset.book + '\n'
                message += '  dataset = ' + inDataset.dataset + '\n'
                message += '  skim    = ' + inDataset.skim + '\n'
                message += '  json    = ' + inDataset.json
                if len(inDataset.custom) != 0:
                    message += '\n  custom    = ' + str(inDataset.custom)

                if yes(message):
                    updateDatasetList = True
                    datasets.append(inDataset)
                else:
                    sys.exit(0)

        else:
            datasets = currentList

    if updateDatasetList:
        # write updates to the list of datasets
        updateDatasetList = writeDatasetList(env.workspace + '/datasets.list', datasets)

    # create the condor jdl object
    condorConf = CondorConfig(env.taskName)
   
    if newTask or (args.update and args.condorTemplatePath):
        path = args.condorTemplatePath
        if not path:
            path = env.cmsswbase + '/src/MitAna/config/condor_template.jdl'

        condorConf.readFromFile(path, env)
        condorConf.writeToFile(env.workspace + '/condor.jdl')
    else:
        condorConf.readFromFile(env.workspace + '/condor.jdl', env)

    # set directory names and make directories if needed
    for datasetInfo in datasets:
        # condorConf to take care of remaps
        # will make directories if they don't exist yet
        datasetInfo.setDirNames(env, condorConf)

    if updateDatasetList:
        writeCatalogs(env.catalogDir, datasets, filesPerFileset = args.numFiles)

    if updateDatasetList or (env.update and env.inMacroPath):
        writeMacros(env.workspace, datasets, noOverwrite = env.update and not env.inMacroPath)

    print ' Checking for running jobs..'

    if args.kill:
        killStatus = range(7)
    elif args.resubmitHeld:
        killStatus = [5]
    else:
        killStatus = []

    makeJobs(datasets, condorConf, limitTo = args.filesets, killStatus = killStatus)

    if newTask:
        jobsInQueue = []
        for datasetInfo in datasets:
            jobsInQueue += [jobInfo for jobInfo in datasetInfo.jobs.values() if jobInfo.jobId != '']
            
        if len(jobsInQueue) != 0:
            message = ' New task was requested but some jobs are in condor queue.\n'
            message += ' Kill jobs?'
            if yes(message):
                for jobInfo in jobsInQueue:
                    killJob(jobInfo)
                    jobInfo.dataset.jobs.pop(jobInfo.fileset)

        else:
            print ' Cannot continue while jobs are running. Exit.'
            sys.exit(1)
    
    if args.kill:
        sys.exit(0)

    # if pilot is requested, update allFilesets with the pilot information
    if args.pilot != 0:
        for datasetInfo in datasets:
            datasetInfo.jobs = {'pilot': JobInfo(datasetInfo, 'pilot', nentries = args.pilot)}

    # loop over datasets to submit
    submitJobs(env, datasets, condorConf)
