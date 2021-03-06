import zipfile #zip file
import rarfile #rar file (open only)
import tarfile #bz2 and gzip files

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

    def isZip( self ):
        return self.fileExtension == "zip" and zipfile.is_zipfile(self.fileName)

    def isRar( self ):
        return self.fileExtension == "rar" and rarfile.is_rarfile(self.fileName)

    def isTarGz( self ):
        return ( self.fileExtension == "tar.gz" or self.fileExtension == "tgz" ) and tarfile.is_tarfile(self.fileName)

    def isTarBz2( self ):
        return self.fileExtension == "tar.bz2" and tarfile.is_tarfile(self.fileName)

    def isTar( self ):
        return self.fileExtension == "tar" and tarfile.is_tarfile(self.fileName)

    def isTarType( self ):
        return (self.fileExtension == "tar.gz" or self.fileExtension == "tar.bz2" or self.fileExtension == "tar" or self.fileExtension == "tgz") and tarfile.is_tarfile(self.fileName)

    def open(self):
        if self.isZip():
            self.openZip()
        elif self.isRar():
            self.openRar()
        elif self.isTarType():
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
    
