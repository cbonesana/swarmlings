#!/usr/bin/env python

import sys
import string
import fileinput
import os
import glob
import datetime
import time

dataset=['ch130','d198','eil76','fl1577','kroA100','lin318','pcb442','pr439','rat783','u1060']
bestval=[999999,999999,999999,999999,999999,999999,999999,999999,999999,999999]
seeds=[0,0,0,0,0,0,0,0,0,0]
algorithms=['','','','','','','','','','']


for element in glob.glob('./output/text/*.txt'):
	i=0
	data=''
	nome=''
	citta=0
	dist=0
	seed=0
	for line in fileinput.input([element]):
		#print str(i)+": "+line
		if i == 2:
			tmp = line.split(' ',1)
			data = str(tmp[1]).strip()
		if i == 3:
			tmp = line.split(' ',2)
			nome = str(tmp[2]).strip()
		if i == 4:
			tmp = line.split(' ',2)
			citta = string.atoi(tmp[2])
		if i == 5:
			tmp = line.split(' ',3)
			try:
				dist = string.atoi(tmp[3])
			except ValueError:
				dist = 999999
		if i == 6:
			tmp = line.split(' ',2)
			seed = string.atoi(tmp[2])
		i=i+1
	#print data + "\t\t" + nome + "\t" + str(dist) + "\t" + str(seed)

	index = dataset.index(data)
	if bestval[index] > dist:
		bestval[index] = dist
		seeds[index] = seed
		algorithms[index] = nome
	

actual_time = datetime.datetime.now().isoformat("_")
filename = './output/'+str(actual_time)+'_results.txt'
f = open(filename,"w")
i=0
for element in dataset:
	f.writelines(dataset[i] + "\t" + algorithms[i] + "\t" + str(bestval[i]) + "\t" + str(seeds[i]) + "\n")
	i=i+1
f.close()

