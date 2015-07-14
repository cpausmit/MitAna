#!/usr/bin/env python

def setSequence(sequenceFile):
    execfile(sequenceFile)

if __name__ == '__main__':
    from argparse import ArgumentParser
    import sys
    import os
    
    argParser = ArgumentParser(description = 'Run BAMBU analysis')
    argParser.add_argument('config', metavar = 'CONFIG', help = 'Analysis macro defining the module sequence.')
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = 't2mit/filefi/041', help = 'Input book, e.g. t2mit/filefi/041.')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset', help = 'Input dataset.')
    argParser.add_argument('--fileset', '-s', metavar = 'FILESET', dest = 'fileset', default = '0000', help = 'Input fileset.')
    argParser.add_argument('--file', '-f', metavar = 'INPUT', dest = 'inputFile', help = 'Input file name when running on a single file.')
    argParser.add_argument('--goodlumi', '-j', metavar = 'FILE', dest = 'goodlumiFiles', nargs = '+', help = 'Input good lumi JSON file.')
    argParser.add_argument('--output', '-o', metavar = 'FILENAME', dest = 'outputFile', help = 'Output file name.')
    argParser.add_argument('--nentries', '-n', metavar = 'N', dest = 'nentries', type = int, default = -1, help = 'Number of entries to process.')
    argParser.add_argument('--hierarchy', '-E', action = 'store_true', dest = 'hierarchy', help = 'Create hierarchical output.')
    argParser.add_argument('--flat', '-t', action = 'store_true', dest = 'flatConfig', help = 'Use "flat python" (no custom imports) analysis macro.')
    
    args = argParser.parse_args()
    sys.argv = []
    
    if args.dataset:
        if args.book == '' or args.dataset == '' or args.fileset == '':
            print 'book, dataset, and fileset must be set together.'
            sys.exit(1)
    
    if args.inputFile and args.dataset:
        print 'Cannot specify file and dataset at the same time.'
        sys.exit(1)

    if flatConfig:
        execfile(sequenceFile)

        import ROOT
        mithep = ROOT.mithep
    else:
        setSequence(args.config)
    
        from MitAna.TreeMod.bambu import mithep, analysis

        # if good run / lumi list is given, prepend the filter module
        if args.goodlumiFiles:
            from MitAna.PhysicsMod.runlumisel import goodLumiFilter
            filterMod = goodLumiFilter(args.goodlumiFiles)
            analysis._sequence = filterMod * analysis._sequence

        analysis.buildSequence()

    # set up input (dataset / fileset or individual file)
    if args.dataset:
        ROOT.gSystem.Load('libMitAnaCatalog.so')
        try:
            catalog = mithep.Catalog(os.environ['MIT_CATALOG'])
        except KeyError:
            print 'Dataset catalog requires environment MIT_CATALOG to be set.'
            sys.exit(1)

        dataset = catalog.FindDataset(args.book, args.dataset, args.fileset, 1)
        analysis.AddDataset(dataset)
        analysis.SetUseCacher(1)
        
    elif args.inputFile:
        analysis.AddFile(args.inputFile)

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
