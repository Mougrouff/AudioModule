import numpy as np
import matplotlib.pyplot as plt

with open("C:\\Users\\a98012636\CLionProjects\\testAudio\\cmake-build-debug\\OutputValues.txt") as f:
    values = f.read().splitlines()

values = [v.split() for v in values]
values = [[float(v[0]), float(v[1])] for v in values]
values = np.array(values).transpose()

print(values)

X = values[0]
Y = values[1]

plt.plot(X,Y)
plt.show()