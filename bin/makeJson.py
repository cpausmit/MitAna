#!/usr/bin/env python

#-------------------------------------------------------------------------------
# makeJson.py
#
# This script is used to make a lumi list JSON file out of the output from
# the RunLumiListMod.
# Usage: makeJson.py <ROOT file> [<ROOT file2> ...]
# Wildcard * is allowed for the ROOT file path specification.
#
#-------------------------------------------------------------------------------

import sys
import array
from argparse import ArgumentParser
import ROOT
ROOT.gROOT.SetBatch(True)

argParser = ArgumentParser(description = 'Make JSON lumi list')
argParser.add_argument('paths', metavar = 'PATH', nargs = '+', help = 'Paths to ROOT files (wildcard allowed) containing lumi list trees.')
argParser.add_argument('--tree', '-t', metavar = 'TREE', dest = 'treeName', default = 'RunLumiList', help = 'Input tree name.')
argParser.add_argument('--branches', '-b', metavar = 'RUN,LUMI', dest = 'branchNames', default = 'run,lumi', help = 'Input branch names in form {run},{lumi}.')
argParser.add_argument('--list', '-i', metavar = 'FILE', dest = 'listFile', default = '', help = 'File that contains the list of input files.')
argParser.add_argument('--out', '-o', metavar = 'FILE', dest = 'outputFile', default = 'lumis.txt', help = 'Output file name.')
argParser.add_argument('--mask', '-m', metavar = 'FILE', dest = 'mask', default = '', help = 'Lumi mask (e.g. Golden JSON) to apply.')

args = argParser.parse_args()
sys.argv = []

if args.mask:
    try:
        with open(args.mask) as maskFile:
            maskJSON = eval(maskFile.read())

        mask = {}
        for runStr, lumiRanges in maskJSON.items():
            run = int(runStr)
            mask[run] = []
            for begin, end in lumiRanges:
                mask[run] += range(begin, end + 1)
    except:
        print 'Could not parse mask JSON', args.mask
        sys.exit(1)

runName, lumiName = args.branchNames.split(',')

source = ROOT.TChain(args.treeName)
if args.listFile:
    with open(args.listFile) as listFile:
        for line in listFile:
            if line.strip().startswith('#'):
                continue

            source.Add(line.strip())
else:
    for path in args.paths:
        source.Add(path)

source.LoadTree(0)

runLeaf = source.GetLeaf(runName)
lumiLeaf = source.GetLeaf(lumiName)
if runLeaf.IsA().GetName() != 'TLeafI' or lumiLeaf.IsA().GetName() != 'TLeafI':
    print 'Branches are not integral'
    sys.exit(1)

if runLeaf.IsUnsigned():
    runPtr = array.array('I', [0])
else:
    runPtr = array.array('i', [0])

if lumiLeaf.IsUnsigned():
    lumiPtr = array.array('I', [0])
else:
    lumiPtr = array.array('i', [0])

source.SetBranchAddress(runName, runPtr)
source.SetBranchAddress(lumiName, lumiPtr)

allLumis = {}

iEntry = 0
while source.GetEntry(iEntry) > 0:
    iEntry += 1

    run = runPtr[0]
    lumi = lumiPtr[0]

    if len(mask):
        if run not in mask or lumi not in mask[run]:
            continue

    if run not in allLumis:
        allLumis[run] = []
        
    lumis = allLumis[run]

    if lumi not in lumis:
        lumis.append(lumi)

runBlocks = {}
for run in sorted(allLumis.keys()):
    ranges = []
    start = 0
    current = -1
    for lumi in sorted(allLumis[run]):
        if lumi == current + 1:
            current = lumi
            continue

        if current > 0:
            ranges.append((start, current))

        current = lumi
        start = current

    if start != 0:
        ranges.append((start, current))

    runBlocks[run] = ',\n'.join(['    [%d, %d]' % r for r in ranges])

with open(args.outputFile, 'w') as json:
    json.write('{\n')
    json.write(',\n'.join(['  "%d": [\n%s\n]' % (run, runBlocks[run]) for run in sorted(allLumis.keys())]))
    json.write('\n}')
