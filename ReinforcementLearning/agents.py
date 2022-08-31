from kaggle_enviroments import make, evaluate

#create the game environment
#set debu=True to see the error if your agents refuses to run

env = make("connectx", debug=True)
print(list(env.agents))
