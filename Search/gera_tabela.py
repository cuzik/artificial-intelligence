arquivo = open('data', 'r')

data = []
for linha in arquivo:
	linha = linha[:-1]
	data += [linha.split(" ")]
arquivo.close()


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

print("\t\\begin{table}[h!]")
print("\t\t\\centering")
print("\t\t\\begin{tabular}{| c | c || c | c | c |}")
print("\t\t\t\\hline")
print("\t\t\t\tPonto Inicio & Ponto Fim & Distancia & Custo & Nos Expandidos"  + "\\\\")
print("\t\t\t\\hline")
for i in range(0,10):
		print("\t\t\t\t(" + str(nodes[i][0][0]) + "," + str(nodes[i][0][1]) + ") & (" + str(nodes[i][1][0]) + "," + str(nodes[i][1][1]) + ") & " + data[i][0] + " & " + data[i][1] + " & " + data[i][2] + "\\\\")
		print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Resultados dos teste com o BFS" + "}")
print("\t\t\\label{table:1}")
print("\t\\end{table}")
print("\n\n\n")

print("\t\\begin{table}[h!]")
print("\t\t\\centering")
print("\t\t\\begin{tabular}{| c | c || c | c | c |}")
print("\t\t\t\\hline")
print("\t\t\t\tPonto Inicio & Ponto Fim & Distancia & Custo & Nos Expandidos"  + "\\\\")
print("\t\t\t\\hline")
for i in range(0,10):
		print("\t\t\t\t(" + str(nodes[i][0][0]) + "," + str(nodes[i][0][1]) + ") & (" + str(nodes[i][1][0]) + "," + str(nodes[i][1][1]) + ") & " + data[i+10][0] + " & " + data[i+10][1] + " & " + data[i+10][2] + "\\\\")
		print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Resultados dos teste com o Djsikstra" + "}")
print("\t\t\\label{table:2}")
print("\t\\end{table}")
print("\n\n\n")

print("\t\\begin{table}[h!]")
print("\t\t\\centering")
print("\t\t\\begin{tabular}{| c | c || c | c | c |}")
print("\t\t\t\\hline")
print("\t\t\t\tPonto Inicio & Ponto Fim & Distancia & Custo & Nos Expandidos"  + "\\\\")
print("\t\t\t\\hline")
for i in range(0,10):
		print("\t\t\t\t(" + str(nodes[i][0][0]) + "," + str(nodes[i][0][1]) + ") & (" + str(nodes[i][1][0]) + "," + str(nodes[i][1][1]) + ") & " + data[i+20][0] + " & " + data[i+20][1] + " & " + data[i+20][2] + "\\\\")
		print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Resultados dos teste com o A*" + "}")
print("\t\t\\label{table:3}")
print("\t\\end{table}")
print("\n\n\n")

print("\t\\begin{table}[h!]")
print("\t\t\\centering")
print("\t\t\\begin{tabular}{| c | c || c | c | c |}")
print("\t\t\t\\hline")
print("\t\t\t\tPonto Inicio & Ponto Fim & Distancia & Custo & Nos Expandidos"  + "\\\\")
print("\t\t\t\\hline")
for i in range(0,10):
		print("\t\t\t\t(" + str(nodes[i][0][0]) + "," + str(nodes[i][0][1]) + ") & (" + str(nodes[i][1][0]) + "," + str(nodes[i][1][1]) + ") & " + data[i+30][0] + " & " + data[i+30][1] + " & " + data[i+30][2] + "\\\\")
		print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Resultados dos teste com o DFS limitado iterativo" + "}")
print("\t\t\\label{table:4}")
print("\t\\end{table}")
print("\n\n\n")