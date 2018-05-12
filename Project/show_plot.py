from numpy import genfromtxt
import matplotlib.pyplot as plt

per_data=genfromtxt('data.csv',delimiter=',')
plt.xlabel('Generation')
plt.ylabel('Fitness')
plt.plot(per_data)
plt.show()
