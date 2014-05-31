#include <gmock/gmock.h>
#include "Archive.h"
#include "ArchivedFile.h"

using namespace testing;

TEST( ArchivedFile, CanRetrieveItsPath )
{
  std::string filename = "test_file.txt";
  ArchivedFile file( filename );
  ASSERT_THAT( file.getPath(), Eq( filename ) );
}

TEST( BasicFunctions, CanDoNothing )
{
  ASSERT_TRUE( true );
}

TEST( BasicFunctions, CanCreateEmptyZipFile )
{
  Archive myArchive( archive_formats::ZIP );
  ASSERT_THAT( myArchive.getType(), Eq( archive_formats::ZIP ) );
}

TEST( BasicFunctions, CanAddAFileToAnArchive )
{
  std::string filename = "my_file.txt";
  Archive myArchive;
  ArchivedFile myFile( filename );
  myArchive.addFile( myFile );
  auto retrievedFile = myArchive.getFile( filename );
  ASSERT_THAT( retrievedFile->getPath(), Eq( filename ) );
}

struct ANonEmptyArchive : public Test
{
  Archive myArchive;
  const std::string filename = "my_file.txt";
  const std::string fileContents = "Insert some Samuel L Ipsum.";

  void SetUp() override
  {
    ArchivedFile myFile( filename );
    myFile.setContents( fileContents );
    myArchive.addFile( myFile );
  }
};

TEST_F( ANonEmptyArchive, CanExtractFileToStream )
{
  std::stringstream ss;
  myArchive.extractFile( filename, ss );

  ASSERT_THAT( ss.str(), Eq( fileContents ) );
}