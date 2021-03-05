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
		this.flag = 0
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

		cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + cproj.name)
		cproj.libdirs.append(Path['OUTPUT'] + "\\" + cproj.name)

		for intlib in data["InternLibs"]:
			cproj.libs.append(intlib + ".lib")
			cproj.libdirs.append(Path['OUTPUT'] + "\\" + intlib)		
			cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + intlib)


	print("		", cproj.name, "  ",  cproj.type)
	return cproj


def ReadProjects(dir, Cprojects, Path):
	print("\n -- Reading Projects From  " + os.path.abspath(dir))
	files = []
	FindFiles(files, dir, 'json', True, 'cproject')
	for file in files:
		Cprojects.append(ReadCProjectJson(file, Path))

def SetBuildOrder(Cprojects):
	print("\n -- Seting Build Order:")
    
	bool_all_checked = False
	proj_idx = 0

	for proj_idx in range(len(Cprojects)):
		Cprojects[proj_idx].flag = 0

	while not bool_all_checked:
	
		if Cprojects[proj_idx].flag == 0:

			deps = Cprojects[proj_idx].libs.copy()
			for i in range(len(deps)):
				deps[i] = deps[i].split('.')[0]

			for dep_proj_idx in range(len(Cprojects)):
				if Cprojects[dep_proj_idx].name in deps:
					if dep_proj_idx > proj_idx:
						Cprojects[proj_idx], Cprojects[dep_proj_idx] = Cprojects[dep_proj_idx], Cprojects[proj_idx]

			Cprojects[proj_idx].flag = 1
			proj_idx = 0
		else:
			if proj_idx + 1 == len(Cprojects):
				bool_all_checked = True	
			proj_idx = proj_idx + 1

	
	str = '		'
	for proj in Cprojects:
		str = str + proj.name + " "

	print(str)


def ResolvePaths(PATH, Cprojects):

	def subf(dirs):
		for dir_idx in range(len(dirs)):

			dir = dirs[dir_idx]

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
					dir = PATH['OUTPUT'] 
				
			if bool_add:
				dir = dir + '\\' + pth
			dirs[dir_idx] = dir

	for proj in Cprojects:
		subf(proj.incldirs)
		subf(proj.libdirs)


def CompileProjects(Cprojects, output):
	print("\n -- Compiling Projects Into ",  os.path.abspath(output))
	for proj in Cprojects:
		print("\n", proj.name)

		for i in range(len(proj.files)):
			file = proj.files[i]
			outfile = output + "\\" + proj.name + "\\obj" + '\\' + file.rsplit('\\', 1)[1]
			GenObj(file, proj.incldirs, output + "\\" + proj.name + "\\obj")
			proj.files[i] = outfile

		if proj.type[0] == 'E':

			PackObjs(proj.files, output + "\\" + proj.name, proj.name)

			linkfiles = [proj.name + ".lib"] + proj.libs

			for i in range(len(linkfiles)):
				linkfiles[i] = linkfiles[i].split('.')[0]

			linkfiles[1], linkfiles[2] = linkfiles[2], linkfiles[1]
			LinkObjs(proj.name, output, linkfiles, proj.libdirs)

		elif proj.type[0] == 'S':
			PackObjs(proj.files, output + "\\" + proj.name, proj.name)
		elif proj.type[0] == 'D':
			pass

def GenObj(file, incl, output):
	outfile = output + "\\" + file.rsplit('\\', 1)[1] + '.o'
	print("     Gen Obj: " +  file.rsplit('\\', 1)[1] + '.o')
	if not os.path.isdir(output):
		os.makedirs(os.path.abspath(output))
	cmd = "g++ -c " + file + '.cpp '+ to_str(incl, True, ' -I') + ' -o ' + outfile
	os.system(cmd)

def PackObjs(files, outdir, name):
	cmd = "ar rcs " + outdir + '/' + name + '.lib ' + to_str(files, False, ' ', True, '.o ')
	print("\n     Packing Objects Into ", name + "\\" + name + '.lib ')
	os.system(cmd)

def LinkObjs(name, output, objs, objpaths):
	print("     Linking: " + to_str(objs, True) + " --> " + name + '.exe' )
	outfile = " -o " + output + "\\" + name + "\\" + name
	libstr = to_str(objs, True, " -l")
	libpathsstr = to_str(objpaths, True, " -L")
	cmd = "g++ -static " + libpathsstr + libstr + outfile
	print(cmd)
	os.system(cmd)


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

