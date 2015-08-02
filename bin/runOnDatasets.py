#!/usr/bin/env python

import sys
import os
import re
import time
import subprocess
import glob
import shutil
import socket

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


def runSubproc(*args, **kwargs):
    """
    Run external process using subprocess.Popen
    """

    proc = subprocess.Popen(list(args), stdout = subprocess.PIPE, stderr = subprocess.PIPE, stdin = subprocess.PIPE)

    if 'stdin' in kwargs and type(kwargs['stdin']) is str:
        stdin = kwargs['stdin']
    else:
        stdin = None
            
    out, err = proc.communicate(stdin)

    if 'stdout' not in kwargs or kwargs['stdout'] is not None:
        if out.strip():
            print out

    if 'stderr' not in kwargs or kwargs['stderr'] is not None:
        if err.strip():
            sys.stderr.write(err)
            sys.stderr.flush()


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

        # write out the list of directories related to this job
        # will be updated in submitJobs() if transfer_output_remaps is used
        with open(env.workspace + '/directories', 'w') as dirList:
            dirList.write(env.workspace + '\n')
            dirList.write(env.outDir + '\n')
            dirList.write(env.logDir + '\n')

    if env.inMacroPath: # including update case
        shutil.copyfile(env.inMacroPath, env.workspace + '/macro.py')

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
        for line in configFile:
            matches = re.match('([^#][^ ]*) +([^ ]+) +([^ ]+)( +[^ ]+)*', line.strip())
            if not matches:
                continue

            book = matches.group(1)
            dataset = matches.group(2)
            skim = matches.group(3)
            # fourth paren captures the last column, which is currently assigned to JSON
            if matches.group(4):
                json = matches.group(4).strip()
            else:
                json = ''

            if skim == 'noskim':
                skim = '-'

            datasets.append((book, dataset, skim, json))

    return datasets


def writeDatasetList(fileName, datasets):
    """
    Write the list of datasets into the dataset configuration file.
    """

    fileContent = readDatasetList(fileName)

    diff = []
    for book, dataset, skim, json in datasets:
        for exBook, exDataset, exSkim, exJson in fileContent:
            if book == exBook and dataset == exDataset:
                if skim != exSkim or json != exJson:
                    message = ' Specified skim/JSON configuration for ' + book + '/' + dataset + ' does not match the existing.\n'
                    message += ' Update configuration?\n'
                    message += ' ' + exSkim + ' -> ' + skim + '\n'
                    message += ' ' + exJson + ' -> ' + json
                    if yes(message):
                        fileContent.remove((exBook, exDataset, exSkim, exJson))
                        fileContent.append((book, dataset, skim, json))
                
                break

        else:
            fileContent.append((book, dataset, skim, json))

    with open(fileName, 'w') as configFile:
        for book, dataset, skim, json in fileContent:
            configFile.write(book + ' ' + dataset + ' ' + skim + ' ' + json + '\n')


def setupDatasetDirs(datasets, env):
    """
    Create a directory for each dataset under log and output directories.
    """

    for book, dataset, skim, json in datasets:
        jobOutDirName = env.outDir + '/' + book + '/' + dataset
        jobLogDirName = env.logDir + '/' + book + '/' + dataset
        jobConfDirName = env.workspace + '/' + book + '/' + dataset

        # job output (histograms) destination; if no remap is used
        if not os.path.exists(jobOutDirName):
            os.makedirs(jobOutDirName)

        # .log, .out, and .err files
        if not os.path.exists(jobLogDirName):
            os.makedirs(jobLogDirName)

        # catalog and analysis macro
        if not os.path.exists(jobConfDirName):
            os.makedirs(jobConfDirName)


def getFilesets(env, datasets, limitTo = []):
    """
    Open the catalog and read out the list of filesets. Write out the catalog to
    the workspace (fileset size may be different depending on env.numFiles).
    """

    allFilesets = {}

    for book, dataset, skim, json in datasets:
        catalogSrc = env.catalogDir + '/' + book + '/' + dataset
        if skim != '-':
            catalogSrc += '/' + skim
        catalogDst = env.workspace + '/' + book + '/' + dataset

        if not os.path.exists(catalogDst + '/Files'):
            # original catalog is not created yet. Copy or write
            set0size = 0
            try:
                with open(catalogSrc + '/Files') as fileList:
                    for line in fileList:
                        if line.split()[0] == '0000':
                            set0size += 1
            except IOError:
                print ' Could not open catalog ' + catalogSrc + '. Skipping dataset.'
                continue
    
            if env.numFiles == 0 or env.numFiles >= set0size:
                # using default fileset size
                for fileName in os.listdir(catalogSrc):
                    if os.path.isfile(catalogSrc + '/' + fileName):
                        shutil.copyfile(catalogSrc + '/' + fileName, catalogDst + '/' + fileName)
            else:
                # fileset size smaller than default
                with open(catalogSrc + '/Files') as src:
                    with open(catalogDst + '/Files', 'w') as dst:
                        iLine = 0
                        for line in src:
                            dst.write('%04d ' % (iLine / env.numFiles))
                            dst.write(' '.join(line.split()[1:]) + '\n')
                            iLine += 1

                nFilesets = iLine / env.numFiles
                if iLine % env.numFiles != 0:
                    nFilesets += 1
                
                with open(catalogSrc + '/Filesets') as src:
                    location = src.readline().split()[1]

                with open(catalogDst + '/Filesets', 'w') as dst:
                    # write dummy data
                    for iSet in range(nFilesets):
                        dst.write('%04d %s 0 0 0 0 0 0\n' % (iSet, location))


        filesets = []
        with open(catalogDst + '/Filesets') as filesetList:
            for line in filesetList:
                filesetId = line.strip().split()[0]
                if len(limitTo) != 0 and filesetId not in limitTo:
                    continue
    
                filesets.append(filesetId)
    
        if len(filesets) != 0:
            allFilesets[(book, dataset)] = filesets

    return allFilesets


def writeMacros(datasets, env):
    """
    Parse the input macro and write a standalone python script for each dataset.
    """

    noOverwrite = env.update and not env.inMacroPath

    from MitAna.TreeMod.bambu import mithep, analysis
    from MitAna.PhysicsMod.runlumisel import goodLumiFilter

    # read from the orignal catalog created in workspace
    catalog = mithep.Catalog(env.workspace)

    for book, dataset, skim, json in datasets:
        fileName = env.workspace + '/' + book + '/' + dataset + '/run.py'

        if os.path.exists(fileName) and noOverwrite:
            continue

        print ' Writing analysis macro for ' + book + '/' + dataset

        # reset and build the analysis
        analysis.reset()

        if env.realData or (json != '' and json != '~'):
            analysis.isRealData = True

        execfile(env.workspace + '/macro.py')

        if json and json != '~' and json != '-':
            analysis._sequence = goodLumiFilter(json) * analysis._sequence

        analysis.buildSequence()

        # get the dataset file list
        ds = catalog.FindDataset(book, dataset, '', 1)
        # would be much nicer if mithep.Dataset had an interface that exposes fileset names
        filesPerFileset = 0
        with open(env.workspace + '/' + book + '/' + dataset + '/Files') as fileList:
            for line in fileList:
                if line.split()[0] == '0000':
                    filesPerFileset += 1

        with open(fileName, 'w') as macro:
            macro.write('import sys\n')
            macro.write('import ROOT\n\n')
            macro.write('fileset = sys.argv[1]\n\n')
            for lib in mithep.loadedLibs:
                macro.write('ROOT.gSystem.Load(\'' + lib + '\')\n')

            macro.write('\nfiles = {\n')
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
            macro.write('analysis.SetUseCacher(1)\n\n')
            macro.write('for f in files[fileset]:\n')
            macro.write('    analysis.AddFile(f)\n\n')
            macro.write('analysis.SetOutputName(fileset + \'.root\')\n\n')
            macro.write(analysis.dumpPython(varName = 'analysis', withCtor = False, objects = {}))
            macro.write('\nanalysis.Run(False)\n')


def getRunningJobs(iwdParent):
    proc = subprocess.Popen(['condor_q', '-global', '-long', '-attributes', 'Owner,ClusterId,ProcId,Iwd,Args,Arguments'], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
    out, err = proc.communicate()

    running = {}
    
    block = {}
    for line in out.split('\n'):
        if line.strip() == '':
            # one job block ended
            if len(block) == 0:
                continue

            if block['Owner'].strip('"') != os.environ['USER'] or not block['Iwd'].strip('"').startswith(iwdParent):
                block = {}
                continue
    
            try:
                book, dataset, fileset = block['Arguments'].strip('"').split()[:3]
            except:
                try:
                    book, dataset, fileset = block['Args'].strip('"').split()[:3]
                except:
                    block = {}
                    continue
            
            running[(book, dataset, fileset)] = block['ClusterId'] + '.' + block['ProcId']
            block = {}
            continue
            
        cfg = line.partition('=')
        block[cfg[0].strip()] = cfg[2].strip()

    return running


def readCondorConf(path, condorConfig = {}):
    with open(path) as confFile:
        for line in confFile:
            if not line.strip() or line.strip().startswith('#'):
                continue

            key, eq, value = line.partition('=')
            if key in condorConfig:
                print ' Ignoring key ' + key + ' found in condor template.'
                continue

            condorConfig[key.strip().lower()] = value.strip()

    return condorConfig


def writeCondorConf(inTemplatePath, env):
    condorConfig = {
        'arguments': '"{book} {dataset} {fileset}"',
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

    readCondorConf(inTemplatePath, condorConfig)

    if 'executable' not in condorConfig:
        condorConfig['executable'] = os.environ['HOME'] + '/cms/cmssw/' + env.mitTag + '/' + env.cmsswname + '/src/MitAna/bin/start.sh'

    if 'transfer_input_files' not in condorConfig:
        inputList = ['{book}/{dataset}/run.py', env.cmsswPack, 'env.sh']
        if os.path.exists(env.workspace + '/x509up'):
            inputList.append('x509up')

        condorConfig['transfer_input_files'] = ','.join(map(lambda x: env.workspace + '/' + x, inputList))

    with open(env.workspace + '/condor.jdl', 'w') as jdlFile:
        for key, value in condorConfig.items():
            jdlFile.write(key + ' = ' + value + '\n')


def submitJobs(env, datasets, allFilesets, runningJobs):
    condorConfig = readCondorConf(env.workspace + '/condor.jdl')

    # list of output / log directories for this task
    # (update if remap is used)
    directories = []
    updateDirectories = False
    with open(env.workspace + '/directories') as dirList:
        for line in dirList:
            directories.append(line.strip())

    for book, dataset, skim, json in datasets:
        outPaths = {}
        if 'transfer_output_files' in condorConfig:
            outputs = map(str.strip, condorConfig['transfer_output_files'].split(','))
    
            if 'transfer_output_remaps' in condorConfig:
                remapDefs = map(str.strip, condorConfig['transfer_output_remaps'].strip('"').split(';'))
        
                for remapDef in remapDefs:
                    source, eq, target = map(str.strip, remapDef.partition('='))
                    outPaths[source] = target

                    if os.path.dirname(target) not in directories:
                        directories.append(os.path.dirname(target))
                        updateDirectories = True
        
            for output in outputs:
                if output not in outPaths:
                    outPaths[output] = env.outDir + '/{book}/{dataset}/' + os.path.basename(output)

        # loop over filesets and do actual submission
        for fileset in allFilesets[(book, dataset)]:
            def formatCfg(s):
                return s.format(task = env.taskName, book = book, dataset = dataset, fileset = fileset)

            # skip fileset if a job is running
            if (book, dataset, fileset) in runningJobs:
                print ' Running: ', book, dataset, fileset, '(' + runningJobs[(book, dataset, fileset)] + ')'
                continue

            # skip fileset if at least one fileset has a non-zero output
            outputExists = False
            for outPath in map(formatCfg, outPaths.values()):
                if not os.path.isdir(os.path.dirname(outPath)):
                    os.makedirs(os.path.dirname(outPath))

                if os.path.exists(outPath) and os.stat(outPath).st_size != 0:
                    print ' Output exists:', book, dataset, fileset
                    outputExists = True
                    break

            if outputExists:
                continue

            print ' Submitting:', book, dataset, fileset
    
            jdlCommand = '\n'.join([key + ' = ' + formatCfg(value) for key, value in condorConfig.items()]) + '\nqueue\n'
            runSubproc('condor_submit', stdin = jdlCommand)

    if updateDirectories:
        with open(env.workspace + '/directories', 'w') as dirList:
            for directory in directories:
                dirList.write(directory + '\n')


if __name__ == '__main__':
    from argparse import ArgumentParser, RawTextHelpFormatter

    if 'CMSSW_BASE' not in os.environ:
        print ' CMSSW_BASE not set.'
        sys.exit(1)
    
    description ='''Submit a BAMBU analysis to cluster.
    When a workspace name is specified and the workspace already exists in thee standard location, the workspace is recreated (--recreate) or checked for failed jobs. The dataset(s) to be processed can be passed from the command line (--book, --dataset, and --filesets (optional)) or listed in a configuration file, which is a plain text file with each row corresponding to a dataset. In this file, the book and dataset names must be in the first two columns, and the lumi list JSON file name (only the file name; the directory for the file is given by MIT_JSON_DIR environment variable). The list of datasets is stored in the workspace. When this script is invoked for an existing workspace with no dataset specification, all datasets in the stored configuration will be checked, and the failed jobs will be resubmitted.
    When a new worksapce is created, tarballs of the binaries, scripts, and header files necessary for the job execution is copied into the workspace. These tarballs are then shipped to condor execution directory for each job. Thus, updates on the source code and scripts in the CMSSW release area will not affect the execution of the jobs. The --update flag can be used to renew the tarballs, although this is in general not recommended, as it can lead to inconsistencies between the outputs in the same worksapce.
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
    argParser.add_argument('--analysis', '-a', metavar = 'ANALYSIS', dest = 'macro', help = 'Analysis python script that sets up the execution sequence of the modules.')
    argParser.add_argument('--name', '-n', metavar = 'NAME', dest = 'taskName', default = '', help = 'Workspace name. If not given, set to the configuration file name if applicable, otherwise to current epoch time.')
    argParser.add_argument('--recreate', '-R', action = 'store_true', dest = 'recreate', help = 'Clear the existing workspace if there is one.')
    argParser.add_argument('--update', '-U', action = 'store_true', dest = 'update', help = 'Update the libraries / scripts / headers.')
    argParser.add_argument('--condor-template', '-t', metavar = 'FILE', dest = 'condorTemplatePath', default = '', help = 'Condor JDL file template. Strings {task}, {book}, {dataset}, and {fileset} can be used as placeholders in any of the lines.')
    argParser.add_argument('--no-submit', '-C', action = 'store_true', dest = 'noSubmit', help = 'Prepare the workspace without submitting jobs.')
    
    args = argParser.parse_args()
    sys.argv = []
    
    if args.macro and not os.path.exists(args.macro):
        print ' Analysis configuration file ' + args.macro + ' does not exist'
        sys.exit(1)
    
    if args.configFileName:
        if not os.path.exists(args.configFileName):
            print ' Task configuration file ' + args.configFileName + ' does not exist'
            sys.exit(1)

        if args.dataset or args.goodlumiFile:
            print ' Configuration file name is given. --dataset and --goodlumi options are ignored.'

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
    env.realData = args.realData
    env.numFiles = args.numFiles
    env.update = args.update
    env.condorTemplatePath = args.condorTemplatePath

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

            elif resp == 1:
                try:
                    directories = []
                    with open(env.workspace + '/directories') as dirList:
                        for line in dirList:
                            directories.append(dirList.strip())
                except:
                    directories = [env.workspace, env.outDir, env.logDir]

                print ' Removing directories:'
                for directory in directories:
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
                for directory in directories:
                    print '  ' + directory
                print ' to'
                for directory in directories:
                    print '  ' + directory + '-' + tag

                for directory in directories:
                    os.rename(directory, directory + '-' + tag)


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
    else:
        message = ' x509 proxy missing. You will not be able to download files from T2 in case T3 cache does not exist.\n'
        message += ' Continue?'
        if not yes(message):
            print ' Exiting.'
            sys.exit(0)
   
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
            for entry in currentList:
                if entry[1] == args.dataset:
                    datasets.append(entry)

            if len(datasets) == 0:
                if not newTask and not env.update:
                    print ' A dataset was given, but we are not updating the workspace. Use --update flag to append new datasets.'
                    sys.exit(1)

                if args.book == '':
                    print ' Specify a book to add the dataset from.'
                    sys.exit(1)

                message = ' Add dataset config?\n'
                message += '  book    = ' + args.book + '\n'
                message += '  dataset = ' + args.dataset + '\n'
                message += '  skim    = ' + args.skim + '\n'
                message += '  json    = ' + args.goodlumiFile

                if yes(message):
                    updateDatasetList = True
                    datasets.append((args.book, args.dataset, args.skim, args.goodlumiFile))
                else:
                    sys.exit(0)

        else:
            datasets = currentList

    if updateDatasetList:
        # write updates to the list of datasets
        writeDatasetList(env.workspace + '/datasets.list', datasets)
        setupDatasetDirs(datasets, env)

    allFilesets = getFilesets(env, datasets, args.filesets)

    if updateDatasetList or (env.update and env.inMacroPath):
        writeMacros(datasets, env)
   
    if len(allFilesets) == 0:
        print ' No valid fileset found.'
        sys.exit(1)

    if newTask or (args.update and args.condorTemplatePath):
        path = args.condorTemplatePath
        if not path:
            path = env.cmsswbase + '/src/MitAna/config/condor_template.jdl'

        writeCondorConf(path, env)

    if args.noSubmit:
        sys.exit(0)

    print ' Checking for running jobs..'
    
    runningJobs = getRunningJobs(env.outDir)

    if newTask and len(runningJobs) != 0:
        message = ' New task was requested but some jobs are running.\n'
        message += ' Kill jobs?'
        if yes(message):
            print ' Killing jobs:'
            for book, dataset, fileset in sorted(runningJobs.keys()):
                print ' ', book, dataset, fileset
                runSubproc('condor_rm', runningJobs[(book, dataset, fileset)], stdout = None, stderr = None)

            runningJobs = {}
    
        else:
            print ' Cannot continue while jobs are running. Exit.'
            sys.exit(1)

    # loop over datasets to submit
    submitJobs(env, datasets, allFilesets, runningJobs)
