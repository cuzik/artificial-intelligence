arquivo = open('data', 'r')
data = []

for linha in arquivo:
	linha = linha[:-1]
	data += [linha.split(" ")]

arquivo.close()

for i in range(0,len(data)):
	for j in range(0,len(data[i])):
		data[i][j] = int(data[i][j])

media_c = []

count = 0
for i in range(0,10):
	count += data[i][1]
media_c += [count/10.0]

count = 0
for i in range(10,20):
	count += data[i][1]
media_c += [count/10.0]

count = 0
for i in range(20,30):
	count += data[i][1]
media_c += [count/10.0]

count = 0
for i in range(30,40):
	count += data[i][1]
media_c += [count/10.0]




media_p = []

count = 0
for i in range(0,10):
	count += data[i][0]
media_p += [count/10.0]

count = 0
for i in range(10,20):
	count += data[i][0]
media_p += [count/10.0]

count = 0
for i in range(20,30):
	count += data[i][0]
media_p += [count/10.0]

count = 0
for i in range(30,40):
	count += data[i][0]
media_p += [count/10.0]





media_x = []

count = 0
for i in range(0,10):
	count += data[i][2]
media_x += [count/10.0]

count = 0
for i in range(10,20):
	count += data[i][2]
media_x += [count/10.0]

count = 0
for i in range(20,30):
	count += data[i][2]
media_x += [count/10.0]

count = 0
for i in range(30,40):
	count += data[i][2]
media_x += [count/10.0]




metodo = []
metodo += ["BFS"]
metodo += ["Djikstra"]
metodo += ["A*"]
metodo += ["DFS"]



print("\t\\begin{table}[h!]")
print("\t\t\\centering")
print("\t\t\\begin{tabular}{| c | c | c | c |}")
print("\t\t\t\\hline")
print("\t\t\t\tMétodo & Passos Médio & Custo Médio & Nós Expandidos Médios" + "\\\\")
print("\t\t\t\\hline")
for i in range(0,4):
	print("\t\t\t\t" + metodo[i] + " & " + str(media_p[i]) + " & " + str(media_c[i]) + " & " + str(media_x[i]) + "\\\\")
	print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Dados Estatísticos Obtidos" + "}")
print("\t\t\\label{table:md}")
print("\t\\end{table}")
print("\n\n\n")