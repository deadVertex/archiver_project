import subprocess
import zipfile #zip files
import rarfile #rar files (open only)
import tarfile #tar, tar.bz2 and tar.gz/tgz files
import lzma #lzma and xz files
import bz2 #bz2 files
import gzip #gz files

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
        elif self.fileExtension == "lz" or self.fileExtension == "xz":
            self.openLzma()
        elif self.fileExtension == "7z":
            self.open7z()
        elif self.fileExtension == "bz2":
            self.openBz2()
        elif self.fileExtension == "gz":
            self.openGz()

    def write(self, contents):
        if self.fileExtension == "zip":
            self.writeZip(contents)
        elif self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz":
            self.writeTar(contents)
        elif self.fileExtension == "7z":
            self.write7z(contents)
            
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

    def openLzma(self):
        #open the lzma
        self.a = lzma.open(self.fileName, "r")
        #read contents
        file_content = self.a.read()
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

    def open7z(self):
        #open the 7z
        self.a = subprocess.call("7za.exe l " + self.fileName)
        #This line will catch the 7za.exe's output and return it as a byte array
        #self.a = subprocess.check_output("7za.exe l -slt " + self.fileName)#.decode('utf-8')#Converts byte array to string
        #print(self.a)
        print("Success")

    def write7z(self, contents):
        #give the commands to 7za.exe for compressing
        for each in contents:
            self.a = subprocess.check_output("7za.exe a " + self.fileName + " " + each['name'])
        print("Success")

    def openBz2(self):
        #open the bz2
        self.a = bz2.open(self.fileName, "r")
        #read the data
        self.aNameList = self.a.read()
        print("Success")
        #close the file
        self.a.close()

    def writeBz2(self, contents):
        #open the bz2 for writing
        self.a = bz2.open(self.fileName, "w")
        #write data
        self.a.write(contents[0]["basename"])
        #close the file
        self.a.close()

    def openGz(self):
        #open the gz
        self.a = gzip.open(self.fileName, "r")
        #read data
        self.aNameList = self.a.read()
        print("Success")
        #close the file
        self.a.close()

    def writeGz(self, contents):
        #open the gz for writing
        self.a = gzip.open(self.fileName, "w")
        #write data
        self.a.write(contents[0]["basename"])
        #close the file
        self.a.close()
