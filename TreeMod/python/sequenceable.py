# "Sequenceable" classes to realize the job (module sequence) configuration syntax
# using * and + signs.

class Sequenceable(object):
    """
    Base class for objects that can be in a sequence or be a sequence itself.
    Three classes derive from Sequenceable: Node, Chain (list of
    consecutive modules), and Bundle (list of parallel Chains). At build(),
    returns a pair (list of chain heads, list of chain tails). The lists are
    single element for Module and Chain.
    """

    def __init__(self):
        pass

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

    def build(self, modlist = []):
        return ([], [])


class Node(Sequenceable):
    """
    PyROOT wrapper for mithep TAModules
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
        self._core = obj
        self._name = name

    def __iter__(self):
        return Node.Iterator(self)

    def build(self, modlist = []):
        if self._core in modlist:
            print 'Module ' + self._name + ' used multiple times in the analysis sequence'
            sys.exit(1)

        modlist.append(self._core)
        return ([self._core], [self._core])


class Chain(Sequenceable):
    """
    Linear sequence of sequenceables
    """

    class Iterator(object):
        def __init__(self, chain):
            self._nodes = chain._nodes
            self._idx = -1
            self._itr = None
            
        def next(self):
            if self._itr:
                try:
                    return self._itr.next()
                except StopIteration:
                    pass

            self._idx += 1

            if self._idx == len(self._nodes):
                raise StopIteration

            self._itr = iter(self._nodes[self._idx])
            return self.next()

    
    def __init__(self, nodes):
        self._nodes = list(nodes)

    def __iter__(self):
        return Chain.Iterator(self)

    def build(self, modlist = []):
        head = []
        tail = []
        for node in self._nodes:
            hm, tm = node.build(modlist)
            if len(tail) != 0:
                for mod in hm:
                    tail[-1].Add(mod)

            tail = list(tm)

            if len(head) == 0:
                head = list(hm)

        return (head, tail)


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
        self._chains = list(chains)

    def __mul__(self, next):
        print 'Cannot make a module dependent on two parallel chains'
        sys.exit(1)

    def __iter__(self):
        return Bundle.Iterator(self)

    def build(self, modlist = []):
        head = []
        tail = []
        for chain in self._chains:
            hm, tm = chain.build(modlist)
            head += hm
            tail += tm

        return (head, tail)
