solution "Dungeon Keeper"
 
    ------------------------------------------------------------------
    -- setup common settings
    ------------------------------------------------------------------
	defines 
	{ 
		"BOOST_ALL_NO_LIB", 
		"_CRT_SECURE_NO_WARNINGS",
        "_ELPP_NO_DEFAULT_LOG_FILE"
	}
	configurations 
	{ 
		"PriRel", 
		"PubRel" 
	}

	vectorextensions "SSE2"
	floatingpoint "Fast"
    
    includedirs 
    { 
        "../include/", 
	}
	
	location "projects"
	if os.is("windows") then
    links
    {
        "ws2_32",
        "winmm"
    }
    end
	
	flags { "Symbols" }
    
	configuration { "vs*"}
		buildoptions 
		{ 
			"/wd4512",
			"/wd4996",
			"/wd4018",
            "/Zm500"
		}
        defines
        {
            "WIN32"
        }
        
    configuration {"gmake"}
		linkoptions( "-lm -lpthread -pthread -Wl,--no-as-needed -lrt -g" ) 
		buildoptions {"-g --std=c++11 -fpermissive" } 
 
    configuration "PriRel"
        defines { "NDEBUG" }
        optimize "On"
		targetsuffix "_r"
		
	configuration "PubRel"
        defines { "NDEBUG", "PUBLIC_BUILD" }
        
        optimize "On"
		
	project "Game"
		kind "ConsoleApp"
		language "C++"
		targetdir "bin"
		includedirs 
           { 
               "include"
           }
		files 
           { 
               "include/**.h", 
               "src/**.cpp",
           }
	project "SimpleNet"
		kind "ConsoleApp"
		language "C++"
		targetdir "projects/simplenet/bin"
		includedirs
			{
				"projects/simplenet/include",
			}
		files
			{
				"projects/simplenet/include/**.h",
				"projects/simplenet/src/**.cpp",
			}