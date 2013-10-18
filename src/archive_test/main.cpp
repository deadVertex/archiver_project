#include "libarchive/archive.h"
#include "libarchive/archive_entry.h"
#include "cassert"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class ArchiveWorker
{
public:
	ArchiveWorker ()
	{
		myArchive = 0;
	}
	bool GetFilenames(std::vector<std::string> &filenames);
	bool ExtractFile(const std::string &filename);
	int OpenArchive(const char* path)
	{
		struct archive *a;
		struct archive_entry *entry;
		int r;
	
		a = archive_read_new();
		archive_read_support_filter_all( a );
		archive_read_support_format_all( a );

		r = archive_read_open_filename( a, path, 0 );
		if ( r != ARCHIVE_OK )
			exit( 1 );

		while ( archive_read_next_header( a, &entry ) == ARCHIVE_OK )
		{
			printf( "%s\n", archive_entry_pathname( entry ) );
			archive_read_data_skip( a );
		}

		r = archive_read_free( a );
		getchar();
		//if ( r != ARCHIVE_OK )
		//	exit( 1 );
	}

	void WriteArchive(const char *outname, std::vector<std::string> filename)
	{

		struct archive *a;
		struct archive_entry *entry;
		struct stat st;
		char buff[8192];
		int len;
		FILE *fd;

		a = archive_write_new();
		archive_write_add_filter_compress(a);
		//archive_errno(a);
		//const char *teeth = archive_error_string(a);
		archive_write_set_format_pax_restricted(a); // Note 1
		archive_write_open_filename(a, outname);
		/*for (int i=0; i<filename.size(); i++) {
			stat(filename[i].c_str(), &st);
			entry = archive_entry_new(); // Note 2
			archive_entry_set_pathname(entry, filename[i].c_str());
			archive_entry_set_size(entry, st.st_size); // Note 3
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0644);
			archive_write_header(a, entry);
			fd = fopen(filename[i].c_str(), "rb");
			assert(fd);
			len = fread(buff, 1, sizeof(buff), fd);
			while ( len > 0 ) {
				int poo = archive_write_data(a, buff, len);
				len = fread(buff, 1, sizeof(buff), fd);
			}
			fclose(fd);
			archive_entry_free(entry);
		}*/
		archive_write_close(a); // Note 4
		archive_write_free(a); // Note 5
	}
private:
	archive *myArchive;
};

int create(ArchiveWorker archive1)
{
	const char *outname = "penis.zip";
	std::vector<std::string> contents;
	contents.push_back ("test.html");
	contents.push_back ("ui.html");

	archive1.WriteArchive(outname, contents);

	return 0;
}

int main( int argc, char *argv )
{
	int method;
	ArchiveWorker archive1;

	cout << "Please select: \n1. Read Archive\n2. Create Archive\n\nEnter Selection (numbers only): ";
	cin >> method;

	if (method == 1){
		archive1.OpenArchive("archive.zip");
		getchar();
	}else if (method == 2){
		create(archive1);
		getchar();
	}else{
		cout << "Invalid selection. Terminating application.";
		getchar();
	}
}