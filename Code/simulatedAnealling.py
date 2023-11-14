from dataclasses import dataclass
from itertools import count
from numpy import exp
import random
#import numpy as np
import operator
import math
@dataclass
#a dataclass to store an edge
class Edge:
    start: int
    end: int
    weight: int

def readFile(fileName):
    fileObj = open(fileName, "r") #opens the file in read mode
    words = fileObj.read().splitlines() #puts the file into a list
    fileObj.close()
    return words

def objective(es, em, b, ei):
    cost=0
    for i in range(len(es)):
        cost+=es[i].weight*abs(em[b[ei[es[i].start]]]-em[b[ei[es[i].end]]])
    return cost

def init(size):
    start=[]
    for i in range(size):
        start.append(i)
    return start

def sa(objective, start, iterations, temperature, es, em, ei):
    start_point=start
    start_point_eval=objective(es, em, start_point, ei)
    temp_start_point, temp_start_eval = start_point, start_point_eval
    outputs = []
    for i in range(iterations):
        ran1 = random.uniform(0, 1)
        ran2 = random.uniform(0, 1)
        pos1 = math.floor((len(start))*ran1)
        pos2 = math.floor((len(start))*ran2)
        temp_step = temp_start_point
        temp = temp_step[pos1]
        temp_step[pos1] = temp_step[pos2]
        temp_step[pos2] = temp
        temp_step_eval = objective(es, em, temp_step, ei)
        if temp_step_eval < start_point_eval:
            start_point, start_point_eval = temp_step, temp_step_eval
            outputs.append(start_point_eval)
            #print('Acceptance Criteria = %.5f' % mac," ",'iteration Number = ',i," " ,'new_best = %.5f' % start_point_eval)
        difference = temp_step_eval - temp_start_eval
        t = temperature / float(i + 1)
        mac = exp(-difference / t)
        if difference < 0 or random.uniform(0, 1) < mac:
            temp_start_point, temp_start_eval = temp_step, temp_step_eval
    return [start_point, start_point_eval, outputs]

edge_member_list=readFile("reference_small_cluster0.txt")
edge_member = []
edge_index ={}
for i in range(len(edge_member_list)):
    edge_member.append(int(edge_member_list[i])) #cconvert list to an int array
    edge_index[int(edge_member_list[i])]=i
#print(edge_member)
#print(edge_index)
edges_list=readFile("reference_small_edge.txt")
edges = []
for i in range(len(edges_list)):
    edge_str =  edges_list[i].split('\t')
    edge_int = list(map(int, edge_str))
    if(edge_int[0]>=edge_member[0] and edge_int[0]<=edge_member[len(edge_member)-1] and edge_int[1]>=edge_member[0] and edge_int[1]<=edge_member[len(edge_member)-1]):
        for j in range(len(edge_member)):
            if(edge_int[0]==edge_member[j]):
                for k in range(len(edge_member)):
                    if(edge_int[1]==edge_member[k]):
                        temp=Edge(edge_int[0], edge_int[1], edge_int[2])
                        edges.append(temp)

edges.sort(key=operator.attrgetter('start'))
start=init(len(edge_member))
cost=objective(edges, edge_member, start, edge_index)
print('no permutation cost: '+str(cost))

# initial temperature
temperature = 1000
# define the total no. of iterations
iterations = 20000000
start_point, output, outputs = sa(objective, start, iterations, temperature, edges, edge_member, edge_index)
print(start_point)
print(output)



