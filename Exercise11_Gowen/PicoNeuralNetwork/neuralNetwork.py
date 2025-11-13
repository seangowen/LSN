import numpy as np
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras.models import Sequential

class NeuralNetwork():

	def __init__(self, input_layer, hidden_layers, output_layer):

		# Input layer
		self.input_layer = layers.Dense(input_layer[0], activation=input_layer[1])

		# Hidden layers
		self.hidden_layers = []
		for neurons, activations in hidden_layers:
			self.hidden_layers.append(layers.Dense(neurons, activation = activation))

		# Output layer
		self.output_layer = layers.Dense(output_layer[0], activation=output_layer[1])


		# Build the Sequential model
		self.model = Sequential()
		self.model.add(self.input_layer)
		for layer in self.hidden_layers:
			self.model.add(layer)
		self.model.add(self.output_layer)

		print(f"Neural network created: input={input_layer}, hidden={hidden_layers}, output={output_layer}")
		



