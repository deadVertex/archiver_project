#include "libarchive/archive.h"
#include "libarchive/archive_entry.h"

#include <cstdlib>

int main( int argc, char **argv )
{
	struct archive *a;
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
		printf( "%s", archive_entry_pathname( entry ) );
		archive_read_data_skip( a );
	}

	r = archive_read_free( a );
	if ( r != ARCHIVE_OK )
		exit( 1 );
}