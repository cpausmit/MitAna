#!/usr/bin/env python

import os
import shutil
import importlib

def configureSequence(args):
    # analysis and mithep defined in config
    
    if args.flatConfig:
        # copy the config to a temporary python directory to allow import
        bambutmp = os.environ['CMSSW_BASE'] + '/python/bambutmp'
        try:
            os.mkdir(bambutmp)
            open(bambutmp + '/__init__.py', 'w').close()
        except:
            pass
    
        shutil.copy(args.config, bambutmp)
        cfg = importlib.import_module('bambutmp.' + os.path.basename(args.config).replace('.py', ''))
        shutil.rmtree(bambutmp)

        try:
            analysis = cfg.analysis
        except AttributeError:
            print 'Analysis object not defined in ' + args.config
            sys.exit(1)

        try:
            mithep = cfg.mithep
        except AttributeError:
            import ROOT
            ROOT.gSystem.Load('libMitAnaTreeMod.so')
            mithep = ROOT.mithep

        if args.goodlumiFile is not None:
            # RunLumiSelectionMod assumed to be the first module
            try:
                jsonDir = os.environ['MIT_JSON_DIR']
                for sm in analysis.GetSuperMods():
                    if sm.Class() != mithep.RunLumiSelectionMod.Class():
                        continue

                    sm.AddJSONFile(jsonDir + '/' + args.goodlumiFile)

            except:
                print 'Lumi list configuration failed.'
                sys.exit(1)

    else:
        from MitAna.TreeMod.bambu import analysis, mithep

        if args.realData or (args.goodlumiFile is not None):
            analysis.isRealData = True

        execfile(args.config)

        # if good run / lumi list is given, prepend the filter module
        if args.goodlumiFile is not None:
            from MitAna.PhysicsMod.runlumisel import goodLumiFilter
            filterMod = goodLumiFilter(args.goodlumiFile)
            analysis._sequence = filterMod * analysis._sequence
    
        analysis.buildSequence()

    return mithep, analysis


if __name__ == '__main__':
    from argparse import ArgumentParser
    import sys
    import os
    
    argParser = ArgumentParser(description = 'Run BAMBU analysis')
    argParser.add_argument('config', metavar = 'CONFIG', help = 'Analysis macro defining the module sequence.')
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = 't2mit/filefi/041', help = 'Input book, e.g. t2mit/filefi/041.')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset', help = 'Input dataset.')
    argParser.add_argument('--fileset', '-s', metavar = 'FILESET', dest = 'fileset', default = '0000', help = 'Input fileset.')
    argParser.add_argument('--file', '-f', metavar = 'INPUT', dest = 'inputFiles', nargs = '*', help = 'Input file name when running on a single file.')
    argParser.add_argument('--goodlumi', '-j', metavar = 'FILE', dest = 'goodlumiFile', help = 'Input good lumi JSON file.')
    argParser.add_argument('--output', '-o', metavar = 'FILENAME', dest = 'outputFile', help = 'Output file name.')
    argParser.add_argument('--nentries', '-n', metavar = 'N', dest = 'nentries', type = int, default = -1, help = 'Number of entries to process.')
    argParser.add_argument('--data', '-D', action = 'store_true', dest = 'realData', help = 'Process real data (sets the real-data flag on various modules).')
    argParser.add_argument('--hierarchy', '-E', action = 'store_true', dest = 'hierarchy', help = 'Create hierarchical output.')
    argParser.add_argument('--flat', '-F', action = 'store_true', dest = 'flatConfig', help = 'Input config is flat, i.e. has no imports (used for batch submission)')
    
    args = argParser.parse_args()
    sys.argv = []
    
    if args.dataset:
        if args.book == '' or args.dataset == '' or args.fileset == '':
            print 'book, dataset, and fileset must be set together.'
            sys.exit(1)
    
    if args.inputFiles and args.dataset:
        print 'Cannot specify file and dataset at the same time.'
        sys.exit(1)

    mithep, analysis = configureSequence(args)

    # set up input (dataset / fileset or individual file)
    if args.dataset:
        try:
            catalog = mithep.Catalog(os.environ['MIT_CATALOG'])
        except KeyError:
            print 'Dataset catalog requires environment MIT_CATALOG to be set.'
            sys.exit(1)

        dataset = catalog.FindDataset(args.book, args.dataset, args.fileset, 1)
        analysis.AddDataset(dataset)
        analysis.SetUseCacher(1)
        
    elif args.inputFiles:
        for f in args.inputFiles:
            analysis.AddFile(f)

    if args.outputFile:
        analysis.SetOutputName(args.outputFile)
    
    if args.nentries >= 0:
        analysis.SetProcessNEvents(args.nentries)
    
    if args.hierarchy:
        analysis.SetKeepHierarchy(True)

    print '\n+++++ ANALYSIS FLOW +++++\n'
    analysis.PrintModuleTree()
    print '\n+++++++++++++++++++++++++\n'
    
    analysis.Run(False)
