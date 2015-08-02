# "Sequenceable" classes to realize the job (module sequence) configuration syntax
# using * and + signs.

import sys

class Sequenceable(object):
    """
    Base class for objects that can be in a sequence or be a sequence itself.
    Three classes derive from Sequenceable: Node, Chain (list of
    consecutive modules), and Bundle (list of parallel Chains). At build(),
    returns a pair (list of chain heads, list of chain tails). The lists are
    single element for Node and Chain.
    """

    def __init__(self):
        self.isBuilt = False
        self.headNodes = []
        self.tailNodes = []

    def __add__(self, next):
        """
        Overrides operator +
        """
        return Bundle([self, next])

    def __mul__(self, next):
        """
        Overrides operator *
        """
        return Chain([self, next])

    def build(self, nodelist = []):
        if self.isBuilt:
            print 'Cannot build a sequence twice.'
            sys.exit(1)

        self.isBuilt = True

    def unbuild(self):
        self.isBuilt = False


class Node(Sequenceable):
    """
    Node with links to next nodes (constructed at Chain.build())
    """

    class Iterator(object):
        def __init__(self, node):
            self._node = node
            self._iterated = False

        def next(self):
            if self._iterated:
                raise StopIteration

            self._iterated = True
            return self._node


    def __init__(self, obj, name):
        Sequenceable.__init__(self)
        self._core = obj
        self._name = name
        self._nextNodes = None
        self.headNodes = [self]
        self.tailNodes = [self]

    def __iter__(self):
        return Node.Iterator(self)

    def build(self, nodelist = []):
        if self.isBuilt:
            print 'Cannot build a sequence twice.'
            sys.exit(1)

        if self._core in nodelist:
            print 'Module ' + self._name + ' used multiple times in the analysis sequence'
            sys.exit(1)

        self._nextNodes = []

        nodelist.append(self._core)

        self.isBuilt = True

    def unbuild(self):
        self._nextNodes = None
        self.isBuilt = False

    def connect(self, nextNode):
        self._nextNodes.append(nextNode)


class Chain(Sequenceable):
    """
    Linear sequence of sequenceables
    """

    class Iterator(object):
        def __init__(self, chain):
            self._elems = chain._elems
            self._idx = -1
            self._itr = None
            
        def next(self):
            if self._itr:
                try:
                    return self._itr.next()
                except StopIteration:
                    pass

            self._idx += 1

            if self._idx == len(self._elems):
                raise StopIteration

            self._itr = iter(self._elems[self._idx])
            return self.next()

    
    def __init__(self, elems):
        Sequenceable.__init__(self)
        self._elems = list(elems)

    def __iter__(self):
        return Chain.Iterator(self)

    def build(self, nodelist = []):
        if self.isBuilt:
            print 'Cannot build a sequence twice.'
            sys.exit(1)

        # elem here can be a Node, Chain, or Bundle
        tail = None
        for elem in self._elems:
            elem.build(nodelist)
            if tail:
                for node in elem.headNodes:
                    tail.connect(node)

            # There should be only one tail node for each
            # element unless it is the last one. Taking the
            # last here but can as well use elem.tailNodes[0]
            tail = elem.tailNodes[-1]

            if len(self.headNodes) == 0:
                self.headNodes = list(elem.headNodes)

        self.tailNodes = [tail]

        self.isBuilt = True

    def unbuild(self):
        for elem in self._elems:
            elem.unbuild()

        self.headNodes = []
        self.tailNodes = []
        self.isBuilt = False


class Bundle(Sequenceable):
    """
    List of parallel chains
    """

    class Iterator(object):
        def __init__(self, bundle):
            self._chains = bundle._chains
            self._idx = -1
            self._itr = None

        def next(self):
            if self._itr:
                try:
                    return self._itr.next()
                except StopIteration:
                    pass

            self._idx += 1

            if self._idx == len(self._chains):
                raise StopIteration

            self._itr = iter(self._chains[self._idx])
            return self.next()


    def __init__(self, chains):
        Sequenceable.__init__(self)
        self._chains = list(chains)

    def __mul__(self, next):
        print 'Cannot make a module dependent on two parallel chains'
        sys.exit(1)

    def __iter__(self):
        return Bundle.Iterator(self)

    def build(self, nodelist = []):
        if self.isBuilt:
            print 'Cannot build a sequence twice.'
            sys.exit(1)

        for chain in self._chains:
            chain.build(nodelist)
            self.headNodes += chain.headNodes
            self.tailNodes += chain.tailNodes

        self.isBuilt = True

    def unbuild(self):
        for chain in self._chains:
            chain.unbuild()
            
        self.headNodes = []
        self.tailNodes = []
        self.isBuilt = False
