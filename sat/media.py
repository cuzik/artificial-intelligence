import pandas as pd

data_1 = pd.Series([0,0,0,0,0,0,0,0,0,0])
data_2 = pd.Series([2,2,0,2,2,2,0,3,1,2])
data_3 = pd.Series([1,1,2,2,1,1,3,2,1,5])

datars_1 = pd.Series([0,0,0,0,0,0,0,0,0,0])
datars_2 = pd.Series([28, 27, 25, 27, 26, 28, 29, 26, 26, 26])
datars_3 = pd.Series([90, 92, 83, 90, 85, 91, 84, 90, 88, 87])

print '\\begin{table}[h!]'
print '\t\\centering'
print '\t\\begin{tabular}{| c || c | c |}'
print '\t\t\\hline'
print '\t\t\tCaso & RS & SA\\\\'
print '\t\t\\hline'
print '\t\t\tuf20-91-01 & '    + str(data_1.mean()) + ' +/- ' + str(data_1.std()) + ' & ' + str(datars_1.mean()) + ' +/- ' + str(datars_1.std()) + '\\\\'
print '\t\t\\hline'
print '\t\t\tuf100-430-01 & '  + str(data_2.mean()) + ' +/- ' + str(data_1.std()) + ' & ' + str(datars_2.mean()) + ' +/- ' + str(datars_2.std()) + '\\\\'
print '\t\t\\hline'
print '\t\t\tuf250-1065-01 & ' + str(data_3.mean()) + ' +/- ' + str(data_1.std()) + ' & ' + str(datars_3.mean()) + ' +/- ' + str(datars_3.std()) + '\\\\'
print '\t\t\\hline'
print '\t\\end{tabular}'
print '\t\\caption{media e desviu padrao dos resultados dos algoritmos}'
print '\t\\label{media}'
print '\\end{table}'
