import pandas as pd

# Path of the file to read
iowa_file_path = '../input/home-data-for-ml-course/train.csv'

# Fill in the line below to read the file into a variable home_data
home_data = pd.read_csv('../input/home-data-for-ml-course/train.csv')

# Print summary statistics in next line
home_data.describe()
home_data = home_data.dropna(axis=0)



print(home_data.describe()["Id"]["mean"])
print(home_data.columns)



home_data_features= ['Rooms', 'Bathroom', 'Landsize', 'Lattitude', 'Longtitude']

X = home_data[home_data_features]
X.describe()
X.head()
