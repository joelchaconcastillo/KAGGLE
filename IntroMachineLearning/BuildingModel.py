##1) Define: What ype of model will it be?
##2) Fit: Capture patterns from the provided data.
##3) Predict: Call function
##4) Evaluatie: Determine how accurate the model's predictions are.
from sklearn.tree import DecisionTreeRegressor
#Define model. Specify a number for random_state to ensure same results from each run.
melbourne_model = DecisionTreeRegressor(random_state=1) ##Random state=1 get the same result in each run
#melbourne_mode.columns
#Fit hte model
melbourne_model.fit(X, y)
print("Making predictions for the following 5 houses:")
print(X.head())
print("The predicitons are")
print(melbourne_model.predict(X, head()))
