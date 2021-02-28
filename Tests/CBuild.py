
Import('cbld')

Includes = [cbld.path['ROOT'] + '\Sourse\Types'] 
Files = ['Test.cpp']

Libs = ['Types']
LibsPath = [cbld.path['OUT_ABS'] + '\Sourse\Types']

exe = cbld.Executable('Tests', Files, Includes, Libs, LibsPath)

