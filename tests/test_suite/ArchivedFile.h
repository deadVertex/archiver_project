#pragma once

#include <string>

class ArchivedFile
{
public:
  ArchivedFile( const std::string &path )
    : m_path( path )
  {
  }

  ~ArchivedFile()
  {
  }

  std::string getPath() const
  {
    return m_path;
  }

  void setContents( const std::string fileContents )
  {
    m_fileContents = fileContents;
  }

  std::string getContents() const
  { 
    return m_fileContents;
  }

protected:

private:
  std::string m_path, m_fileContents;
};


