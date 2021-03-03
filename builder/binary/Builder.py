
import SCons.Script
from SCons.Environment import Environment
import json
import os as os


def RootDir(reponame):
	current = Dir('.').srcnode().abspath	
	while True:
		names = current.rsplit("\\", 1) 
		if names[1] == reponame:
			return current
		current = names[0]
		if not len(current):
			return 0


def ReadCProjectJson(file):
	with open(file) as json_file:
		data = json.load(json_file)
		cproj = CProject();

		cproj.dir = file.rsplit('\\', 1)[0] + '\\'
		cproj.name = (file.rsplit('\\', 1)[0]).rsplit('\\', 1)[1]
		cproj.type = data["Type"]
		cproj.files = data["Files"]
		cproj.incldirs = data["IncludePaths"]
		cproj.libs = data["Libraries"]
		cproj.libdirs = data["LibPaths"]

		return cproj

def ReadProjects(dir, Cprojects):
	subfolders = os.listdir(dir + "\\")
	for i in range(len(subfolders)):
		subfolders[i] = dir + "\\" + subfolders[i] 

	for subfld in subfolders:
		if subfld.rsplit('\\', 1)[1] == 'cproject.json':
			Cprojects.append(ReadCProjectJson(subfld))
			return

	for subfld in subfolders:
		if os.path.isdir(subfld):
			ReadProjects(subfld, Cprojects)

def SetBuildOrder(Cprojects):
	pass

def AbsPaths(PATH, Cprojects):

	def sub_res(proj, dir, PATH):
		
		if dir.find(".") != 1:
			return
		
		bool_add = len(dir) > 2
		pth  = dir.rsplit('.')[1]

		if dir[0] == 'R':
			dir = PATH['ROOT']
		elif dir[0] == 'C':
			dir = proj.dir
		elif dir[0] == 'O':
			dir = PATH['OUT_ABS'] 
		
		if bool_add:
			dir = dir + '\\' + pth

		return dir

	for proj in Cprojects:

		for dir_idx in range(len(proj.files)):
			proj.files[dir_idx] = proj.dir + proj.files[dir_idx]

		for dir_idx in range(len(proj.incldirs)):
			proj.incldirs[dir_idx] = sub_res(proj, proj.incldirs[dir_idx], PATH)

		for dir_idx in range(len(proj.libdirs)):
			proj.incldirs[dir_idx] = sub_res(proj, proj.incldirs[dir_idx], PATH)



def CompileProjects(Cprojects, outdir, fromdir):

	print(" -- Compiling Projects")

	for proj in Cprojects:
		env = Environment(CPPPATH=proj.incldirs, LIBS=proj.libs, LIBPATH=proj.libdirs, CCCOMSTR = "Compiling $TARGET", LINKCOMSTR = "Linking $TARGET")
		if proj.type == 'Executable': 
			env.Program(proj.name, sourse=proj.files)
			
		elif proj.type == 'StaticLibrary': 
			env.StaticLibrary(proj.name, sourse=proj.files)


class CProject():
	name = ''
	dir = ''
	type = ''
	files = []
	incldirs = []
	libs = []
	libdirs = []




Path = {}
Path['ROOT'] = RootDir('AppBuilder')
Path['OUTPUT'] = 'build/binaries'
Path['OUT_ABS'] = Path['ROOT'] + "\\" + 'build/binaries'
Path['SRS_ROOT'] = Path['ROOT'] + '/sourse'

Cprojects = []


print("\n Output directory: " + Path['OUT_ABS'] )
print(" -- Reading Projects ")
ReadProjects(Path['ROOT'], Cprojects)
AbsPaths(Path, Cprojects)


SetBuildOrder(Cprojects)

CompileProjects(Cprojects, '..\\..\\build', '..\\..\\')

