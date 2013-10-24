#include "archive_worker.h"
#include <sstream>
#include <string>

bool ArchiveWorker::GetStructure()
{
	//provide a structure of the directories within the archive in fileStructure global vector
	//directory list is from the deepest folder; invert list to start at the top
	int j = 0;
	for (int i=directories.size(); i>-1; i--)
	{
		std::vector<std::string> row;
		size_t seperator = directories[i].find_last_of('/');
		if (seperator == -1)
			row.push_back(directories[i]);

		std::string tempPath;
		size_t seperator2;
		row.push_back(directories[i].substr(seperator+1, -1));
		seperator2 = directories[i].substr(0, seperator-1).find_last_of('/');
		tempPath = directories[i].substr(seperator2+1, seperator-seperator2);

		for (int g=0; g<fileStructure.size(); g++)
		{
			if (fileStructure[g][0] == tempPath)
			{
				std::stringstream ss;
				ss << i;
				fileStructure[g].push_back(ss.str());
			}
		}

		j++;
		//place the new row within fileStructure global
		fileStructure.push_back(row);
	}
	return;
}

int ArchiveWorker::OpenArchive( const char *path)
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
		// Check if the entry has the directory bit flag set.
		if ( archive_entry_stat( entry )->st_mode & S_IFDIR )
			directories.push_back( archive_entry_pathname( entry ) );
		//printf( "%s\n", archive_entry_pathname( entry ) );
		archive_read_data_skip( a );
	}

	r = archive_read_free( a );
	//getchar();
	//if ( r != ARCHIVE_OK )
	//	exit( 1 );
}

void ArchiveWorker::WriteArchive(const char *outname, std::vector<std::string> filename)
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
	int err1 = archive_write_close(a); // Note 4		
	int err2 = archive_write_free(a); // Note 5
}