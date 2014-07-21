import subprocess
import os
import zipfile #zip files
import rarfile #rar files (open only)
import tarfile #tar, tar.bz2 and tar.gz/tgz files
#import lzma #lzma and xz files (Python 3.3 or later)
import bz2 #bz2 files
import gzip #gz files

#Create Custom exception
class InvalidTypeError(Exception):
    pass

class Archive:
    #GENERAL (UNSPECIFIC EXTENSION) METHODS
    #Constructor
    def __init__(self, archive):
        self.fileName = archive
        self.fileExtension = self.fetchExtension(archive)
        self.extractMode = False

    #Fetches file extension to define archive type
    def fetchExtension(self, fileName):
        if self.fileName[self.fileName.rfind('.')+1:] == "gz" or self.fileName[self.fileName.rfind('.')+1:] == "bz2":
            extension = self.fileName[self.fileName[:self.fileName.rfind('.')].rfind('.')+1:]
            if extension == "tar.gz" or extension == "tar.bz2":
                return extension
            else:
                return self.fileName[self.fileName.rfind('.')+1:]
        else:
            return self.fileName[self.fileName.rfind('.')+1:]

    #Reads the archive for its file(s), and checking to see if the archive has to be extracted
    def open(self, loc=""):
        if self.fileExtension == "zip" and zipfile.is_zipfile(self.fileName):
            if self.extractMode:
                self.openZip(loc)
            else:
                self.openZip()
        elif self.fileExtension == "rar" and rarfile.is_rarfile(self.fileName):
            if self.extractMode:
                self.openRar(loc)
            else:
                self.openRar()
        elif (self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz") and tarfile.is_tarfile(self.fileName):
            if self.extractMode:
                self.openTar(loc)
            else:
                self.openTar()
        #elif self.fileExtension == "lz" or self.fileExtension == "xz":
        #    if self.extractMode:
        #        self.openLzma(loc)
        #    else:
        #        self.openLzma()
        elif self.fileExtension == "7z":
            if self.extractMode:
                self.open7z(loc)
            else:
                self.open7z()
        elif self.fileExtension == "bz2":
            if self.extractMode:
                self.openBz2(loc)
            else:
                self.openBz2()
        elif self.fileExtension == "gz":
            if self.extractMode:
                self.openGz(loc)
            else:
                self.openGz()
        else:
            print("Invalid/No file extension found!")
            raise(InvalidTypeError)

    #Writes the contents of 'contents' to the archive
    def write(self, contents):
        if self.fileExtension == "zip":
            self.writeZip(contents)
        elif self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz":
            self.writeTar(contents)
        elif self.fileExtension == "7z":
            self.write7z(contents)
        elif self.fileExtension == "bz2":
            self.writeBz2(contents)
        elif self.fileExtension == "gz":
            self.writeGz(contents)
        else:
            print("Invalid/No file extension found!")
            raise(InvalidTypeError)

    def extract(self, loc):
        #remove the filename from location for certain archive types to use
        loc = os.path.split(loc)
        #Find the correct archive extension, and extract
        self.extractMode = True
        self.open(loc)
            
    #EXTENSION SPECIFIC METHODS
    def openZip(self, loc=""):
        #open the zip
        self.a = zipfile.ZipFile(self.fileName, "r")
        #fetch the files/folders
        self.aNameList = self.a.namelist()
        if self.extractMode:
            self.a.extractall(loc[0])
            print("All files in " + self.fileName + " written to " + loc[0] + "/.")
        else:
            for each in self.aNameList:
                print(each)
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

    def openRar(self, loc=""):
        #open the zip
        self.a = rarfile.RarFile(self.fileName, "r")
        #fetch the files/folders
        self.aNameList = self.a.namelist()
        if self.extractMode:
            self.a.extractall(loc[0])
            print("All files in " + self.fileName + " written to " + loc[0] + "/.")
        else:
            for each in self.aNameList:
                print(each)
        #close the file
        self.a.close()

    def openTar(self, loc=""):
        #open the tar
        if self.fileExtension == "tar":
            self.a = tarfile.open(self.fileName, "r:")
        elif self.fileExtension == "tgz" or self.fileExtension == "tar.gz":
            self.a = tarfile.open(self.fileName, "r:gz")
        elif self.fileExtension == "tar.bz2":
            self.a = tarfile.open(self.fileName, "r:bz2")
        #fetch the files/folders
        self.aNameList = self.a.getnames()
        if self.extractMode:
            self.a.extractall(loc[0])
            print("All files in " + self.fileName + " written to " + loc[0] + "/.")
        else:
            for each in self.aNameList:
                print(each)
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

    def openLzma(self, loc=""):
        #open the lzma
        self.a = lzma.open(self.fileName, "r")
        #read contents
        self.aNameList = self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')]
        if self.extractMode:
            if loc[1] != "":
                b = open(loc[0] + "/" + loc[1], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + loc[1] + " written to " + loc[0] + "/.")
            else:
                b = open(loc[0] + "/" + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')] + " written to " + loc[0] + "/.")
        print("Success")
        #close the file
        self.a.close()

    def writeLzma(self, contents):
        #open the lzma for writing
        self.a = lzma.open(self.fileName, "w")
        #Write data to archive
        self.a.write(open(contents[0]["basename"]))
        #close the file
        self.a.close()

    def open7z(self, loc=""):
        #open the 7z
        if self.extractMode:
            print("7za.exe e " + self.fileName + " -o" + loc[0] + "/ -y")
            self.a = subprocess.call("7za.exe e " + self.fileName + " -o" + loc[0] + "/ -y")
        else:
            self.a = subprocess.call("7za.exe l " + self.fileName)
        #This line will catch the 7za.exe's output and return it as a byte array
        #self.a = subprocess.check_output("7za.exe l -slt " + self.fileName)#.decode('utf-8')#Converts byte array to string
        #print(self.a)

    def write7z(self, contents):
        #give the commands to 7za.exe for compressing
        for each in contents:
            self.a = subprocess.check_output("7za.exe a " + self.fileName + " " + each['name'])
        print("Success")

    def openBz2(self, loc=""):
        #open the bz2
        self.a = bz2.open(self.fileName, "r")
        #read the data
        self.aNameList = [self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')]]
        if self.extractMode:
            if loc[1] != "":
                b = open(loc[0] + "/" + loc[1], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + loc[1] + " written to " + loc[0] + "/.")
            else:
                b = open(loc[0] + "/" + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')] + " written to " + loc[0] + "/.")
        #close the file
        self.a.close()

    def writeBz2(self, contents):
        #open the bz2 for writing
        self.a = bz2.open(self.fileName, "w")
        #write data
        self.a.writelines(open(contents[0]["basename"], 'rb'))
        #close the file
        self.a.close()

    def openGz(self, loc=""):
        #open the gz
        self.a = gzip.open(self.fileName, "r")
        #read data
        self.aNameList = [self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')]]
        if self.extractMode:
            if loc[1] != "":
                b = open(loc[0] + "/" + loc[1], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + loc[1] + " written to " + loc[0] + "/.")
            else:
                b = open(loc[0] + "/" + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')], "wb")
                b.write(self.a.read())
                b.close()
                print("File " + self.fileName[self.fileName.rfind('/')+1:self.fileName.rfind('.')] + " written to " + loc[0] + "/.")
        #close the file
        self.a.close()

    def writeGz(self, contents):
        #open the gz for writing
        self.a = gzip.open(self.fileName, "w")
        #write data
        self.a.writelines(open(contents[0]["basename"], 'rb'))
        #close the file
        self.a.close()
