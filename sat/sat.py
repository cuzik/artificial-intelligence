from random import *
import numpy as np
import matplotlib.pyplot as plt

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

def fitness(mirror,max_value):
  fit = 0
  for i in mirror:
    fit += point_clausula(i)
  return (max_value - fit)

def disturbance(values):
  data = values.copy()
  j = randint(1,len(data))
  data[j] = not values[j]
  return data

def function_r(T):
  return T*.995

def sat(test_case,T,alpha,L,P,M):
  data_ori = read_file(files_test[test_case])
  values   = hash_bin(files_test[test_case])
  data_t = []
  data_s = []
  for i in range(0,M):
    nsuccess = 0
    for j in range(0,P):
      disturb_valus = disturbance(values)

      mirror_v = mirror_bin(data_ori,values)
      mirror_d = mirror_bin(data_ori,disturb_valus)

      delta_F = fitness(mirror_d,len(data_ori)) - fitness(mirror_v,len(data_ori))
      if delta_F <= 0 or np.exp(-delta_F/T) > uniform(0,1):
        # print 'oi: ' + str(delta_F)
        values = disturb_valus
        nsuccess += 1

      if nsuccess >= L:
        break
    T = alpha(T)
    asd = fitness(mirror_bin(data_ori,values),len(data_ori))
    # print T
    data_t += [T]
    # print asd
    data_s += [asd]
    if nsuccess == 0 or T < 0.0005 or asd == 0:
      break
  # if fitness(mirror_bin(data_ori,values),len(data_ori)) == 0:
  plt.plot(data_t)
  plt.title("Convegencia da Temperatura")
  plt.grid(True)
  plt.xlabel("iteracoes")
  plt.ylabel("temporatura")
  plt.show()

  plt.plot(data_s)
  plt.title("Convegencia da Solucao")
  plt.grid(True)
  plt.xlabel("iteracoes")
  plt.ylabel("solucao f(x)")
  plt.show()
  return fitness(mirror_bin(data_ori,values),len(data_ori))

def random_search(test_case):
  data_ori = read_file(files_test[test_case])
  fit_max = 0
  best = {}
  for i in range(0,500000):
    values   = hash_bin(files_test[test_case])
    mirror = mirror_bin(data_ori,values)
    fit = fitness(mirror,len(data_ori))
    if fit_max < fit:
      best = values
      fit_max = fit
  # print best
  return fit_max

def main():
  while(True):
    print sat(3,20,function_r,2,100,500000)
  # for i in range(1,4):
  #   print 'case test ' + str(i)
  #   for j in range(0,10):
  #     print sat(i,20,function_r,2,100,500000)
      # print random_search(i)

if __name__ == '__main__':
  main()
