#ifndef __ARCHIVE_WORKER_H__
#define __ARCHIVE_WORKER_H__

#include <vector>
#include <string>

#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

class ArchiveWorker
{
public:
	ArchiveWorker ()
	{
		myArchive = 0;
	}
	bool GetFilenames(std::vector<std::string> &filenames);
	bool GetStructure();
	bool ExtractFile(const std::string &filename);
	int OpenArchive(const char* path);
	void WriteArchive(const char *outname, std::vector<std::string> filename);
public:
	std::vector< std::vector<std::string> > fileStructure;
	std::vector< std::string > directories;
private:
	archive *myArchive; 
};

#endif