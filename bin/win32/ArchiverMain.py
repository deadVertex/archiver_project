import sys
import os
import zipfile #zip file
import rarfile #rar file (open only)
import tarfile #bz2 and gzip files

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

class Archive:
    #GENERAL (EXTENSION INDEPENDANT) METHODS
    def __init__(self, archive):
        #Constructor
        self.fileName = archive
        self.fileExtension = self.fetchExtension(archive)

    def fetchExtension(self, fileName):
        #Fetches file extension to define archive type
        if self.fileName[self.fileName.rfind('.')+1:] == "gz" or self.fileName[self.fileName.rfind('.')+1:] == "bz2":
            extension = self.fileName[self.fileName[:self.fileName.rfind('.')].rfind('.')+1:]
            if extension == "tar.gz" or extension == "tar.bz2":
                return extension
            else:
                return self.fileName[self.fileName.rfind('.')+1:]
        else:
            return self.fileName[self.fileName.rfind('.')+1:]

    def open(self):
        if self.fileExtension == "zip" and zipfile.is_zipfile(self.fileName):
            self.openZip()
        elif self.fileExtension == "rar" and rarfile.is_rarfile(self.fileName):
            self.openRar()
        elif (self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz") and tarfile.is_tarfile(self.fileName):
            self.openTar()

    def write(self, contents):
        if self.fileExtension == "zip":
            self.writeZip(contents)
        elif self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz":
            self.writeTar(contents)
            
    #EXTENSION SPECIFIC METHODS
    def openZip(self):
        #open the zip
        self.a = zipfile.ZipFile(self.fileName, "r")
        #fetch the files/folders
        self.aNameList = self.a.namelist()
        print("Success")
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
        print("Success")
        #close the file
        self.a.close()

    def openTar(self):
        #open the tar
        if self.fileExtension == "tar":
            self.a = tarfile.open(self.fileName, "r:")
        elif self.fileExtension == "tgz" or self.fileExtension == "tar.gz":
            self.a = tarfile.open(self.fileName, "r:gz")
        elif self.fileExtension == "tar.bz2":
            self.a = tarfile.open(self.fileName, "r:bz2")
        #fetch the files/folders
        self.aNameList = self.a.getnames()
        print("Success")
        #close the file
        self.a.close()

    def writeTar(self, contents):
        #open the tar for writing
        if self.fileExtension == "tar.gz" or self.fileExtension == "tgz":
            self.a = tarfile.open(self.fileName, "w:gz")
        elif self.fileExtension == "tar.bz2":
            self.a = tarfile.open(self.fileName, "w:bz2")
        else:
            self.a = tarfile.open(self.fileName, "w")
        #write files
        for data in contents:
            self.a.add(data['basename'], data['name'])
        #close the file
        self.a.close()
    
#Start execution
archiverMain()
