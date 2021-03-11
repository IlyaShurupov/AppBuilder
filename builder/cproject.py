
from common import *

class CProject():
	def __init__(this):
		
		this.name = ''
		this.dir = ''
		this.type = ''
		this.files = []
		this.projdeps = []
		this.externs = []
		
		this.rebuild = True
		this.incldirs = []
		this.libs = []
		this.libdirs = []

		this.defenitions = []

		this.flag = 0
		this.flag2 = 0

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