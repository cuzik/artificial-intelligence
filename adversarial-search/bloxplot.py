import matplotlib.pyplot as plt
import numpy as np

data = []
data += [59704]
data += [63904]
data += [59704]
data += [63904]
data += [55504]
data += [63904]
data += [59704]
data += [63904]
data += [59704]

data2 = []
data2 += [8743]
data2 += [10506]
data2 += [10953]
data2 += [10497]
data2 += [9067]
data2 += [12721]
data2 += [9471]
data2 += [14822]
data2 += [10142]

data3 = []
for i in range(0,9):
	data3 += [((data[i]-data2[i])/float(data[i]))*100]

plt.figure()
plt.boxplot(data, 0, '')

plt.figure()
plt.boxplot(data2, 0, '')

# plt.figure()
# plt.boxplot(data3, 0, '')

plt.show()