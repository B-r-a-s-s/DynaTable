import numpy as np
import math
# import matplotlib.pyplot as plt

a = 0.025 # m
alpha = 75 # deg
beta = 75 # deg

T = 0.34 # torque per motor in N*m

Tkg = 3.5 # power per motor in kg*cm
T = Tkg * 0.0981 # conversion factor

nm = 4 # number of motors

# Worst case, fully flat

alpha = 28 # deg
beta = 15 # degrees

# alpha = 23 # deg
# beta = 6 # degrees

alpha = math.radians(alpha)
beta = math.radians(beta)

Fy = T * np.sin(beta) / (a * np.cos(0.5*np.pi - alpha - beta))

print('Vertical power, assuming two motors:',Fy*nm,'N')
print('Vertical power, assuming two motors:',Fy*nm/9.81,'kg')
print('Vertical power, assuming two motors:',(Fy*nm-10)/9.81,'kg, minus 10 N')