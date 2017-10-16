import sys
from subprocess import call
nodes = []
nodes += [[[35, 5],[25,40]]]
nodes += [[[20,20],[41,17]]]
nodes += [[[10,10],[30,32]]]
nodes += [[[ 1, 1],[23,35]]]
nodes += [[[18, 7],[38,25]]]
nodes += [[[ 9, 4],[ 9,35]]]
nodes += [[[40, 5],[ 2,35]]]
nodes += [[[15,15],[40,40]]]
nodes += [[[ 1, 2],[42,42]]]
nodes += [[[ 1,20],[20,42]]]

for i in ["1","2","3","4"]:
	for node in nodes:
		# print "./hocus_pocus " + node[0][0] + " " + node[0][1] + " " + node[1][0] + " " + node[1][1] + " " + i + " 7 10"
		call("./hocus_pocus " + str(node[0][0] - 1) + " " + str(node[0][1] - 1) + " " + str(node[1][0] - 1) + " " + str(node[1][1] - 1) + " " + i + " 7 10", shell = True)