
import os
import sys


def rebuild():
	os.system("MSBuild.exe  ./config/Nodes.sln /t:Clean,Build /p:Configuration=Release /p:Platform=x64 ")   


exec(sys.argv[1])