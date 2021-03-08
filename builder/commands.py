import os as os
import cparser


class ExeptionTerminated(Exception):
	pass

class ExeptionBuildError(Exception):
	pass
	

class BuildCommand():
	
	def init(this, name, describ):
		this.id = name
		this.describ = describ

	def OnError(this, args):
		print(" Failed to execute command ")

	def OnFinish(this):
		pass

	def Exec(this, bld, args, original):
		pass

	def help(this):
		print("  " + this.id + " - " + this.describ)

class BLD(BuildCommand):
	def __init__(this):
		this.init("bld", " compiles binaries depending on the env ")
	
	def Exec(this, bld, args, original):
		bld.Build(args)

class ENV(BuildCommand):
	def __init__(this):
		this.init("env", "prints the env variables")
	
	def Exec(this, bld, args, original):
		bld.Logout("   current build env: ")
		bld.LogEnv()

class EXIT(BuildCommand):
	def __init__(this):
		this.init("q", " exits from builder commands proc ")
	
	def Exec(this, bld, args, original):
		exit(0)

class HELP(BuildCommand):
	def __init__(this):
		this.init("help", " prints describtin of each command  ")
	
	def Exec(this, bld, args, original):
		for command in bld.commands:
			command.help()

class DBG(BuildCommand):
	def __init__(this):
		this.init("dbg", " runs compiler debuging utils ")
	
	def Exec(this, bld, args, original):
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

class SET(BuildCommand):
	def __init__(this):
		this.init("set", " set enviroment variable ")
	
	def Exec(this, bld, args, original):
		exec("bld.env."+args[0]+"="+args[1])

class EXEC(BuildCommand):
	def __init__(this):
		this.init("exec", " executes python code directly calling 'exec'")
	
	def Exec(this, bld, args, original):
		exec(original.split(":", 1)[1].lstrip())

def init_comands(blder):
	blder.commands.append(ENV())
	blder.commands.append(BLD())
	blder.commands.append(EXIT())
	blder.commands.append(HELP())
	blder.commands.append(DBG())
	blder.commands.append(SET())
	blder.commands.append(EXEC())