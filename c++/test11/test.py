#!/usr/bin/python

import sys, os

def parse_dir(path):
    print "parse dir %s" % (path)
    d = os.walk(path)
    for p, dir_list, file_list in d:
        # print p, dir_list, file_list
        for dir_name in dir_list:
            parse_dir(os.path.join(p, dir_name))
        for file_name in file_list:  
            parse_file(os.path.join(p, file_name))

def parse_file(path):
    print "parse file %s" % (path)
    with open(path, "r") as f:
        lines = f.readlines()
        argv = []
        for line in lines:
            str_raw = line.strip()
            if str_raw:
                argv.append("\'%s\'" % (str_raw))
        if argv:
            os.system("./proc %s" % (' '.join(argv)))

def parse_path(path):
    if not os.path.exists(path):
        print "path %s not exist" % (path)
        return
    if os.path.isdir(path):
        parse_dir(path)
    elif os.path.isfile(path):
        parse_file(path)

def main():
    argc = len(sys.argv)
    if argc <= 1:
        print "usage test.py dirpath/filepath"
        return
    
    for i in range(1, argc):
        parse_path(sys.argv[i])

if __name__ == "__main__":
    main()