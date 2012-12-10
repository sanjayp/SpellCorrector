# A script to parse out benchmark words into a master list.

input = open('../benchmarks/spell-errors.txt','r')
output = open('../dictionaries/benchmark_words/spell-errors.dic','a')

for line in input:
    output.write((line.split(':'))[0].rstrip('\n')+'\n')
