-- A solution contains projects, and defines the available configurations
solution "regal"
  configurations { "Debug", "Release" }

  require "inc/regal" 
  require "inc/glu" 

  -- A project defines one build target
  project "regalStatic"
    targetname "Regal"
    kind "StaticLib"
    if os.is("macosx") and _ACTION == "xcode4" then
      buildoptions "-stdlib=libstdc++"
    end
    language "C++"
    files ( regalFiles )
    excludes ( regalExcludes )
    includedirs ( regalIncludes )
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
    
  -- A project defines one build target
  project "regalDynamic"
    targetname "Regal"
    kind "SharedLib"
    language "C++"
    files ( regalFiles )
    excludes ( regalExcludes )
    includedirs ( regalIncludes )
    if os.is("macosx") then
      files ( gluFiles )
      excludes( gluExcludes )
      includedirs( gluIncludes )
    end
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }    

