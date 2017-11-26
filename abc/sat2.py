import math
import locale
from random import *

MAX = 100
MIN = 0
NUM_BEES = 50
files_test = [6,20,100,250]
atual_file = 0


def read_file(input):
  input = str(input) + ".cnf"
  file = open(input)
  data = []
  for i in file:
    a = i.split(" ")
    a = a[:-1]
    data += [a]
  return data

data_ori = read_file(files_test[atual_file])

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




def obj_fun(bee):
  return fitness(mirror_bin(data_ori,bee))

def new_spot(variables_number, min, max):
  values = hash_bin(files_test[atual_file])
  return values


def population_init(variables_number, population_zise):
  data = []
  for i in range(0, population_zise):
    data += [new_spot(variables_number,MIN,MAX)]
  return data

def all_fit(employeds):
  data = []
  for bee in employeds:
    data += [partial_fit(bee)]
  return data

def partial_fit(bee):
  value = obj_fun(bee)
  if value >= 0:
    return 1.0/(1.0+value)
  return 1.0+abs(value)

def pi(employeds):
  fit = all_fit(employeds)
  total_value = sum(fit)
  data = []
  for i in fit:
    data += [i / total_value]
  return data

def sum(vector):
  value = 0
  for i in vector:
    value += i
  return value

def create_roulette(prob):
  data = []
  prob_range = 0
  for i in prob:
    prob_range += i
    data += [prob_range]
  return data

def collect_nectar(nectar_spots):
  data = []
  for i in nectar_spots:
    data += [i-1]
  return data

def position_select(roulette):
  data = []
  for i in range(0,len(roulette)):
    data += [patter_spot(roulette)]
  return data

def patter_spot(roulette):
  number = uniform(0,1)
  count = 0
  for i in roulette:
    if number <= i:
      return count
    count += 1

def switch_to_scout(onlookers, employeds, nectar_spots):
  for i in onlookers:
    if nectar_spots[i] <= 0:
      employeds[i] = new_spot(files_test[atual_file],MIN,MAX)
      nectar_spots[i] = 5
  return (employeds, nectar_spots)

def find_best(employeds, best_solution, interacoes):
  best_value = obj_fun(best_solution)
  switch = False
  for bee in employeds:
    atual = obj_fun(bee)
    if atual > best_value:
      switch = True
      best_value = atual
      best_solution = bee
  if switch:
    print('<',end='')
    print('%7i' % interacoes,end='')
    print(' => ',end='')
    print('%11.7f' % obj_fun(best_solution))
  return best_solution

def dance(employeds, nectar_spots):
  for i in range(0,len(employeds)):
    j = randint(0,files_test[atual_file]-1)
    k = randint(0,len(employeds)-1)
    while k == i:
      k = randint(0,len(employeds)-1)
    (employeds[i], flag_switch) = near_spot(employeds[i], j, employeds[k])
    if flag_switch:
      nectar_spots[i] = 5
  return (employeds, nectar_spots)

def copy_bee(bee):
  copy = bee.copy()
  return copy

def near_spot(bee_base, j, bee_aux):
  new_bee = copy_bee(bee_base)
  new_bee[j+1] = bee_aux[j+1]
  if obj_fun(new_bee) < obj_fun(bee_base):
    return (new_bee, True)
  return (bee_base, False)

def main():
  onlookers = []
  best_solution = new_spot(files_test[atual_file],MIN,MAX)

  #Inicialize a população;
  employeds = population_init(files_test[atual_file],int(NUM_BEES/2));
  nectar_spots = [6 for i in range(0,len(employeds))]
  for i in range(0,500):
    #Posicione as abelhas campeiras em suas fontes de alimento;
    (employeds, nectar_spots) = dance(employeds, nectar_spots)

    #Calcule a probabilidade de escolha da fonte por abelhas observadoras;
    nectar_spots = collect_nectar(nectar_spots)

    #Posicione as abelhas observadoras nas fontes de alimento dependendo de seus valores;
    roulette = create_roulette(pi(employeds))

    #Interrompa exploração de fontes esgotadas;
    onlookers = position_select(roulette)

    #Envie abelhas exploradoras para buscar novas fontes de alimento;
    (employeds, nectar_spots) = switch_to_scout(onlookers, employeds, nectar_spots)

    #Memorize a melhor fonte de alimento encontrada até o momento;
    best_solution = find_best(employeds, best_solution, i)

    # print(obj_fun(best_solution))
  print(best_solution)
  print(obj_fun(best_solution))

if __name__ == '__main__':
  main()