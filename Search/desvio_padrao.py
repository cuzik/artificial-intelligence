import math
def somar(valores):
	soma = 0
	for v in valores:
		soma += v
	return soma

def media(valores):
	soma = somar(valores)
	qtd_elementos = len(valores)
	media = soma / float(qtd_elementos)
	return media

def variancia(valores):
	_media = media(valores)
	soma = 0
	_variancia = 0

	for valor in valores:
		soma += math.pow( (valor - _media), 2)
	_variancia = soma / float( len(valores) )
	return _variancia

def desvio_padrao(valores):
	return math.sqrt( variancia(valores) )







arquivo = open('data', 'r')
data = []

for linha in arquivo:
	linha = linha[:-1]
	data += [linha.split(" ")]

arquivo.close()

for i in range(0,len(data)):
	for j in range(0,len(data[i])):
		data[i][j] = int(data[i][j])


dp_c = []
aux = []
for i in range(0,10):
	aux += [data[i][1]]
dp_c += [desvio_padrao(aux)]

aux = []
for i in range(10,20):
	aux += [data[i][1]]
dp_c += [desvio_padrao(aux)]

aux = []
for i in range(20,30):
	aux += [data[i][1]]
dp_c += [desvio_padrao(aux)]

aux = []
for i in range(30,40):
	aux += [data[i][1]]
dp_c += [desvio_padrao(aux)]





dp_p = []
aux = []
for i in range(0,10):
	aux += [data[i][0]]
dp_p += [desvio_padrao(aux)]

aux = []
for i in range(10,20):
	aux += [data[i][0]]
dp_p += [desvio_padrao(aux)]

aux = []
for i in range(20,30):
	aux += [data[i][0]]
dp_p += [desvio_padrao(aux)]

aux = []
for i in range(30,40):
	aux += [data[i][0]]
dp_p += [desvio_padrao(aux)]





dp_x = []
aux = []
for i in range(0,10):
	aux += [data[i][2]]
dp_x += [desvio_padrao(aux)]

aux = []
for i in range(10,20):
	aux += [data[i][2]]
dp_x += [desvio_padrao(aux)]

aux = []
for i in range(20,30):
	aux += [data[i][2]]
dp_x += [desvio_padrao(aux)]

aux = []
for i in range(30,40):
	aux += [data[i][2]]
dp_x += [desvio_padrao(aux)]





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
	print("\t\t\t\t" + metodo[i] + " & " + str(dp_p[i]) + " & " + str(dp_c[i]) + " & " + str(dp_x[i]) + "\\\\")
	print("\t\t\t\\hline")
print("\t\t\\end{tabular}")
print("\t\t\\caption" + "{" + "Dados Estatísticos Obtidos" + "}")
print("\t\t\\label{table:dp}")
print("\t\\end{table}")
print("\n\n\n")