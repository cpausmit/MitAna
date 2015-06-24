def setSequence(sequenceFile):
    execfile(sequenceFile)


if __name__ == '__main__':
    from argparse import ArgumentParser
    import sys
    import os
    
    argParser = ArgumentParser(description = 'Run BAMBU analysis')
    argParser.add_argument('config', metavar = 'CONFIG')
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = 't2mit/filefi/040')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset')
    argParser.add_argument('--fileset', '-s', metavar = 'FILESET', dest = 'fileset')
    argParser.add_argument('--file', '-f', metavar = 'INPUT', dest = 'inputFile')
    argParser.add_argument('--output', '-o', metavar = 'FILENAME', dest = 'outputFile')
    argParser.add_argument('--nentries', '-n', metavar = 'N', dest = 'nentries', type = int, default = 1)
    argParser.add_argument('--cacher', '-c', action = 'store_true', dest = 'useCacher')
    argParser.add_argument('--no-hierarchy', '-H', action = 'store_true', dest = 'anarchy')
    argParser.add_argument('--libs', '-L', metavar = 'LIST', dest = 'libListName')
    
    args = argParser.parse_args()
    sys.argv = []
    
    if args.dataset or args.fileset:
        if args.book == '' or args.dataset == '' or args.fileset == '':
            raise RuntimeError('book, dataset, and fileset must be set together.')
    
    if args.inputFile and (args.dataset or args.fileset):
        raise RuntimeError('cannot specify file and dataset/fileset at the same time.')
    
    setSequence(args.config)

    from MitAna.TreeMod.bambu import mithep, analysis
    
    if args.dataset:
        catalog = mithep.Catalog('catalog')
        dataset = catalog.FindDataset(args.book, args.dataset, args.fileset, 1)
        analysis.AddDataset(dataset)
        analysis.SetUseCacher(1)
    
    elif args.inputFile:
        analysis.AddFile(args.inputFile)
    
    if args.outputFile:
        analysis.SetOutputName(args.outputFile)
    
    if args.nentries > 0:
        analysis.SetProcessNEvents(args.nentries)
    
    if args.useCacher:
        analysis.SetUseCacher(1)
    
    if args.anarchy:
        analysis.SetKeepHierarchy(False)
    
    print '\n+++++ ANALYSIS FLOW +++++\n'
    analysis.PrintModuleTree()
    print '\n+++++++++++++++++++++++++\n'
    
    analysis.Run(False)
