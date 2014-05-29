#pragma once

#include <string>

class ArchivedFile
{
public:
	ArchivedFile(const std::string &path)
    : m_path(path)
	{
	}

	~ArchivedFile()
	{
	}

  std::string getPath() const
  {
    return m_path;
  }

protected:

private:
  std::string m_path;
};


