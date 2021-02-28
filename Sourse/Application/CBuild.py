
Import('cbld')

Includes = [cbld.path['ROOT'] + '/Sourse/Types', cbld.path['ROOT'] + '/Sourse/Application'] 

Files = [ "Application.cpp",
          "Object/private/Property.cpp",
          "Object/private/Object.cpp",
          "Operator/private/Operator.cpp",
          "Data/private/Seance.cpp",
          "Platform/private/Win32Window.cpp",
          "Platform/private/SysHandler.cpp" ,
          "UI/private/UInterface.cpp",
          "UI/private/UInputsMap.cpp"  ]

App = cbld.StaticLibrary('Application', Files, Includes, [], [])