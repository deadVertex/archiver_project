import sys
import os
import zipfile
import rarfile

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
        except Exception:
            print("There was an error writing to file. Please ensure files are in the same folder as the .py, and the filenames are valid.")
    else:
        print("Incorrect command line parameters supplied")

class Archive:
    #GENERAL (EXTENSION INDEPENDANT) METHODS
    def __init__(self, archive):
        #Constructor
        self.fileName = archive
        self.fileExtension = self.fetchExtension(archive)

    def fetchExtension(self, fileName):
        #Fetches file extension to define archive type
        return self.fileName[self.fileName.rfind('.')+1:]

    def open(self):
        if self.fileExtension == "zip":
            self.openZip()
        elif self.fileExtension == "rar":
            self.openRar()

    def write(self, contents):
        if self.fileExtension == "zip":
            self.writeZip(contents)

    #EXTENSION SPECIFIC METHODS
    def openZip(self):
        #open the zip
        self.a = zipfile.ZipFile(self.fileName, "r")
        #fetch the files/folders
        self.aNameList = self.a.namelist()
        #close the file
        self.a.close()

    def writeZip(self, contents):
        #open the zip for writing
        self.a = zipfile.ZipFile(self.fileName, "w")
        #write files
        for data in contents:
            self.a.write(data['basename'], data['name'], zipfile.ZIP_DEFLATED)
        #close the file
        self.a.close()

    def openRar(self):
        #open the zip
        self.a = rarfile.RarFile(self.fileName, "r")
        #fetch the files/folders
        self.aNameList = self.a.namelist()
        #close the file
        self.a.close()
    
#Start execution
archiverMain()
