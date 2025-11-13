import numpy as np


class DataProcessor:

	def __init__(self):
		
		# Initialize instance variables
		self.a, self.b, self.noise = (None, None, None)

		self.X_train = None
		self.Y_train = None


	def generateToyData(self, sample_size, a, b, noise_factor, seed=None):

		if seed is not None:
			np.random.seed(seed)

		self.a, self.b = (a, b)
		self.noise = noise_factor * np.random.randn(sample_size)  # Gaussian noise

		self.X_train = np.random.uniform(-1,1,sample_size) # 100 points uniformly randomly distributed between -1 and 1
		self.Y_train = self.a * self.X_train + self.b + self.noise

	def returnData(self):

		self.X_train = self.X_train.reshape(-1,1)
		self.Y_train = self.Y_train.reshape(-1,1)

		return self.X_train, self.Y_train