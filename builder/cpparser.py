
class ExeptionFailedParseCP(Exception):
	pass

from common import * 
from cproject import *

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

	cproj = CProject();
	cproj.name = (file.rsplit('\\', 1)[0]).rsplit('\\', 1)[1]
	cproj.dir = Path['ROOT'] + '\\' + os.path.relpath(file.rsplit('\\', 1)[0] + '\\', os.path.abspath(Path['ROOT']))

	cppfiles = []
	FindFiles(cppfiles, file.rsplit('\\', 1)[0], 'cpp')

	for cppfile in cppfiles:
		cproj.files.append(cppfile.rsplit('.', 1)[0])

	with open(file) as json_file:
		data = json.load(json_file)


		cproj.type = data["Type"]

		if "IncludePaths" in data:
			cproj.incldirs = data["IncludePaths"]
		
		if "LibPaths" in data:
			cproj.libdirs = data["LibPaths"]

		cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + cproj.name)
		cproj.libdirs.append(Path['OUTPUT'] + "\\" + cproj.name)

		if "InternLibs" in data:
			for intlib in data["InternLibs"]:
				cproj.libs.append(intlib + ".lib")
				cproj.libdirs.append(Path['OUTPUT'] + "\\" + intlib)		
				cproj.incldirs.append(Path['ROOT'] + "\\sourse\\" + intlib)
				cproj.projdeps.append(intlib)

		libs = []
		if "Libraries" in data:
			libs = data["Libraries"]
			cproj.libdirs.append(Path['EXTERNS'])
		cproj.libs = cproj.libs + libs
		cproj.externs += libs 

	return cproj

def ReadSolution(dir, Cprojects, Path):
	print(" -- Reading Projects ")

	files = []
	FindFiles(files, dir, 'json', True, 'cproject')
	for file in files:
		Cprojects.append(ReadCProjectJson(file, Path))
	ResolvePaths(Path, Cprojects)

	ResolveDeps(Cprojects)
	inhirent_from_deps(Cprojects)

def save_to_json(data, name, absout):
	if not os.path.isdir(absout):
		os.makedirs(absout)
	with open(absout + "\\" + name + ".json", 'w+') as outfile:
	    json.dump(data, outfile)

def load_from_json(absfile):
	data = {}
	if os.path.isfile(absfile):
		if absfile.rsplit('.', 1)[1] == "json":
			with open(absfile) as json_file:
				data = json.load(json_file)
	return data


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

	cahefile = os.path.dirname(__file__) + '\\cache.json'

	if not os.path.isfile(cahefile):
		with open('cache.json', 'w+') as json_file:
			data["LastRun"] = 0
			data["Projects"] = []
			json.dump(data, json_file)

	with open(cahefile) as json_file:
		data = json.load(json_file)
	return data