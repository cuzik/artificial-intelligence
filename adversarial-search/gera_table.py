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
data3 += ['A']
data3 += ['B']
data3 += ['C']
data3 += ['D']
data3 += ['E']
data3 += ['F']
data3 += ['G']
data3 += ['H']
data3 += ['I']

data4 = []
for i in range(0,9):
	data4 += [((data[i]-data2[i])/float(data[i]))*100]


print '\t\t\t\hline'
for i in range(0,9):
	print '\t\t\t\t' + data3[i] + ' ' + ' & ' + str(data[i]) + ' & ' + str(data2[i]) + ' & ' + str("%.2f" % data4[i]) + ' \\\\'
	print '\t\t\t\hline'
print '\t\t\t\hline'
print '\t\t\t\t' + 'Media' + ' ' + ' & ' + str("%.2f" % media(data)) + ' & ' + str("%.2f" % media(data2)) + ' & ' + str("%.2f" % media(data4)) + ' \\\\'
print '\t\t\t\hline'
print '\t\t\t\t' + 'Desvio Padrao' + ' ' + ' & ' + str("%.2f" % desvio_padrao(data)) + ' & ' + str("%.2f" % desvio_padrao(data2)) + ' & ' + str("%.2f" % desvio_padrao(data4)) + ' \\\\'
print '\t\t\t\hline'

