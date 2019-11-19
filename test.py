import numpy as np
import math
# import matplotlib.pyplot as plt

a = 0.025 # m
alpha = 54 # deg
beta = 39 # degrees
T = 0.11 # Nm

alpha = math.radians(alpha)
beta = math.radians(beta)

Fy = a * T * np.sin(beta) / np.cos(0.5*np.pi - alpha - beta)

print('Vertical power, assuming two motors:',Fy*2,'N')