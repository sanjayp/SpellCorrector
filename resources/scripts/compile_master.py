# Compiles the master.dic file from all sub-dictionaries specified
# in subdictionary_locations.list.

input = open('subdictionary_locations.list', 'r')
output = open('../dictionaries/master.dic', 'a')

dict = []

for line in input:
    file = open(line.strip().rstrip('\n'))
    wordlist = []
    for line2 in file:
        wordlist.append(line2.strip().rstrip('\n'))
    dict = sorted(list(set(dict + wordlist)))

for word in dict:
    output.write(word+'\n')
