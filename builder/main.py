import os as os
from os.path import relpath
import shutil
import sys
import  compiler
import cparser 
import commands




class CProject():
	def __init__(this):
		this.rebuild = True
		this.flag = 0
		this.name = ''
		this.dir = ''
		this.type = ''
		this.files = []
		this.incldirs = []
		this.libs = []
		this.libdirs = []

class Env():
	def __init__(this):
		this.rebuild = False
		this.OutDir = 'build\\binaries'
		this.debug = False
		this.rebuild_type = "tree"
		this.projs = []
		this.changed_files = []
		this.RootDirName = os.path.dirname(__file__).rsplit('\\', 1)[1]
		this.path = {}


class Builder():

	def __init__(this, env = Env()):
		this.env = env
		this.commands = []

	def LogEnv(this):
		this.Logout("        RootDirName    " + " '" + this.env.RootDirName + "' ")
		this.Logout("        OutDir         " + this.env.OutDir)
		this.Logout("        debug          " + str(this.env.debug))
		this.Logout("        rebuild_type   " + this.env.rebuild_type)
		this.Logout("        rebuild        " + str(this.env.rebuild))

	def Build(this, args):

		if this.env.rebuild and os.path.isfile(os.path.abspath("cache.json")):
			print("remove")
			os.remove(os.path.abspath("cache.json"))


		this.env.path['ROOT'] = RootDir(this.env.RootDirName)
		this.env.path['OUTPUT'] = this.env.OutDir

		this.Logout(" -- Build started")

		this.env.projs.clear()
		cparser.ReadSolution(this.env.path['ROOT'], this.env.projs, this.env.path)
	
		if not len(this.env.projs):
			this.Logout("No Cprojects found in working dir: '" + this.env.path['ROOT'] + "'", "warning")
			raise commands.ExeptionTerminated

		if os.path.isdir(os.path.abspath(this.env.path['OUTPUT'])):
			pass
			#shutil.rmtree(os.path.abspath(this.path['OUTPUT']))

		this.FindModified()
		this.CalcDepTree()
		this.CompileObjects()
		this.PackObjects()
		this.LinkObjects()
		cparser.SaveCache(this)

	def FindModified(this):
		print(" -- Finding Modified Files")
		this.env.changed_files.clear()
		is_build = False
		cache = cparser.getCache()
		lastcycletime = cache["LastRun"]
		

		for proj in this.env.projs:
			proj.rebuild = False
	
			files = []
			cparser.FindFiles(files, proj.dir, 'cpp')
			cparser.FindFiles(files, proj.dir, 'h')

			for file in files:
				if os.path.getmtime(file) > lastcycletime:
					this.env.changed_files.append(file)
					proj.rebuild = True
					is_build = True	
					break

		if is_build:
			return

		print("       Not Found. Terminating ")
		raise commands.ExeptionTerminated

	def CheckRebuild(this):
		print("\n -- Determinding Objects to be Rebuilded:")
		
		if this.env.rebuild_type == "tree":
			pass
		elif this.env.rebuild_type == "fl":
			pass
		elif this.env.rebuild_type == "prj":
			pass

	def CalcDepTree(this):
		print(" -- Defining Dependency Tree")
	    
		bool_all_checked = False
		proj_idx = 0

		for proj_idx in range(len(this.env.projs)):
			this.env.projs[proj_idx].flag = 0

		while not bool_all_checked:
		
			if this.env.projs[proj_idx].flag == 0:

				deps = this.env.projs[proj_idx].libs.copy()
				for i in range(len(deps)):
					deps[i] = deps[i].split('.')[0]

				for dep_proj_idx in range(len(this.env.projs)):
					if this.env.projs[dep_proj_idx].name in deps:
						if dep_proj_idx > proj_idx:
							this.env.projs[proj_idx], this.env.projs[dep_proj_idx] = this.env.projs[dep_proj_idx], this.env.projs[proj_idx]

				this.env.projs[proj_idx].flag = 1
				proj_idx = 0
			else:
				if proj_idx + 1 == len(this.env.projs):
					bool_all_checked = True	
				proj_idx = proj_idx + 1

	def CompileObjects(this):
		output = this.env.path['OUTPUT']
		print(" -- Compiling Objects")

		for proj in this.env.projs:

			reb_files = []
			if this.env.rebuild_type == "fl":
				for ch_fl in this.env.changed_files:
					if ch_fl.rsplit('.', 1)[1] == "cpp":
						cppfile = ch_fl.rsplit('.', 1)[0]
						if cppfile in proj.files:
							reb_files.append(cppfile)
			else:
				reb_files = reb_files + proj.files
			

			if not len(reb_files) or not proj.rebuild:
				continue

			print("\n ", proj.name)

			for i in range(len(reb_files)):
				file = reb_files[i]
				outfile = output + "\\" + proj.name + "\\obj" + '\\' + file.rsplit('\\', 1)[1]		
				print("	" +  file.rsplit('\\', 1)[1] + '.o')
				compiler.GenObj(file, proj.incldirs, output + "\\" + proj.name + "\\obj", this.env.debug)

			for i in range(len(proj.files)):
				proj.files[i] = output + "\\" + proj.name + "\\obj" + '\\' + proj.files[i].rsplit('\\', 1)[1]

	def PackObjects(this):
		output = this.env.path['OUTPUT']
		print("\n\n -- Packing Objects")

		for proj in this.env.projs:
			if not proj.rebuild: continue
			print("     ", proj.name)
			compiler.PackObjs(proj.files, output + "\\" + proj.name, proj.name)

	def LinkObjects(this):
		output = this.env.path['OUTPUT']
		print("\n\n -- Linking Objects")

		for proj in this.env.projs:
			if not proj.type == "Executable": continue
			print("     ", proj.name)
			
			linkfiles = [proj.name + ".lib"] + proj.libs
			for i in range(len(linkfiles)):
				linkfiles[i] = linkfiles[i].split('.')[0]

			compiler.LinkObjs(proj.name, output, linkfiles, proj.libdirs, this.env.debug)

	def Logout(this, text = '', type = 'comment'):
		
		color = "\033[0;0m"
		type_log = ''

		if type == 'error':		
			color = "\033[1;31m"
			type_log = '  ERROR: '
		elif type == 'warning':		
			type_log = '  WARNING: '
			color = "\033[1;36m"

		print(type_log + text)

		if type == 'error':
			print(" ---- Build terminated Because Of Errors ---- ")
			raise commands.ExeptionBuildError


def RootDir(reponame):
	current = os.path.dirname(os.path.realpath(__file__))
	while True:
		names = current.rsplit("\\", 1) 
		if names[1] == reponame:
			return current
		current = names[0]
		if not len(current):
			return 0

def getArgs(command):
	args = []
	if command.find(':') >= 0:
		args += command.split(':', 1)[1].split(' ')
		arglen = len(args)
		i = 0
		while i < arglen:
			if args[i] == '':
				args.remove(args[i])
				arglen -= 1
			i += 1
	return args

def ProcCommands(bld):

	while True:

		print("(bld) < ", end=" ")
		usrinput = input()
		
		usrcommand = usrinput
		args = getArgs(usrinput)

		if usrinput.find(":") > 0: 
			usrcommand = usrinput.split(":", 1)[0]

		usrcommand = usrcommand.lstrip()
		usrcommand = usrcommand.rstrip()

		found = False
		for command in bld.commands:
			if command.id == usrcommand:
				found = True

				try:
					command.Exec(bld, args, usrinput)
				except (commands.ExeptionBuildError, commands.ExeptionTerminated) as err:
					command.OnError(args)

		if not found:
			print(" command '" + usrcommand + "' not found. try 'help' ")

		print("")


if __name__ == "__main__":
	print(" \n\n ---------------- Builder ------------------------- \n\n ")
	bld = Builder()
	commands.init_comands(bld)
	ProcCommands(bld)


else:
	bld = Builder()
