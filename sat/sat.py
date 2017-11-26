from random import *

files_test = [6,20,100,250]

def read_file(input):
  input = str(input) + ".cnf"
  file = open(input)
  data = []
  for i in file:
    a = i.split(" ")
    a = a[:-1]
    data += [a]
  return data

def rand_True_or_False():
  return randint(0,2) == 1

def hash_bin(number):
  res = {}
  for i in range(1,number+1):
    res.update({i : rand_True_or_False()})
  return res

def attr_value(id, value):
  if id < 0:
    return not value
  return value

def mirror_bin(data,values):
  mirror = []
  for i in data:
    mirror_aux = []
    for j in i:
      mirror_aux += [attr_value(int(j), values[abs(int(j))])]
    mirror += [mirror_aux]
  return mirror

def point_clausula(clausula):
  if clausula[0] or clausula[1] or clausula[2]:
    return 1
  return 0

def fitness(mirror):
  fit = 0
  for i in mirror:
    fit += point_clausula(i)
  return fit

def sat():
  data_ori = read_file(files_test[3])
  values   = hash_bin(files_test[3])
  mirror = mirror_bin(data_ori,values)
  print fitness(mirror)

def random_search():
  data_ori = read_file(files_test[1])
  fit_max = 0
  best = {}
  for i in range(0,5000):
    values   = hash_bin(files_test[1])
    mirror = mirror_bin(data_ori,values)
    fit = fitness(mirror)
    if fit_max < fit:
      best = values
      fit_max = fit
  print best
  print fit_max

random_search()