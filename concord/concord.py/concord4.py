import sys
import re

LEFT_OVERFLOW_LIM = 20
RIGHT_OVERFLOW_LIM = 31
CHARS_BEFORE_KEY = 29

class concord:
    def __init__(self, input=None, output=None):
        self.input = self.__setup_file(input)
        self.output = output
        self.exclusions = self.__get_exclusions()
        self.phrases = self.__get_phrases()
        self.keys_n_indicies = self.__organize_input()
        self.concord = self.__format_concord()
        self.output_result()

    # depending on given input return opened file/stdin
    def __setup_file(self, input):
        if input == None:
            return sys.stdin
        else:
            return open(input, 'r')

    # collect the listed exclusions bounded by ''' and """
    # from stdin/input file
    def __get_exclusions(self):
        # check if correct file version is input
        if "1" in self.input.readline():
            print("Input is version 1, concord4.py expected version 2")
            if not self.input:
                self.input.close()
            sys.exit(1)
        
        self.input.readline(); # remove '''' line
        ex_words = []

        # read exclusion words until "" is read in 
        for x in self.input:
            if '"' in x:
                break;    
            ex_words.append(x.strip().lower())
        return ex_words

    # collect the listed phrases from stdin/input file
    # from where __get_exclusions finished reading
    def __get_phrases(self):
        phrases = []
        for x in self.input:  
            phrases.append(x.strip())
        
        # if a file was opened, we close it
        if not self.input:
            self.input.close()
        return phrases

    # finds and returns all of the index words and their respective line 
    # numbers in a sorted dictionary
    def __organize_input(self):
        # collects touples of index words and their line numbers
        index_words = []
        line_num = 0
        for line in self.phrases:
            index_words += [(word, line_num) for word in line.split() if not self.__is_exclusion(word)]
            line_num += 1

        # if index word alredy in collection (ie. a repeated index word) 
        # append line number, otherwise add a new entry to collection
        collection = {}
        for x in index_words:
            if x[0] in collection:
                collection[x[0]].append(x[1])
            else:
                collection[x[0]] = [x[1]]
        return dict(sorted(collection.items(), key=lambda x: x[0].lower()))

    # checks if passed in word is in exclusion list
    def __is_exclusion(self, word):
        for ex in self.exclusions:
            if re.search(r'\b' + ex + r'\b', word, re.IGNORECASE):
                return True
        return False

    # create the final output by putting the sorted index words and corresponding 
    # lines into a correctly formatted concord list
    def __format_concord(self):
        listed_concord = []
        for keyword in self.keys_n_indicies:
            for index in self.keys_n_indicies[keyword]:
                cur_line = self.phrases[index]
                cur_line = re.sub(r'\b' + keyword + r'\b', keyword.upper(), cur_line)
                listed_concord.append((keyword.upper(), cur_line))
        return self.__add_spacing(listed_concord)

    # adds in spacing and removes overflow words of given lines
    def __add_spacing(self, capped_phrases):
        spaced_list = []
        for keyword, phrase in capped_phrases:
            result = ""
            # cut off overflow to the left of the key
            while self.__index_of_key(keyword, phrase) > LEFT_OVERFLOW_LIM:
                phrase = ' '.join(phrase.split(' ')[1:])
            # cut off overflow to the right of the key
            while len(phrase) - self.__index_of_key(keyword, phrase) > RIGHT_OVERFLOW_LIM:
                phrase = ' '.join(phrase.split(' ')[:-1])
            result = (" " * (CHARS_BEFORE_KEY-self.__index_of_key(keyword, phrase))) + phrase
            spaced_list.append(result)
        return spaced_list

    # finds index of given keyword within given phrase
    def __index_of_key(self, keyword, phrase):
        matchobj = re.search((r'\b' + keyword + r'\b'), phrase)
        return matchobj.span()[0]

    # determines if result needs to be output to a file 
    # if it does output to given file
    def output_result(self):
        if not self.output:
            return
        f = open(self.output, "w")
        for line in self.concord:  
            f.write(line + "\n")
        f.close()

    def full_concordance(self):
        return self.concord