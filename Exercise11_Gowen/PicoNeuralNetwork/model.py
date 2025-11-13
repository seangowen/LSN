from tensorflow import keras
from tensorflow.keras import optimizers

class Model(keras.Model):

	def __init__(self, neural_network, optimizer, loss_function):

		super().__init__()

		self.neural_network = neural_network.model
		self.optimizer = optimizer
		self.loss_function = loss_function

		print("Model has been instanciated")


	def train(self, X_train, Y_train, X_test, Y_test, num_epochs, batch_size):

		self.neural_network.compile(
			optimizer = self.optimizer,
			loss = self.loss_function,
			metrics=["accuracy"]
			)
		history = self.neural_network.fit(
			x = X_train,
			y = Y_train,
			batch_size = batch_size,
			epochs = num_epochs,
			validation_data=(X_test, Y_test)
			)

		return history


	def test(self, X_test, Y_test):

		results = self.neural_network.evaluate(X_test, Y_test, verbose=1)

		# Ensure results is always a list
		if not isinstance(results, (list, tuple)):
			results = [results]

		return results
		

	
