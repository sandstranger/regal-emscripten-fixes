-- A solution contains projects, and defines the available configurations
solution "regal"
  configurations { "Debug", "Release" }

  local regalFiles = { 
    "../../src/regal/**.h",
    "../../src/regal/**.cpp",
    "../../src/regal/**.c",
    "../../src/glsl/**.c",
    "../../src/glsl/**.cpp"
  }

  local regalExcludes = {
    "../../src/glsl/**/Main.cpp",
    "../../src/glsl/**/main.cpp",
    "../../src/glsl/**/builtin_compiler/**"
  }

  local regalInc = {
    "../../include",
    "../../src/regal",
    "../../src/lookup3",
    "../../src/glsl/include",
    "../../src/glsl/src/glsl",
    "../../src/glsl/src/mesa"
  }
    
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

