
import sys
import os
from pathlib import Path

SolutionDir = Path(sys.argv[1])
OutputDir = Path(sys.argv[2])
ProjectName = sys.argv[3]

exe_path = str(OutputDir.absolute()) + "\\" + ProjectName + "\\"
libs_path = exe_path + "lib"
root = str(SolutionDir.absolute()) + "\\..\\.."

# Creating Lib Directory
os.system("mkdir " + libs_path)

# Copying libtcc into lib dir
os.system("copy " + root + "\\ext\\tcc\\lib\\libtcc1-64.a " + libs_path + " /Y ")
os.system("copy " + root + "\\ext\\tcc\\libtcc.dll " + exe_path + " /Y ")

# Copying Application.dll into output dir
os.system("copy " + str(OutputDir.absolute()) + "\\Application\\Application.dll " + exe_path + " /Y")
os.system("copy " + str(OutputDir.absolute()) + "\\Application\\Application.lib " + libs_path + " /Y")



source_path = Path(root + "\\source");

def copy_includes(crnt_path):
	for path in crnt_path.iterdir():
		if path.is_file():
			if path.suffix == ".h":
				outpath = str(OutputDir.absolute()) + "\\" + ProjectName + "\\include\\" + str(path.relative_to(source_path))
				
				if Path(outpath).is_file() and Path(outpath).stat().st_mtime >= path.stat().st_mtime:
					continue

				os.system("xcopy " + str(path.absolute()) + " " + str(Path(outpath).parents[0].absolute()) + "\\ /Y")
		else:
			copy_includes(path)
			

copy_includes(source_path)


os.system("xcopy " + root + "\\ext\\compiler\\ " + exe_path + "\\compiler\\ /E /H /C /I")