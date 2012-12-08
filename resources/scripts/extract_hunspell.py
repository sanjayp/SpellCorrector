# A script to parse out hunspell words into a master list.

input = open('../dictionaries/hunspell/en_US.dic','r')
output = open('../dictionaries/hunspell/hunspell.dic','a')

for line in input:
    output.write((line.split('/'))[0].rstrip('\n')+'\n')
