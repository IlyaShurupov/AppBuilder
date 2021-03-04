import os as os
from os.path import relpath
import json
import shutil

def to_str(list, befor = False, separetor = ' ',  after = False, after_sep = ' '):
	string = ''
	for item in list:
		if befor and after:
			string = string + separetor + item + after_sep 
		else:
			if after:
				string = string + item + after_sep
			if befor:
				string = string + separetor + item

	return string

def RootDir(reponame):
	current = os.path.dirname(os.path.realpath(__file__))
	while True:
		names = current.rsplit("\\", 1) 
		if names[1] == reponame:
			return current
		current = names[0]
		if not len(current):
			return 0

def FindFiles(files, abs_path, type, must_include = False, name = ''):
	subfolders = os.listdir(abs_path + "\\")

	for i in range(len(subfolders)):
		if subfolders[i].rfind('.') != -1:
			if  subfolders[i].rsplit('.', 1)[1] == type:
				if must_include:
					if name in subfolders[i].rsplit('.', 1)[0]:
						files.append(abs_path + '\\' + subfolders[i])
				else:
					files.append(abs_path + '\\' + subfolders[i])

	for subfld in subfolders:
		if os.path.isdir(abs_path + '\\' + subfld):
			FindFiles(files, abs_path + '\\' + subfld, type, must_include, name)

class CProject():
	def __init__(this):
		this.name = ''
		this.dir = ''
		this.type = ''
		this.files = []
		this.incldirs = []
		this.libs = []
		this.libdirs = []


def ReadCProjectJson(file, Path):
	cproj = CProject();
	cproj.name = (file.rsplit('\\', 1)[0]).rsplit('\\', 1)[1]
	cproj.dir = Path['ROOT'] + '\\' + relpath(file.rsplit('\\', 1)[0] + '\\', Path['ROOT_ABS'])

	cppfiles = []
	FindFiles(cppfiles, file.rsplit('\\', 1)[0], 'cpp')
	for cppfile in cppfiles:
		cproj.files.append(cppfile.rsplit('.', 1)[0])

	with open(file) as json_file:
		data = json.load(json_file)
		cproj.type = data["Type"]
		cproj.incldirs = data["IncludePaths"]
		cproj.libs = data["Libraries"]
		cproj.libdirs = data["LibPaths"]

	print("		", cproj.name, "  ",  cproj.type)
	return cproj


def ReadProjects(dir, Cprojects, Path):
	print("\n -- Initializing Projects: ")
	files = []
	FindFiles(files, dir, 'json', True, 'cproject')
	for file in files:
		Cprojects.append(ReadCProjectJson(file, Path))

def SetBuildOrder(Cprojects):
	print("\n -- Seting Build Order:")

	str = '		'
	for proj in Cprojects:
		str = str + proj.name + " "

	print(str)


def ResolvePaths(PATH, Cprojects):
	for proj in Cprojects:
		for dir_idx in range(len(proj.incldirs)):
			dir = proj.incldirs[dir_idx]

			if dir.find('}') != 2:
				continue

			bool_add = len(dir) > 2
			pth  = dir.split('}')[1]

			if dir[0] == '{' and dir[2] == '}':
				if dir[1] == 'R':
					dir = PATH['ROOT']
				elif dir[1] == 'C':
					dir = proj.dir
				elif dir[1] == 'O':
					dir = PATH['OUT'] 
				
			if bool_add:
				dir = dir + '\\' + pth
			proj.incldirs[dir_idx] = dir

def CompileProjects(Cprojects, output):
	print("\n -- Compiling Projects Into ",  output, " : ")
	for proj in Cprojects:

		for file in proj.files:
			GenObj(file, proj.incldirs, output + "\\" + proj.name + "\\obj")

		if proj.type[0] == 'E':
			pass
		elif proj.type[0] == 'E':
			pass
		elif proj.type[0] == 'E':
			pass

def GenObj(file, incl, output):
	outfile = output + "\\" + file.rsplit('\\', 1)[1] + '.o'
	print("\n    Generating Object: " + file + ".cpp --> " +  outfile)
	if not os.path.isdir(output):
		os.makedirs(os.path.abspath(output))
	cmd = "g++ -c " + file + '.cpp '+ to_str(incl, True, ' -I') + ' -o ' + outfile
	print("	", cmd)
	os.system(cmd)

def PackObjs(files, outdir, name):
	print("Packing Objects: " + to_str(files, True, '.o ', False) + " --> " + outdir + '/' + name + '.o ' )
	os.system("ar rcs " + " " + outdir + '/' + name + '.o ' + to_str(files, '.o '))

def LinkObjs(name, files):
	print("Linking Objects: " + to_str(files, True) + " --> " + name + '.exe' )
	os.system("g++ -o " + name + to_str(files, False, ' ', True, '.o '))


Path = {}

Path['ROOT_ABS'] = RootDir('AppBuilder')
Path['OUT_ABS'] = Path['ROOT_ABS'] + "\\" + 'build/binaries'

Path['ROOT'] = relpath(Path['ROOT_ABS'], os.path.dirname(os.path.realpath(__file__)))
Path['OUTPUT'] = Path['ROOT'] + '\\build\\binaries'

Cprojects = []

os.system("cls")
if os.path.isdir(Path['OUT_ABS']):
	shutil.rmtree(Path['OUT_ABS'])

print("\n\n  -------- Build Started --------- ")

ReadProjects(Path['ROOT'], Cprojects, Path)
ResolvePaths(Path, Cprojects)


SetBuildOrder(Cprojects)

CompileProjects(Cprojects, Path['OUTPUT'])

