import pandas as pd
iowa_file_path = '../input/home-data-for-ml-course/train.csv'
home_data = pd.read_csv(iowa_file_path)
from learntools.core import binder
binder.bind(globals())
from learntools.machine_learning.ex3 import *
print("Setup Complete")

