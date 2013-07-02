#include "libarchive/archive.h"
#include "libarchive/archive_entry.h"
#include "cassert"

#include <cstdlib>

void write_archive(const char *outname, const char *filename)
{
  struct archive *a;
  struct archive_entry *entry;
  struct stat st;
  char buff[8192];
  int len;
  FILE *fd;

  a = archive_write_new();
  archive_write_add_filter_gzip(a);
  archive_write_set_format_pax_restricted(a); // Note 1
  archive_write_open_filename(a, outname);
  while (filename) {
    stat(filename, &st);
    entry = archive_entry_new(); // Note 2
    archive_entry_set_pathname(entry, filename);
    archive_entry_set_size(entry, st.st_size); // Note 3
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    archive_write_header(a, entry);
    fd = fopen(filename, "rb");
	assert(fd);
    len = fread(buff, 1, sizeof(buff), fd);
    while ( len > 0 ) {
        archive_write_data(a, buff, len);
        len = fread(buff, 1, sizeof(buff), fd);
    }
    fclose(fd);
    archive_entry_free(entry);
    filename++;
  }
  archive_write_close(a); // Note 4
  archive_write_free(a); // Note 5
}

int main( int argc, char **argv )
{
	const char *outname = "";

	write_archive(outname, "I:\\Files\\Documents\\GitHub\\archiver_project\\bin\\win32\\test.html");

	return 0;

	/*struct archive *a;
	struct archive_entry *entry;
	int r;
	
	a = archive_read_new();
	archive_read_support_filter_all( a );
	archive_read_support_format_all( a );

	r = archive_read_open_filename( a, "archive.zip", 0 );
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
	//	exit( 1 );*/
}