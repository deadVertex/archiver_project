import sys
import os
from Archiver import *

def archiverMain():
    #Definitions
    i = 0
    fileName = ""
    mode = ""
    contents = []

    #Read command line parameters
    for arg in sys.argv:
        if i==1:
            mode = arg
        elif i==2:
            fileName = arg
            if mode != "write":
                break
        elif i>2:
            contents.append({'name':arg, 'basename':os.path.basename(arg)})
        i=i+1

    #Check for valid input
    if mode == "open" and fileName != "":
        #Check valid input file
        if not(os.path.isfile(fileName)):
            print("Invalid file name supplied")
        else:
            archive = Archive(fileName)
            archive.open()
    elif mode == "write" and fileName != "":
        #Create output file to validate name
        try:
            archive = Archive(fileName)
            archive.write(contents)
        except Exception as e:
            print("There was an error writing to file. Please ensure files are in the same folder as the .py, and the filenames are valid.\n" + e)
    else:
        print("Incorrect command line parameters supplied")

#Start execution
archiverMain()