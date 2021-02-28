
Import('cbld')

Includes = [cbld.path['ROOT'] + '\Sourse\Types'] 

Files = [ "Geometry/private/Ray.cpp",
          "Geometry/private/Mesh.cpp",
          "Geometry/private/Trig.cpp",
          "Time/private/Timer.cpp",
          "Memory/private/Mem.cpp",
          "IO/private/Parser.cpp" ,
          "Strings/private/Strings.cpp" ]

Types = cbld.StaticLibrary('Types', Files, Includes, [], [])
