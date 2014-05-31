#pragma once

#include <memory>
#include <iosfwd>

#include "ArchivedFile.h"

namespace archive_formats
{
  enum ArchiveFormatEnum
  {
    UNKNOWN = 0,
    ZIP,
  };
}

class Archive
{
public:
  Archive( int format = archive_formats::UNKNOWN ) :
    m_archiveFormat( format )
  {
  }

  ~Archive()
  {
  }

  int getType() const
  {
    return m_archiveFormat;
  }

  bool addFile( const ArchivedFile &file )
  {
    // NOTE: Assuming that the file's path is in a valid cross-platform path format.
    // TODO: Read file path and store it in some sort of directory structure.
    m_file = std::make_shared< ArchivedFile >( file );
    // TODO: Return false if file path is not unique.
    return true;
  }

  ArchivedFile* getFile( const std::string &path )
  {
    return m_file.get();
  }

  void extractFile( const std::string &path, std::ostream &stream )
  {
    stream << m_file->getContents();
  }

private:
  int			m_archiveFormat;
  std::shared_ptr<ArchivedFile> m_file;

};


