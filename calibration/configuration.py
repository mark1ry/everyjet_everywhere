import json

class settings_check:
    def __init__(self):
        self.read_root: bool = False
        self.read_pickle: bool = False
        self.output_pickle: bool = False
        self.produce_plots: bool = False
        self.even_root_path: str = ""
        self.odd_root_path: str = ""
        self.even_pkl_path: str = ""
        self.odd_pkl_path: str = ""
        self.even_tree_name: str = ""
        self.odd_tree_name: str = ""
        self.benhA_pkl: str = ""
        self.benhB_pkl: str = ""
        self.clenhA_pkl: str = ""
        self.clenhB_pkl: str = ""
        self.plots_path: str = ""
        self.plot_extra_info: str = ""
        self.log_scale: bool = True

    def update_settings(self, path):
        config = json.load(path)
        for key, value in config.items():
            if key in self.__dict__:
                setattr(self, key, type(self.__dict__[key])(value))
            else:
                print(f"WARNING: The key {key} was not expected in the settings. It will be ignored")

        if not self.check_validity():
            self.reset_settings()
            print("WARNING: The settings given were incompatible. Please check that all the required variables have been properly given")

    def reset_settings(self):
        self.__init__()

    def check_validity(self):
        if self.read_root == self.read_pickle: # Data should be read from either the pickle or root
            return False
        if self.read_root:
            if self.even_root_path=="" or self.odd_root_path=="" or self.even_pkl_path=="" or self.odd_pkl_path=="" or self.even_tree_name=="" or self.odd_tree_name=="":
                return False  # If the root is to be read we need all those paths to retrieve the data
        if self.read_pickle or self.output_pickle:
            if self.benhA_pkl=="" or self.benhB_pkl=="" or self.clenhA_pkl=="" or self.clenhB_pkl=="":
                return False # If need to input/output pickle files these paths are required
        if self.produce_plots:
            if self.plots_path=="": # Require the output path of the plots
                return False

        return True

class settings_split:
    def __init__(self):
        self.even_root_path: str = ""
        self.odd_root_path: str = ""
        self.even_pkl_path: str = ""
        self.odd_pkl_path: str = ""
        self.even_tree_name: str = ""
        self.odd_tree_name: str = ""
        self.output_root_path: str = ""

    def update_settings(self, path):
        config = json.load(path)
        for key, value in config.items():
            if key in self.__dict__:
                setattr(self, key, type(self.__dict__[key])(value))
            else:
                print(f"WARNING: The key {key} was not expected in the settings. It will be ignored")

        if not self.check_validity():
            self.reset_settings()
            print("WARNING: The settings given were incompatible. Please check that all the required variables have been properly given")

    def reset_settings(self):
        self.__init__()

    def check_validity(self):
        if self.even_root_path=="" or self.even_pkl_path=="" or self.even_tree_name=="" or self.output_root_path=="": # Data should be read from either the pickle or root
            return False
        elif self.odd_pkl_path=="" or self.odd_root_path=="" or self.odd_tree_name=="":
            print("WARNING: Only one dataset will be used (Not suitable for even/odd split data)")
            return True

        return True
