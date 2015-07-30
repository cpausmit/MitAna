#!/usr/bin/env python

import sys
import os
import re
import time
import subprocess
import glob
import shutil
import socket

def yesno():
    while True:
        response = sys.stdin.readline().strip()
        if response == 'y':
            return True
        elif response == 'N':
            return False
        else:
            print ' [y/N]:'


def runSubproc(*args, **kwargs):
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


def setupTask(env):
    if not env.update and not env.inMacroPath:
        print ' No analysis configuration specified. Set by --analysis=config_path'
        return False

    # write out environment variables
    with open(env.taskDir + '/' + env.envFile, 'w') as envFile:
        envFile.write('export SCRAM_ARCH="' + env.scramArch + '"\n')
        envFile.write('export CMSSW_RELEASE="' + env.release + '"\n')
        envFile.write('export MIT_DATA="' + env.mitdata + '"\n')
        envFile.write('export MIT_JSON_DIR="' + env.jsondir + '"\n')

    if not env.update or env.inMacroPath:
        # write out a flat (no imports) analysis macro
        print ' Writing analysis macro', env.macro
    
        from analysis import configureSequence
        configArgs = Env()
        configArgs.config = env.inMacroPath
        configArgs.flatConfig = False
        configArgs.realData = env.realData
        if env.realData:
            configArgs.goodlumiFile = ''
        else:
            configArgs.goodlumiFile = None

        mithep, analysis = configureSequence(configArgs)
    
        with open(env.taskDir + '/' + env.macro, 'w') as macro:
            macro.write('import ROOT\n')
            for lib in mithep.loadedLibs:
                macro.write('ROOT.gSystem.Load(\'' + lib + '\')\n')
            macro.write('\nmithep = ROOT.mithep\n\n')
            macro.write(analysis.dumpPython())

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

    shutil.copy2(env.cmsswdir + '/' + env.libPack, env.taskDir + '/' + env.libPack)

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

    shutil.copy2(env.cmsswdir + '/' + env.hdrPack, env.taskDir + '/' + env.hdrPack)

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
    
    shutil.copy2(env.cmsswdir + '/' + env.binPack, env.taskDir + '/' + env.binPack)

    if not env.update:
        # create an empty dataset list file
        open(env.taskDir + '/datasets.list', 'w').close()

    return True


def readDatasetList(fileName):
    datasets = []

    with open(fileName) as configFile:
        for line in configFile:
            matches = re.match('([^ ]+) +([^ ]+)( +[^ ]+)*', line.strip())
            if not matches:
                continue

            book = matches.group(1)
            dataset = matches.group(2)
            # third paren captures the last column, which is currently assigned to JSON
            if matches.group(3):
                json = matches.group(3).strip()
            else:
                json = ''

            datasets.append((book, dataset, json))

    return datasets


def writeDatasetList(fileName, datasets):
    fileContent = readDatasetList(fileName)

    diff = []
    for book, dataset, json in datasets:
        for exBook, exDataset, exJson in fileContent:
            if book == exBook and dataset == exDataset:
                if json != exJson:
                    print ' Specified JSON file for ' + book + '/' + dataset + ' does not match the existing.'
                    print ' Update configuration?'
                    print ' ' + exJson + ' -> ' + json
                    print ' [y/N]:'
                    if yesno():
                        fileContent.remove((exBook, exDataset, exJson))
                        fileContent.append((book, dataset, json))
                
                break

        else:
            fileContent.append((book, dataset, json))

    with open(fileName, 'w') as configFile:
        for book, dataset, json in fileContent:
            configFile.write(book + ' ' + dataset + ' ' + json + '\n')


def getFilesets(catalogDir, datasets, limitTo = []):
    allFilesets = {}

    for book, dataset, json in datasets:
        filesets = []
        with open(catalogDir + '/' + book + '/' + dataset + '/Filesets') as catalog:
            for line in catalog:
                filesetId = line.strip().split()[0]
                if len(limitTo) != 0 and filesetId not in limitTo:
                    continue
    
                filesets.append(filesetId)
    
        if len(filesets) != 0:
            allFilesets[(book, dataset)] = filesets

    return allFilesets


def getRunningJobs(iwdParent):
    proc = subprocess.Popen(['condor_q', '-submitter', os.environ['USER'], '-long', '-attributes', 'ClusterId,ProcId,Iwd,Args,Arguments'], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
    out, err = proc.communicate()

    running = {}
    
    block = {}
    for line in out.split('\n'):
        if line.strip() == '':
            if len(block) == 0:
                continue

            # one job block ended
            if not block['Iwd'].strip('"').startswith(iwdParent):
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


def makeCondorConf(fileName, env):
    condorConfig = {
        'arguments': '"{book} {dataset} {fileset} {json}"',
        'initialdir': env.outDir + '/{book}/{dataset}',
        'output': env.logDir + '/{book}/{dataset}/{fileset}.out',
        'error': env.logDir + '/{book}/{dataset}/{fileset}.err',
        'log': env.logDir + '/{book}/{dataset}/{fileset}.log',     
        'universe': 'vanilla',
        'executable': env.cmsswbase + '/src/MitAna/bin/start.sh',
        'getenv': 'False',
        'input': '/dev/null',
        'should_transfer_files': 'YES',
        'when_to_transfer_output': 'ON_EXIT'
    }

    with open(fileName) as condorTemplateFile:
        for line in condorTemplateFile:
            if not re.match('#', line.strip()):
                key, eq, value = line.partition('=')
                if key in condorConfig:
                    print ' Ignoring key ' + key + ' found in condor template.'
                    continue

                condorConfig[key.strip().lower()] = value.strip()

    if 'transfer_input_files' not in condorConfig:
        inputFilesList = env.cmsswbase + '/src/MitAna/bin/analysis.py,'
        if env.x509File:
            inputFilesList += ' ' + env.x509File + ','
        inputFilesList += ' ' + env.taskDir + '/'

        condorConfig['transfer_input_files'] = inputFilesList

    # write the condor JDL template for record
    with open(env.taskDir + '/condor.jdl', 'w') as jdlFile:
        for key, value in condorConfig.items():
            jdlFile.write(key + ' = ' + value + '\n')

        jdlFile.write('queue\n')

    return condorConfig


def submitJobs(env, condorConfig, datasets, allFilesets, runningJobs):
    for book, dataset, json in datasets:
        jobOutDirName = env.outDir + '/' + book + '/' + dataset
        jobLogDirName = env.logDir + '/' + book + '/' + dataset
   
        if not os.path.exists(jobOutDirName):
            os.makedirs(jobOutDirName)
    
        if not os.path.exists(jobLogDirName):
            os.makedirs(jobLogDirName)
    
        if not os.path.exists(env.taskDir + '/catalog/' + book + '/' + dataset):
            try:
                os.mkdir(env.taskDir + '/catalog/' + book)
            except:
                pass

            shutil.copytree(env.catalogDir + '/' + book + '/' + dataset, env.taskDir + '/catalog/' + book + '/' + dataset)

        outPaths = {}
        if 'transfer_output_files' in condorConfig:
            outputs = map(str.strip, condorConfig['transfer_output_files'].split(','))
    
            if 'transfer_output_remaps' in condorConfig:
                remapDefs = map(str.strip, condorConfig['transfer_output_remaps'].strip('"').split(';'))
        
                for remapDef in remapDefs:
                    source, eq, target = map(str.strip, remapDef.partition('='))
                    outPaths[source] = target
        
            for output in outputs:
                if output not in outPaths:
                    outPaths[output] = env.outDir + '/{book}/{dataset}/' + os.path.basename(output)

        if not env.noSubmit:        
            # loop over filesets and do actual submission
            for fileset in allFilesets[(book, dataset)]:
                def formatCfg(s):
                    return s.format(task = env.taskName, book = book, dataset = dataset, fileset = fileset, json = json)

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
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = 't2mit/filefi/042')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset')
    argParser.add_argument('--filesets', '-s', metavar = 'FILESETS', dest = 'filesets', nargs = '*', default = [])
    argParser.add_argument('--goodlumi', '-j', metavar = 'FILE', dest = 'goodlumiFile', default = '')
    argParser.add_argument('--data', '-D', action = 'store_true', dest = 'realData', help = 'Process real data (sets the real-data flag on various modules).')
    argParser.add_argument('--analysis', '-a', metavar = 'ANALYSIS', dest = 'macro', help = 'Analysis python script that sets up the execution sequence of the modules.')
    argParser.add_argument('--name', '-n', metavar = 'NAME', dest = 'taskName', default = '', help = 'Workspace name. If not given, set to the configuration file name if applicable, otherwise to current epoch time.')
    argParser.add_argument('--recreate', '-R', action = 'store_true', dest = 'recreate', help = 'Clear the existing workspace if there is one.')
    argParser.add_argument('--update', '-U', action = 'store_true', dest = 'update', help = 'Update the libraries / scripts / headers.')
    argParser.add_argument('--condor-template', '-t', metavar = 'FILE', dest = 'condorTemplateName', default = os.environ['CMSSW_BASE'] + '/src/MitAna/config/condor_template.jdl', help = 'Condor JDL file template. Strings {task}, {book}, {dataset}, and {fileset} can be used as placeholders in any of the lines.')
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
    
    env.x509File = '/tmp/x509up_u' + str(os.getuid())
    if not os.path.exists(env.x509File):
        print ' x509 proxy missing. You will not be able to download files from T2 in case T3 cache does not exist.'
        print ' Continue? [y/N]:'
        if not yesno():
            print ' Exiting.'
            sys.exit(0)
    
        env.x509File = ''
    
    env.taskName = args.taskName
    if not env.taskName:
        if args.configFileName:
            env.taskName = os.path.basename(args.configFileName[:args.configFileName.find('.')])
        else:
            env.taskName = str(int(time.time()))

    env.inMacroPath = args.macro
    env.realData = args.realData
    env.update = args.update
    env.noSubmit = args.noSubmit

    try:
        env.taskDir = os.environ['HOME'] + '/cms/condor/' + env.taskName
        env.outDir = os.environ['MIT_PROD_HIST'] + '/' + env.taskName
        env.logDir = os.environ['MIT_PROD_LOGS'] + '/' + env.taskName

        env.scramArch = os.environ['SCRAM_ARCH']
        env.cmsswbase = os.environ['CMSSW_BASE']
        env.release = os.path.basename(os.environ['CMSSW_RELEASE_BASE'])

        env.mitdata = os.environ['MIT_DATA']
        env.jsondir = os.environ['MIT_JSON_DIR']
        env.catalogDir = os.environ['MIT_CATALOG']
    
    except KeyError, err:
        print ' Environment', err.args[0], ' not set'
        sys.exit(1)

    env.cmsswdir = os.path.dirname(env.cmsswbase)
    env.cmsswname = os.path.basename(env.cmsswbase)
    env.macro = 'sequence.py'
    env.envFile = 'env.sh'
    env.libPack = env.cmsswname + '.lib.tar.gz'
    env.hdrPack = env.cmsswname + '.inc.tar.gz'
    env.binPack = env.cmsswname + '.MitAna-bin.tar.gz'

    newTask = True

    if os.path.isdir(env.taskDir):
        if args.recreate:
            print ' Task ' + env.taskName + ' already exists.'
            print ' You are requesting this to be a new production task. All existing files will be cleared.'
            print ' Wanna save existing task? Do like:  moveOutput.sh', env.taskName, env.taskName + '-last.'
            print ' Do you wish to continue? [y/N]'
            if not yesno():
                print ' Exiting.'
                sys.exit(0)
    
            shutil.rmtree(env.taskDir)
            shutil.rmtree(env.outDir)
            shutil.rmtree(env.logDir)

        elif args.update:
            print ' Updating ' + env.taskName + ' to the latest:'
            print ' . Binaries'
            print ' . Headers'
            print ' . MitAna/bin'
            if env.inMacroPath:
                print ' . ' + env.inMacroPath
            print ' The output of the task may become inconsistent with the existing ones after this operation.'
            print ' Do you wish to continue? [y/N]'
            if not yesno():
                print ' Exiting.'
                sys.exit(0)

            newTask = False

        else:
            newTask = False
   
    if newTask:
        print ' Creating task', env.taskName
        os.makedirs(env.taskDir)
        os.makedirs(env.taskDir + '/catalog')
        os.mkdir(env.outDir)
        os.mkdir(env.logDir)

    if newTask or args.update:
        if not setupTask(env):
            shutil.rmtree(env.taskDir)
            shutil.rmtree(env.outDir)
            shutil.rmtree(env.logDir)
            sys.exit(1)
    
    # datasets: list of tuples (book, dataset, json)
    if args.configFileName:
        datasets = readDatasetList(args.configFileName)
    elif args.book and args.dataset:
        datasets = [(args.book, args.dataset, args.goodlumiFile)]
    else:
        datasets = readDatasetList(env.taskDir + '/datasets.list')

    # write updates to the list of datasets
    writeDatasetList(env.taskDir + '/datasets.list', datasets)
    
    allFilesets = getFilesets(env.catalogDir, datasets, args.filesets)
    
    if len(allFilesets) == 0:
        print ' No valid fileset found.'
        sys.exit(1)

    print ' Checking for running jobs..'
    
    runningJobs = getRunningJobs(env.outDir)

    if newTask and len(runningJobs) != 0:
        print ' New task was requested but some jobs are running.'
        print ' Kill jobs? [y/N]'
        if yesno():
            print ' Killing jobs:'
            for book, dataset, fileset in sorted(runningJobs.keys()):
                print ' ', book, dataset, fileset
                runSubproc('condor_rm', runningJobs[(book, dataset, fileset)], stdout = None, stderr = None)

            runningJobs = {}
    
        else:
            print ' Cannot continue while jobs are running. Exit.'
            sys.exit(1)
    
    condorConfig = makeCondorConf(args.condorTemplateName, env)

    # loop over datasets to submit
    submitJobs(env, condorConfig, datasets, allFilesets, runningJobs)
