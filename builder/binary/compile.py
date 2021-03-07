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

	def Build(this):
		cparser.ReadSolution(this.path['ROOT'], this.projs, this.path)
	
		if not len(this.projs):
			this.Logout("No Cprojects found in working dir: '" + this.path['ROOT'] + "'", "warning")
			exit(0)

		if os.path.isdir(os.path.abspath(this.path['OUTPUT'])):
			pass
			#shutil.rmtree(os.path.abspath(this.path['OUTPUT']))

		this.FindModified()
		this.CalcDepTree()
		this.CompileProjects()
		cparser.SaveCache(this)

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
			exit(1)

	def ProcArgs(this, args):
			
			out_dir = ''
			root_abs_dir = ' '
			this.path['ROOT_ABS'] = os.path.dirname(os.path.realpath(__file__))

			for arg in args:
				if arg == __file__.rsplit('\\', 1)[1]:
					continue
				if arg == "debug":
					this.debug = True
				elif arg[0] == "-":
					if arg[1] == "o":
						out_dir = arg.split('o', 1)[1]
					elif arg[1] == "d":
						root_abs_dir = arg.split("", 1)[1]
					elif arg[1] == "r":
						root_abs_dir = RootDir(arg.split("r", 1)[1])
					elif arg[1] == "t":
						this.rebuild_type = arg.split("t", 1)[1]
				else:
					this.Logout("Cant resolve argument '" + arg + "'", "warning")

			if out_dir == '':
				out_dir = "build/binaries/"
				if this.debug:
					out_dir += "debug"
				else:
					out_dir += "release"
			if root_abs_dir != ' ':
				this.path['ROOT_ABS'] = root_abs_dir

			this.path['ROOT'] = relpath(this.path['ROOT_ABS'], os.path.dirname(os.path.realpath(__file__)))
			this.path['OUTPUT'] = this.path['ROOT'] + "\\" + out_dir

	def __init__(this, args):

		this.Logout("\n\n ---------- Build Started ---------- ")
		this.debug = False
		this.rebuild_type = "tree"
		this.path = {}
		this.projs = []
		this.changed_files = []

		this.ProcArgs(args)

	def CompileProjects(this):
		output = this.path['OUTPUT']
		print("\n -- Compiling Projects Into ",  os.path.abspath(output))



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

			print("\n\n ", proj.name)
			print("     Generating Objects Into " +  proj.name + '\\obj')

			for i in range(len(reb_files)):
				file = reb_files[i]
				outfile = output + "\\" + proj.name + "\\obj" + '\\' + file.rsplit('\\', 1)[1]		
				print("	" +  file.rsplit('\\', 1)[1] + '.o')
				compiler.GenObj(file, proj.incldirs, output + "\\" + proj.name + "\\obj", this.debug)

			for i in range(len(proj.files)):
				proj.files[i] = output + "\\" + proj.name + "\\obj" + '\\' + proj.files[i].rsplit('\\', 1)[1]	

			if proj.type[0] == 'E':
				print("\n     Packing Objects Into ", proj.name + "\\" + proj.name + '.lib ')				
				compiler.PackObjs(proj.files, output + "\\" + proj.name, proj.name)
				linkfiles = [proj.name + ".lib"] + proj.libs
				for i in range(len(linkfiles)):
					linkfiles[i] = linkfiles[i].split('.')[0]
				print("\n     Linking Objects [" + cparser.to_str(linkfiles, True) + " ] Into " + proj.name + "\\" + proj.name + '.exe' )
				compiler.LinkObjs(proj.name, output, linkfiles, proj.libdirs, this.debug)

			elif proj.type[0] == 'S':
				print("\n     Packing Objects Into ", proj.name + "\\" + proj.name + '.lib ')
				compiler.PackObjs(proj.files, output + "\\" + proj.name, proj.name)


	def CalcDepTree(this):
		print("\n -- Defining Dependency Tree:")
	    
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

		
		str = '		'
		for proj in this.projs:
			str = str + proj.name + " "

		print(str)

	def FindModified(this):
		print("\n -- Finding Modified Files")
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
		exit(0)

	def CheckRebuild(this):
		print("\n -- Determinding Objects to be Rebuilded:")
		
		if this.rebuild_type == "tree":
			pass
		elif this.rebuild_type == "fl":
			pass
		elif this.rebuild_type == "prj":
			pass



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
	os.system("cls")

	Builder(sys.argv).Build()


if __name__ == "__main__":
	main()