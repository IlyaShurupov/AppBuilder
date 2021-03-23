
import  compiler
import commands
from cpparser import *
from common import * 
from common import __FPFILE__
from common import SPL
from cproject import *

class Env():
	def __init__(this):
		this.name = 'default'
		this.RootDirName = os.path.dirname(__FPFILE__).rsplit(SPL, 1)[1]
		this.type = 'binaries'
		this.platform = 'windows'
		this.sysarch = '64'
		this.OutDir = 'build'+SPL+'binaries'
		this.debug = False
		this.rebuild_type = "tree"
		this.threading = False

	def Log(this):
		names = list(this.__dict__.keys())
		for i in range(len(names)):
			print("      " +  names[i] + " - " + str(this.__dict__[names[i]]))
		print(" ")


class Builder():

	def __init__(this, env = Env()):
		this.envs = [] 
		this.env = env
		this.commands = []
		this.envs.append(env)
		this.changed_files = []
		this.projs = []
		this.path = {}

	def RunInteractiveMode(this):

		commands.init_comands(this)

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
			for command in this.commands:
				if command.id == usrcommand:
					found = True

					try:
						command.Exec(this, args, usrinput)
					except (commands.ExeptionBuildError, commands.ExeptionTerminated) as err:
						command.OnError(args)

			if not found:
				print(" command '" + usrcommand + "' not found. try 'help' ")

			print("")

	def Build(this, args):
		this.Logout(" -- Build started")
		

		this.path['ROOT'] = RootDir(this.env.RootDirName)
		this.path['OUTPUT'] = this.path['ROOT'] + SPL + this.env.OutDir
		this.path['EXTERNS'] = this.path['ROOT'] + SPL + "extern" +SPL+ "lib" 

		this.projs.clear()
		ReadSolution(this.path['ROOT'], this.projs, this.path)
	
		if not len(this.projs):
			this.Logout("No Cprojects found in working dir: '" + this.path['ROOT'] + "'", "warning")
			raise commands.ExeptionTerminated

		this.FindModified()
		this.CheckRebuild()
		this.CompileObjects()
		this.PackObjects()
		this.LinkObjects()
		SaveCache(this)

	def FindModified(this):
		print(" -- Finding Modified Files")
		this.changed_files.clear()
		is_build = False
		cache = getCache()
		lastcycletime = cache["LastRun"]
		

		for proj in this.projs:
			proj.rebuild = False
	
			files = []
			FindFiles(files, proj.dir, 'cpp')
			FindFiles(files, proj.dir, 'h')

			for file in files:
				if os.path.getmtime(file) > lastcycletime:
					this.changed_files.append(file)
					proj.rebuild = True
					is_build = True	
					break

		if is_build:
			return

		print("       Not Found. Terminating ")
		raise commands.ExeptionTerminated

	def CheckRebuild(this):
		print(" -- Determinding Objects to be Rebuilded ")
		
		def rebld(proj):
			for dep in proj.projdeps:
				
				if rebld(dep):
					proj.rebuild = True
					return True

			return proj.rebuild


		if this.env.rebuild_type == "tree":
			for root in get_root_objs(this.projs):
				root.rebuild = rebld(root)


	def CompileObjects(this):
		output = this.path['OUTPUT']
		print(" -- Compiling Objects")

		for proj in this.projs:
			reb_files = []
			if this.env.rebuild_type == "fl":
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

			threads = []
			import threading

			for i in range(len(reb_files)):
				file = reb_files[i]
				outfile = output + SPL + proj.name + SPL + "obj" + SPL + file.rsplit(SPL, 1)[1]		
				
				if this.env.threading:
					thread = threading.Thread(target=compiler.GenObj, 
											args=(file, proj.incldirs, 
											output + SPL + proj.name + SPL + "obj", 
											this.env.debug, 
											this.env.sysarch,
											proj.defenitions))
					threads.append(thread)
					thread.start()
				else:
					compiler.GenObj(file,
									proj.incldirs, 
									output + SPL + proj.name + SPL + "obj", 
									this.env.debug, 
									this.env.sysarch, 
									this.env.platform, 
									proj.defenitions)

			for thread in threads:
				thread.join()

			for i in range(len(proj.files)):
				proj.files[i] = output + SPL + proj.name + SPL + "obj" + SPL + proj.files[i].rsplit(SPL, 1)[1]

	def PackObjects(this):
		output = this.path['OUTPUT']
		print("\n\n -- Packing Objects")

		for proj in this.projs:
			if not proj.rebuild: continue
			print("     ", proj.name)
			compiler.PackObjs(proj.files, output + SPL + proj.name, proj.name, this.env.platform)

	def LinkObjects(this):
		output = this.path['OUTPUT']
		print("\n\n -- Linking Objects")

		for proj in this.projs:
			if not proj.type == "Executable": continue
			print("     ", proj.name)
			
			linkprojs = get_build_order(this.projs, proj)
			linknames = []
			externals = []
			for linkproj in linkprojs:
				linknames.append(linkproj.name)
				externals += linkproj.externs

			linknames += externals + proj.externs
			compiler.LinkObjs(proj.name, output, linknames, proj.libdirs, this.env.debug, this.env.sysarch, this.env.platform)

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



if __name__ == "__main__":
	print(" \n\n ---------------- Builder ------------------------- \n\n ")
	Builder().RunInteractiveMode()