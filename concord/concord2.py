#!/usr/bin/env python3
import sys

def main():
    read_in = read_stdin() # list containing list of exclusions and list of lines
    formatted = find_n_cap(read_in) # tuple containing key word and capitalized lines
    formatted.sort(key=lambda x: x[0]) # sort by keywords 'x[0]'
    to_push = output_form(formatted) # finalized spacing and cut off of overflow words

    for x in to_push:
        print(x)

# read in the input file and separate the exclusion words
# and phrases into lists
def read_stdin():
    # check if input file is the correct version
    if "1" in sys.stdin.readline():
        print("Input is version 1, concord2.py expected version 2")
        sys.exit(1)
    
    line = sys.stdin.readline(); #gets rid of first set of '''
    exclusions = []
    phrases = []
    
    # read in exclusion words
    for line in sys.stdin:
        if "\"" in line:
            break
        exclusions.append(line.strip()) #removes \n's

    # read in phrases
    for line in sys.stdin:
        phrases.append(line.strip()) # removes \n's

    return [exclusions, phrases]

# find any words that aren't excluded and capitalize them
# and add them to the tuple to be sorted
def find_n_cap(input_lists):
    exclude = input_lists[0]
    phrases = input_lists[1]
    formatted = [] # list of tuples ("key", "phrase")

    for line in phrases: # pulls out one line
        for word in line.split(): # splits sentance into list of words
            isX = False
            if exclude: # if there are exclude words
                for x in exclude:
                    isX = False
                    if x.upper() == word.upper(): # make sure the case is same
                        isX = True
                        break
            if not isX:
                formatted.append((word.upper(), cap_key(word,line)))
    return formatted

# capitalize the given word in the given sentance
def cap_key(word, line):
    result = ''
    sep = ''
    for w in line.split():
        if w == word:
            result += sep + word.upper()
        else:
            result += sep + w
        sep = ' '
    return result

# format the spacing and centering of the sentances in a list
def output_form(keys_n_lines):
    output_lines = []
    for (k,x) in keys_n_lines:
        x = space_n_cut(x,k)
        output_lines.append(x)
    return output_lines

# calculate the space needed to centre line and cut off overflow words
def space_n_cut(line, centre):
    result = ""
    # cut off overflow to the left of the key
    while line.index(centre) > 20:
        line = ' '.join(line.split(' ')[1:])
    # cut off overflow to the right of the key
    while len(line) - line.index(centre) > 31:
        line = ' '.join(line.split(' ')[:-1])
    result = (" " * (29-line.index(centre))) + line
    return result 

if __name__ == "__main__":
    main()
