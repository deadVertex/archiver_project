import sys
import os
import Archive

def archiverMain():
    #Definitions
    i = 0
    fileName = ""
    mode = ""
    contents = []

    #Aesthetic spacing
    print()

    #Read command line parameters
    for arg in sys.argv:
        if i==1:
            mode = arg
        elif i==2:
            fileName = arg
        elif i>2:
            contents.append({'name':arg, 'basename':os.path.basename(arg)})
        i=i+1

    if not(os.path.isfile(fileName)) and (mode != "write" and mode != "about"):
        print("Supplied file does not exist")
        sys.exit(0)
    else:
        archive = Archive.Archive(fileName)

    #Check for valid input
    if mode == "read" and fileName != "":
        #Check valid input file then open the archive
        archive.open()
        print(archive.aNameList)
    elif mode == "write" and fileName != "":
        #write to archive
        archive.write(contents)
    elif mode == "extract":
        #Extract the contents of the archive to a listed folder
        if contents:
            archive.extract(contents[0]['name'])
        else:
            print("No destination path supplied! Use '/' to specify same directory as script")
    elif mode == "about":
        #About/Help prompt
        print()
        print("--An Archiver Project--")
        print()
        print("File Formats Supported:")
        print("zip, rar(read only), tar, tar.gz, tar.bz2, gz, bz2, lz, xz, 7z")
        print()
        print("--Commands--")
        print()
        print("read\tReads the files contained within an archive")
        print("\tSyntax: open archive_name")
        print()
        print("write\tClears and writes a file(s) to an archive")
        print("\tSyntax: write archive_name [files_to_be_compressed..]")
        print()
        print("extract\tExtracts the archive to a listed folder (\"/\" = same folder)")
        print("\tAlways overwrites files at destination")
        print("\tThis will ignore file names, except in the case of")
        print("\t.gz, .bz2 (but not the tar variants), .lz, or .xz")
        print("\tSyntax: extract archive_name destination_path")
        print()
        print("about\tDisplays this help and information utility")
        print("\tSyntax: about")
        print()
    else:
        print("Incorrect command line parameters supplied")
    
#Start execution
archiverMain()
