-- A solution contains projects, and defines the available configurations
solution "regal"
  configurations { "Debug", "Release" }
    
  -- A project defines one build target
  project "regal"
    kind "StaticLib"
    language "C++"
    files { "src/regal/**.h", "src/regal/**.cpp" }
    includedirs { 
      "include",
      "src/glsl/include",
      "src/glsl/src/glsl",
      "src/glsl/src/mesa"
    }
                          
    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
                                                    
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }    
