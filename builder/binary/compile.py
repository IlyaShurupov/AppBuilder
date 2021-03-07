import os as os
from os.path import relpath
import shutil
import sys
import  compiler
import cparser 



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

class Builder():

	def __init__(this, args = []):
		this.OutDir = 'build\\binaries'
		this.debug = False
		this.rebuild_type = "tree"

		this.path = {}
		this.projs = []
		this.changed_files = []

		this.RootDirName = os.path.dirname(__file__).rsplit('\\', 1)[1]
		this.path['ROOT_ABS'] = RootDir(this.RootDirName)

		this.ProcArgs(args)

	def ProcArgs(this, args):
			
			for arg in args:

				if arg == "debug":
					this.debug = True

				elif arg == "rebld":
					cahefile = os.path.dirname(__file__) + '\\cache.json'
					if os.path.isfile(cahefile): os.remove(cahefile)

				elif arg[0] == "-":

					if arg[1] == "o":
						this.OutDir = arg.split('o', 1)[1]

					elif arg[1] == "r":
						this.RootDirName = arg.split("r", 1)[1]

					elif arg[1] == "t":
						this.rebuild_type = arg.split("t", 1)[1]

					else:
						this.Logout(" Cant resolve argument '" + arg + "'", "warning")

				else:
					this.Logout(" Cant resolve argument '" + arg + "'", "warning")

			this.path['ROOT_ABS'] = RootDir(this.RootDirName)
			this.path['ROOT'] = relpath(this.path['ROOT_ABS'], os.path.dirname(os.path.realpath(__file__)))
			this.path['OUTPUT'] = this.path['ROOT'] + "\\" + this.OutDir

	def Build(this, args):

		this.ProcArgs(args)

		this.projs.clear()
		cparser.ReadSolution(this.path['ROOT'], this.projs, this.path)
	
		if not len(this.projs):
			this.Logout("No Cprojects found in working dir: '" + this.path['ROOT'] + "'", "warning")
			1 / 0

		if os.path.isdir(os.path.abspath(this.path['OUTPUT'])):
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
		this.changed_files.clear()
		is_build = False
		cache = cparser.getCache()
		lastcycletime = cache["LastRun"]
		

		for proj in this.projs:
			proj.rebuild = False
	
			files = []
			cparser.FindFiles(files, proj.dir, 'cpp')
			cparser.FindFiles(files, proj.dir, 'h')

			for file in files:
				if os.path.getmtime(file) > lastcycletime:
					this.changed_files.append(file)
					proj.rebuild = True
					is_build = True	
					break

		if is_build:
			return

		print("       Not Found. Terminating ")
		1 / 0

	def CheckRebuild(this):
		print("\n -- Determinding Objects to be Rebuilded:")
		
		if this.rebuild_type == "tree":
			pass
		elif this.rebuild_type == "fl":
			pass
		elif this.rebuild_type == "prj":
			pass

	def CalcDepTree(this):
		print(" -- Defining Dependency Tree")
	    
		bool_all_checked = False
		proj_idx = 0

		for proj_idx in range(len(this.projs)):
			this.projs[proj_idx].flag = 0

		while not bool_all_checked:
		
			if this.projs[proj_idx].flag == 0:

				deps = this.projs[proj_idx].libs.copy()
				for i in range(len(deps)):
					deps[i] = deps[i].split('.')[0]

				for dep_proj_idx in range(len(this.projs)):
					if this.projs[dep_proj_idx].name in deps:
						if dep_proj_idx > proj_idx:
							this.projs[proj_idx], this.projs[dep_proj_idx] = this.projs[dep_proj_idx], this.projs[proj_idx]

				this.projs[proj_idx].flag = 1
				proj_idx = 0
			else:
				if proj_idx + 1 == len(this.projs):
					bool_all_checked = True	
				proj_idx = proj_idx + 1

	def CompileObjects(this):
		output = this.path['OUTPUT']
		print("\n\n -- Compiling Objects")

		for proj in this.projs:

			reb_files = []
			if this.rebuild_type == "fl":
				for ch_fl in this.changed_files:
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
				compiler.GenObj(file, proj.incldirs, output + "\\" + proj.name + "\\obj", this.debug)

			for i in range(len(proj.files)):
				proj.files[i] = output + "\\" + proj.name + "\\obj" + '\\' + proj.files[i].rsplit('\\', 1)[1]

	def PackObjects(this):
		output = this.path['OUTPUT']
		print("\n\n -- Packing Objects")

		for proj in this.projs:
			if not proj.rebuild: continue
			print("     ", proj.name)
			compiler.PackObjs(proj.files, output + "\\" + proj.name, proj.name)

	def LinkObjects(this):
		output = this.path['OUTPUT']
		print("\n\n -- Linking Objects")

		for proj in this.projs:
			if not proj.type == "Executable": continue
			print("     ", proj.name)
			
			linkfiles = [proj.name + ".lib"] + proj.libs
			for i in range(len(linkfiles)):
				linkfiles[i] = linkfiles[i].split('.')[0]

			compiler.LinkObjs(proj.name, output, linkfiles, proj.libdirs, this.debug)

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
			1 / 0


def RootDir(reponame):
	current = os.path.dirname(os.path.realpath(__file__))
	while True:
		names = current.rsplit("\\", 1) 
		if names[1] == reponame:
			return current
		current = names[0]
		if not len(current):
			return 0

def main():
	#os.system("cls")
	print(" \n\n ---------------- Builder ------------------------- \n\n ")

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

	bld = Builder([])

	while True:
		print("(bld) < ", end=" ")
		command = input()


		if command == "q":
			exit(0)

		elif command.find("bld") == 0 or command.find("rebld") == 0:
			if command.find("rebld") == 0:
				cahefile = os.path.dirname(__file__) + '\\cache.json'
				if os.path.isfile(cahefile): 
					os.remove(cahefile)
			
			try:
				bld.Build(getArgs(command))
			except ZeroDivisionError:  
				pass

		elif command.find("dbg") == 0:
			args = getArgs(command)
			if os.path.isfile(args[0]):
				os.system("gdb " + args[0])
			else:
				exe = args[0]
				files = []
				cparser.FindFiles(files, os.path.abspath(bld.path['OUTPUT']), 'exe', True, args[0])
				if not len(files):
					print(" exe not found in bld root path ")
				else:
					if len(files) > 1:
						bld.Logout(" Ambigues Executable Name - Enter full path", "warning")
					else:
						os.system("gdb " + files[0])

		elif command.find("os") == 0:
			os.system(command.split(':', 1)[1])

		elif command.find("eval") == 0:
			eval(command.split(':', 1)[1])
		
		elif command.find("args") == 0:
			bld.ProcArgs(getArgs(command))
		
		elif command.find("help") == 0:
			print("  in progress")

		elif command.find("run") == 0:
			args = getArgs(command)
			if os.path.isfile(args[0]):
				os.system("gdb " + args[0])
			else:
				exe = args[0]
				files = []
				cparser.FindFiles(files, os.path.abspath(bld.path['OUTPUT']), 'exe', True, args[0])
				if not len(files):
					print(" exe not found in bld root path ")
				else:
					if len(files) > 1:
						bld.Logout(" Ambigues Executable Name - Enter full path", "warning")
					else:
						os.system(files[0])

		else:
			print("  Command Not Found")


		print("\n")


if __name__ == "__main__":
	main()
else:
	bld = Builder()
