-- A solution contains projects, and defines the available configurations
solution "regal"
  configurations { "Debug", "Release" }
    
  -- A project defines one build target
  project "regal"
    kind "StaticLib"
    if os.is("macosx") then
      buildoptions "-stdlib=libstdc++"
    end
    language "C++"
    files { 
      "../../src/regal/**.h",
      "../../src/regal/**.cpp",
      "../../src/regal/**.c",
    }
    includedirs { 
      "../../include",
      "../../src/lookup3",
      "../../src/glsl/include",
      "../../src/glsl/src/glsl",
      "../../src/glsl/src/mesa"
    }
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }    
