definitions_debug = { "DEBUG" }
definitions_release = { "RELEASE" }

flags_debug = { "Symbols", "StaticRuntime" }
flags_release = { "OptimizeSpeed", "StaticRuntime" }

solution( "archiver_project" )
	configurations( { "debug", "release" } )
	includedirs( {"include","include/python","src","tests"} )
	platforms( {"x32","x64"} )
	configuration( "x64" )
		defines( "USE_64BIT" )
	configuration( "x32" )
		defines( "USE_32BIT" )
	configuration( { "windows", "debug" } )
		libdirs( { "lib/win32/debug","lib/win32" } )
	configuration( { "windows", "release" } )
		libdirs( { "lib/win32/release","lib/win32" } )
	configuration ""
	location("build")
	
	project( "test_suite" )
		kind( "ConsoleApp" )
		language( "C++" )
		files( { "tests/**"} )
		location( "build/test_suite" )
		links( { "gmock", "gmock_main" } )
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

	project( "archiver" )
		kind( "ConsoleApp" )
		language( "C++" )
		files( { "src/**"} )
		location( "build/archiver" )
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