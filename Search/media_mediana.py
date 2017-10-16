arquivo = open('data', 'r')
data = []

for linha in arquivo:
	linha = linha[:-1]
	data += [linha.split(" ")]

arquivo.close()

for i in range(0,len(data)):
	for j in range(0,len(data[i])):
		data[i][j] = int(data[i][j])

media = []
count = 0

for i in range(0,10):
	count += data[i][1]
media += [count/10.0]

for i in range(10,20):
	count += data[i][1]
media += [count/10.0]

for i in range(20,30):
	count += data[i][1]
media += [count/10.0]

print media