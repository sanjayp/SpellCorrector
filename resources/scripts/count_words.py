# Counts the number of misspelled words in spell-errors.txt.

file = open("../benchmarks/spell-errors.txt", 'r');
count = 0
for line in file:
    line = line.split(':')
    wrong = line[1].split(',')
    count+=len(wrong)
print count
