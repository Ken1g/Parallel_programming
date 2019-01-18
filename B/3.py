import matplotlib.pyplot as plt

f = open('out')
N = []
time = []

for line in f:
	N.append(int(line.split()[2][:-1]))
	time.append(int(line.split()[5]))

plt.plot(N, time)
plt.show()


