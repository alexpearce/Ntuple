import ROOT
import numpy as np

class Ntuple(ROOT.TChain):
    """Wrapper class for TChain."""
    # ROOT TBranch types corresponding the numpy types
    types_map = {
        # int-like
        "B": "int8",
        "b": "uint8",
        "S": "int16",
        "s": "uint16",
        "I": "int32",
        "i": "uint32",
        "L": "int64",
        "l": "uint64",
        "O": "bool",
        # float-like
        "F": "float32",
        "D": "float64"
    }
    def __init__(self, name):
        """Initialise an Ntuple object.

        Keyword arguments:
        name -- Name of the TTree this object represents
        """
        super(Ntuple, self).__init__(name)
        # Dictionary of branch names to array pointers
        # Shorthand access to this is provided by the val method
        self.vars = {}

    def add(self, path):
        """Add a TTree to the chain. Superseeds TChain.Add."""
        ret = self.Add(path)
        self.setup_branches()
        return ret

    def val(self, var, reference=False):
        """Return the value of var for the current entry.

        A KeyError is thrown if no branch called `var` is found.
        Keyword arguments:
        var -- String of the variable value to be retrieved
        reference -- If True, the array pointer is returned, else the value
        of the first array element (i.e. the value itself)
        """
        if var not in self.vars:
            raise KeyError('Branch name `{0}` not in Ntuple `{1}`'.format(
                var, self.GetName()
            ))
        return self.vars[var] if reference else self.vars[var][0]

    def setup_branches(self):
        """Populate the vars dict with appropriate-type numpy arrays."""
        # This is subtle. GetListOfBranches returns a TObjArray pointer,
        # and so modifying this, by appending friend branches for instance,
        # changes the original TTree instance. The friend tree concept is
        # supposed to prevent this. So, copy the list and modify that.
        branches = list(self.GetListOfBranches())
        for branch in branches:
            name = branch.GetName()
            title = branch.GetTitle()
            # Don't bind to branches containing arrays
            if title.find("[") >= 0: continue
            btype = title.split("/")[-1]
            dtype = Ntuple.types_map[btype]
            # Use the key value if it exists, else create one
            try:
                z = self.vars[name]
            except KeyError:
                z = np.zeros(1, dtype=dtype)
                self.vars[name] = z
            self.SetBranchAddress(name, z)
