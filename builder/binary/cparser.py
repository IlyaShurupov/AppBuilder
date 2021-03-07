
import os as os
import json

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

def ReadCProjectJson(file, Path):
	import compile
	cproj = compile.CProject();
	cproj.name = (file.rsplit('\\', 1)[0]).rsplit('\\', 1)[1]
	cproj.dir = Path['ROOT'] + '\\' + os.path.relpath(file.rsplit('\\', 1)[0] + '\\', Path['ROOT_ABS'])

	cppfiles = []
	FindFiles(cppfiles, file.rsplit('\\', 1)[0], 'cpp')

	for cppfile in cppfiles:
		cproj.files.append(cppfile.rsplit('.', 1)[0])

	with open(file) as json_file:
		data = json.load(json_file)
		cproj.type = data["Type"]
		cproj.incldirs = data["IncludePaths"]
		
		cproj.libdirs = data["LibPaths"]

		cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + cproj.name)
		cproj.libdirs.append(Path['OUTPUT'] + "\\" + cproj.name)
		cproj.libdirs.append(Path['ROOT'] + "\\extern\\lib")

		for intlib in data["InternLibs"]:
			cproj.libs.append(intlib + ".lib")
			cproj.libdirs.append(Path['OUTPUT'] + "\\" + intlib)		
			cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + intlib)
		cproj.libs = cproj.libs + data["Libraries"]

	print("		", cproj.name, "  ",  cproj.type)
	return cproj


def ReadSolution(dir, Cprojects, Path):
	print("\n -- Reading Projects From  " + os.path.abspath(dir))

	with open('cache.json') as jsonfile:
		cache = json.load(jsonfile)

	files = []
	FindFiles(files, dir, 'json', True, 'cproject')
	for file in files:
		Cprojects.append(ReadCProjectJson(file, Path))
	ResolvePaths(Path, Cprojects)

def getLastCycleTime():
	timefilepath = os.path.dirname(__file__) + "\\time"
	time = 0
	with open('time', 'w+') as time:
		time = os.path.getmtime(timefilepath)
	os.remove(timefilepath)
	return time

def SaveCache(Builder):
	cache = {}
	cache["LastRun"] = getLastCycleTime()
	projs = []
	cache["Projects"] = projs

	with open('cache.json', 'w+') as outfile:
	    json.dump(cache, outfile)

def getCache():
	data = {}
	with open(os.path.dirname(__file__) + "\\cache.json") as json_file:
		data = json.load(json_file)
	return data