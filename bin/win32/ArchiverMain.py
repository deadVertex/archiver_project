import sys
import os

def archiverMain():
    #Definitions
    i = 0
    fileName = ""
    mode = ""

    #Read command line parameters
    for arg in sys.argv:
        if i==1:
            mode = arg
        elif i==2:
            fileName = arg
            break
        i=i+1

    #Check for valid input
    if mode == "open" and fileName != "":
        #Check valid input file
        if os.path.isfile(fileName):
            print("Valid file name supplied")
        else:
            print("Invalid file name supplied")
    elif mode == "write" and fileName != "":
        #Create output file to validate name
        try:
            outFile = open(fileName, "w+")
            #TEMPORARY DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<-------I WANT ATTENTION!
            outFile.close()
            os.remove(fileName)
            #TEMPORARY DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<-------I WANT ATTENTION!
        except FileNotFoundError:
            print("\"" + fileName + "\" is an invalid file name. Could not write to file.")
    else:
        print("Incorrect command line parameters supplied")

class Archive:
    def __init__(self, fileName):
        #Constructor
        self.fileName = fileName
        fileExtension = self.fetchExtension(fileName)

    def fetchExtension(self, fileName):
        #Fetches file extension to define archive type
        return self.fileName[self.fileName.rfind('.')+1:]
    
#Start execution
archiverMain()
