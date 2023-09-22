#!/usr/bin/env python3.7

# Copyright @yansong.li

# ILP model:
#  minimize
#        sum of w[u][v]*d[u][v] for all edges
#  subject to
#        d[u][v] >= p[v]-p[u] for all edges
#        d[u][v] >= p[u]-p[v] for all edges
#        p[v] =  sum of i*x[v][i] for all v
#        sum of x[v][i] = 1 for all v
#        sum of x[v][i] = 1 for all i
#
#        x[v][i] is binary for all v,i

import gurobipy as gp
import math
from gurobipy import GRB
try:

    # weights

    w = []
    with open ("reference_10million_cluster14_1000_new_matrix.txt") as textFile:
        for line in textFile:
            current = [item.strip() for item in line.split(' ')]
            w.append(current)
    V=len(w)
    print(V)
    # Create a new model
    m = gp.Model("cluster")

    #set some parameters to speed up the optimization
    #m.setParam("LogFile", "reference__cluster0.log")
    m.setParam("MIPFocus", 3)
    m.setParam("NoRelHeurTime", 150)
    m.setParam("TimeLimit", 200)
    #m.setParam("Heuristics", 0.000001)
    #m.setParam("RINS", 1)
    #m.setParam("Cuts", 3)

    # Create variables
    d = m.addVars(V, V, vtype=GRB.INTEGER,name="d")
    x = m.addVars(V, V, vtype=GRB.BINARY, name="x")
    p = m.addVars(V, vtype=GRB.INTEGER, name="p")
    MIPFocus=1
    # Set objective: minimize sum of w[u][v]*d[u][v] for all edges
    m.setObjective(gp.quicksum(gp.quicksum((w[u][v] * d[u,v]) for v in range(V)) for u in range(V)), GRB.MINIMIZE)

    # Add constraint: d[u][v] >= p[v]-p[u] for all edges
    m.addConstrs((d[u,v] >= p[v]-p[u] for v in range(V) for u in range(V)), "c0")

    # Add constraint: d[u][v] >= p[u]-p[v] for all edges
    m.addConstrs((d[u,v] >= p[u]-p[v] for v in range(V) for u in range(V)), "c1")

    # Add constraint: p[v] =  sum of i*x[v][i] for all v
    m.addConstrs((p[v] == gp.quicksum(i * x[v,i] for i in range(V)) for v in range(V)), "c2")

    # Add constraint: sum of x[v][i] = 1 for all v 
    m.addConstrs((gp.quicksum(x[v,i] for i in range(V)) == 1 for v in range(V)), "c3")

    # Add constraint: sum of x[v][i] = 1 for all i
    m.addConstrs((gp.quicksum(x[v,i] for v in range(V)) == 1 for i in range(V)), "c4")
    

    # Optimize model
    m.optimize()

    #for v in m.getVars():
            #print('%s %g' % (v.VarName, v.X))
    for i in range(len(p)):
        print(p[i])
    f = open("reference_10million_cluster14_1000_new_permutation.txt", "w")
    for i in range(len(p)):
        f.write(str(math.floor(p[i].X))+"\n")
    f.close()
    print('Obj: %g' % m.ObjVal)
except gp.GurobiError as e:
    print('Error code ' + str(e.errno) + ': ' + str(e))

except AttributeError:
    print('Encountered an attribute error')