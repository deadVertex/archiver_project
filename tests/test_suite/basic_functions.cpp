#include <gmock/gmock.h>
#include "Archive.h"
#include "ArchivedFile.h"

using namespace testing;

TEST(ArchivedFile, CanRetrieveItsPath)
{
  std::string filename = "test_file.txt";
  ArchivedFile file(filename);
  ASSERT_THAT(file.getPath(), Eq(filename));
}

TEST(BasicFunctions, CanDoNothing)
{
	ASSERT_TRUE(true);
}

TEST(BasicFunctions, CanCreateEmptyZipFile)
{
  Archive myArchive(archive_formats::ZIP);
  ASSERT_THAT(myArchive.getType(), Eq(archive_formats::ZIP));
}

TEST(BasicFunctions, CanAddAFileToAnArchive)
{
  std::string filename = "my_file.txt";
  Archive myArchive;
  ArchivedFile myFile(filename);
  myArchive.addFile(myFile);
  auto retrievedFile = myArchive.getFile(filename);
  ASSERT_THAT(retrievedFile->getPath(), Eq(filename));
}