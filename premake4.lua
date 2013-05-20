definitions_debug = { "DEBUG" }
definitions_release = { "RELEASE" }

flags_debug = { "Symbols" }
flags_release = { "OptimizeSpeed" }

solution( "archiver_project" )
	configurations( { "debug", "release" } )
	includedirs( {"include","src"} )
	platforms( {"x32","x64"} )
	configuration( "x64" )
		defines( "USE_64BIT" )
	configuration( "x32" )
		defines( "USE_32BIT" )
	configuration( "windows" )
		libdirs( { "lib/win32" } )
	configuration ""
	location("build")
	
	project( "window_test" )
		kind( "WindowedApp" )
		language( "C++" )
		targetname( "window_test" )
		files( { "src/window_test/**" } )
		includedirs( {"src/window_test/**"} )
		location( "build/window_test" )
		links( {"berkelium", "SDL2", "SDL2main" } )
		configuration( "windows" )
			targetdir( "bin/win32" )
			debugdir("bin/win32")
		configuration( "debug" )
			targetsuffix "_debug"
			flags( flags_debug )
			defines( definitions_debug )
		configuration("release")
			targetsuffix("_release")
			defines( definitions_release )
			flags( flags_release )
			
			
	project( "archive_contents_test" )
		kind( "ConsoleApp" )
		language( "C++" )
		targetname( "archive_contents_test" )
		files( { "src/archive_test/**" } )
		includedirs( {"src/archive_test/**"} )
		location( "build/archive_contents_test" )
		links( { "zdll", "UnRar", "LZMA" } )
		configuration( "windows" )
			targetdir( "bin/win32" )
			debugdir("bin/win32")
		configuration( "debug" )
			targetsuffix "_debug"
			flags( flags_debug )
			defines( definitions_debug )
			links( { "archive_static_debug" } )
		configuration("release")
			targetsuffix("_release")
			defines( definitions_release )
			flags( flags_release )
			links( { "archive_static" } )