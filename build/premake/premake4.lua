-- A solution contains projects, and defines the available configurations
solution "regal"
  configurations { "Debug", "Release" }

  require "inc/regal" 

  -- A project defines one build target
  project "regalStatic"
    targetname "regal"
    kind "StaticLib"
    if os.is("macosx") then
      buildoptions "-stdlib=libstdc++"
    end
    language "C++"
    files ( regalFiles )
    excludes ( regalExcludes )
    includedirs ( regalInc )
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
    
  -- A project defines one build target
  project "regalDynamic"
    targetname "regal"
    kind "SharedLib"
    if os.is("macosx") then
      -- buildoptions "-stdlib=libstdc++"
    end
    language "C++"
    files ( regalFiles )
    excludes ( regalExcludes )
    includedirs ( regalInc )
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }    

