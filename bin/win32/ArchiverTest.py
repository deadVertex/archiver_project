import unittest, os, shutil
from Archiver import *

def createTestContent():
	os.mkdir( "test_temp/test_directory" )
	os.mkdir( "test_temp/test_directory/folder1" )
	os.mkdir( "test_temp/test_directory/folder2" )
	os.mkdir( "test_temp/test_directory/folder1/folder3" )
	f0 = open( "test_temp/file0.txt", 'w')
	f0.write( "5 man jungle jam." )
	f0.close()

	f1 = open( "test_temp/test_directory/folder1/file1.txt", 'w' )
	f1.write( "Swiggity Swooty." )
	f1.close()

	f2 = open( "test_temp/test_directory/folder1/folder3/file2", 'w' )
	f2.write( "Holla Holla Get Dolla." )
	f2.close()


class TestArchiverClass( unittest.TestCase ):

	def test_canRecognizeZip( self ):
		testArchiver = Archive( "test_archives/folder.zip" )
		self.assertTrue( testArchiver.isZip )

	def test_readZip( self ):
		testArchiver = Archive( "test_archives/folder.zip" )
		testArchiver.openZip()
		self.assertEqual( len( testArchiver.aNameList ), 4 )

	def test_writeZipSingleFile( self ):
		testArchiver = Archive( "test_temp/test_zip.zip" )
		contents = []
		filepath = "test_temp/file0.txt"
		# TODO: Make a function for this.
		contents.append({'name': os.path.basename( filepath ), 'basename':filepath})
		testArchiver.writeZip( contents )
		testArchiver.openZip()
		self.assertEqual( testArchiver.aNameList[ 0 ] , "file0.txt" )

	def test_canRecognizeRar( self ):
		testArchiver = Archive( "test_archives/folder.rar" )
		self.assertTrue( testArchiver.isRar() )

	def test_readRar( self ):
		testArchiver = Archive( "test_archives/folder.rar" )
		testArchiver.openRar()
		self.assertEqual( len( testArchiver.aNameList ), 4 )

	def test_canRecognizeTar( self ):
		testArchiver = Archive( "test_archives/folder.tar" )
		self.assertTrue( testArchiver.isTar() )
		self.assertTrue( testArchiver.isTarType() )

	def test_readTar( self ):
		testArchiver = Archive( "test_archives/folder.tar" )
		testArchiver.openTar()
		self.assertEqual( len( testArchiver.aNameList ), 3 )

	def test_canRecognizeTarGz( self ):
		testArchiver = Archive( "test_archives/folder.tar.gz" )
		self.assertTrue( testArchiver.isTarGz() )
		self.assertTrue( testArchiver.isTarType() )

	def test_readTarGz( self ):
		testArchiver = Archive( "test_archives/folder.tar.gz" )
		testArchiver.openTar()
		self.assertEqual( len( testArchiver.aNameList ), 3 )

	def test_canRecognizeTarBz2( self ):
		testArchiver = Archive( "test_archives/folder.tar.bz2" )
		self.assertTrue( testArchiver.isTarBz2() )
		self.assertTrue( testArchiver.isTarType() )

	def test_readTarBz2( self ):
		testArchiver = Archive( "test_archives/folder.tar.bz2" )
		testArchiver.openTar()
		self.assertEqual( len( testArchiver.aNameList ), 3 )

os.mkdir( "test_temp" )
createTestContent()
unittest.main( exit=False )
shutil.rmtree( "test_temp" )
