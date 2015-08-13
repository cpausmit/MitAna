#!/usr/bin/env python

import os
import shutil
import re

if __name__ == '__main__':
    from argparse import ArgumentParser
    import sys
    import os
    
    argParser = ArgumentParser(description = 'Run BAMBU analysis')
    argParser.add_argument('config', metavar = 'CONFIG', help = 'Analysis macro defining the module sequence.')
    argParser.add_argument('--book', '-b', metavar = 'BOOK', dest = 'book', default = 't2mit/filefi/042', help = 'Input book, e.g. t2mit/filefi/042.')
    argParser.add_argument('--dataset', '-d', metavar = 'DATASET', dest = 'dataset', help = 'Input dataset.')
    argParser.add_argument('--fileset', '-s', metavar = 'FILESET', dest = 'fileset', default = '0000', help = 'Input fileset.')
    argParser.add_argument('--file', '-f', metavar = 'INPUT', dest = 'inputFiles', nargs = '*', help = 'Input file name when running on a single file.')
    argParser.add_argument('--goodlumi', '-j', metavar = 'FILE', dest = 'goodlumiFile', default = '', help = 'Input good lumi JSON file.')
    argParser.add_argument('--custom', '-m', metavar = 'EXPR', dest = 'custom', nargs = '+', help = 'Custom variables passed to the analysis object. EXPR is any number of space-separated expressions of form key=value.')
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

    from MitAna.TreeMod.bambu import analysis, mithep

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

        analysis.book = args.book
        analysis.dataset = args.dataset
        
    elif args.inputFiles:
        for f in args.inputFiles:
            analysis.AddFile(f)

    # set up output
    if args.outputFile:
        analysis.SetOutputName(args.outputFile)

    # set up number of entries to process
    if args.nentries >= 0:
        analysis.SetProcessNEvents(args.nentries)

    # set up output (histogram file) format
    analysis.SetKeepHierarchy(args.hierarchy)

    # set up data / MC
    if args.realData or args.goodlumiFile:
        analysis.isRealData = True

    # add custom variables
    if args.custom is not None:
        for expr in args.custom:
            key, eq, value = expr.partition('=')
            if not value:
                continue
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

            print 'Adding a custom variable', key, '=', value
            analysis.custom[key] = value

    # load macro
    execfile(args.config)

    # if good run / lumi list is given, prepend the filter module
    if args.goodlumiFile:
        from MitAna.PhysicsMod.runlumisel import goodLumiFilter
        filterMod = goodLumiFilter(args.goodlumiFile)
        analysis._sequence = filterMod * analysis._sequence

    # build the module sequence
    analysis.buildSequence()

    print '\n+++++ ANALYSIS FLOW +++++\n'
    analysis.PrintModuleTree()
    print '\n+++++++++++++++++++++++++\n'
    
    analysis.Run(False)
