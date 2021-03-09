
class ExeptionFailedParseCP(Exception):
	pass

import os as os
import json
from main import CProject

def find_dep(name, projs):
		for proj in projs:
			if proj.name == name:
				return proj
		raise ExeptionFailedParseCP

def inhirent_from_deps(projs):

	for proj in projs: proj.flag = 0

	def inhirent(proj):
		if proj.flag: return 

		for dep in proj.projdeps:
			if len(dep.projdeps) : inhirent(dep)
			
			proj.flag = 1
			proj.incldirs += dep.incldirs
			proj.libs += dep.libs
			proj.libdirs += dep.libdirs		

	for proj in projs: inhirent(proj)


def get_root_objs(projs):

	out = []

	for proj in projs: proj.flag = 0

	for proj in projs:
		for dep in proj.projdeps: 
			dep.flag = 1

	for proj in projs:
		if not proj.flag:
			out.append(proj)

	return out

def get_build_order(projs, project):

	levels = {}

	for proj in projs: 
		proj.flag = 0
		proj.flag2 = 0

	def mark_dep_level(proj):
		proj.flag2 = 1
		for dep in proj.projdeps:
			if dep.flag <= proj.flag:
				dep.flag = proj.flag + 1
			mark_dep_level(dep)
			
	mark_dep_level(project)


	levels['0'] = [project]
	for proj in projs:
		if proj.flag2:
			if proj.flag in levels:
				levels[str(proj.flag)].append(proj)
			else:
				levels[str(proj.flag)] = [proj]

	max_level = 0
	for proj in projs: 
		if proj.flag > max_level: max_level = proj.flag

	out = [] 
	for i in range(max_level + 1):
		out += levels[str(i)]

	return out


def ResolveDeps(projs):
	print(" -- Defining Dependency Tree")
	for proj in projs:
		resoleved = []
		for dep_idx in range(len(proj.projdeps)):
			resoleved.append(find_dep(proj.projdeps[dep_idx], projs))
		
		proj.projdeps.clear()
		proj.projdeps += resoleved
		

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