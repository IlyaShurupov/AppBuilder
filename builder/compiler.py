
from common import * 
from common import SPL

def GenObj(file, incl, output, dbg = False, sysarch = "64", platform = 'windows', defenitions = []):
	if platform == 'windows' or platform == 'linux':
		outfile = output + SPL + file.rsplit(SPL, 1)[1] + '.o'	
		if not os.path.isdir(output):
			os.makedirs(os.path.abspath(output))
		cmd = "g++ -c " 
		if dbg:
			cmd = cmd + " -g "
		sysarch_arg = " -m" + sysarch + " "
		destr = to_str(defenitions, True, " -D")
		cmd = cmd + sysarch_arg + file + '.cpp '+ to_str(incl, True, ' -I') + ' -o ' + outfile + destr
		os.system(cmd)
		print("      " +  file.rsplit(SPL, 1)[1] + '.o')

def PackObjs(files, outdir, name, platform = 'windows'):
	if platform == 'windows':
		cmd = "ar rcs " 
		cmd += outdir + SPL + name + '.lib ' + to_str(files, False, ' ', True, '.o ')
		os.system(cmd)
	elif platform == 'linux':
		cmd = "ar rcs " 
		cmd += outdir + SPL + "lib" + name + '.so ' + to_str(files, False, ' ', True, '.o ')
		os.system(cmd)

def LinkObjs(name, output, objs, objpaths, dbg = False, sysarch = "64", platform = 'windows'):
	if platform == 'windows':
		outfile = " -o " + output + SPL + name + SPL + name
		libstr = to_str(objs, True, " -l")
		libpathsstr = to_str(objpaths, True, " -L")
		cmd = "g++ -static " 
		if dbg:
			cmd = cmd + " -g "
		sysarch_arg = " -m" + sysarch + " "
		cmd = cmd + sysarch_arg + libpathsstr + libstr + outfile
		os.system(cmd)

	elif platform == 'linux':
		outfile = " -o " + output + SPL + name + SPL + name
		libstr = to_str(objs, True, " -l")
		libpathsstr = to_str(objpaths, True, " -L")
		cmd = "g++ -no-pie " 
		if dbg:
			cmd = cmd + " -g "
		sysarch_arg = " -m" + sysarch + " "
		cmd = cmd + sysarch_arg + libpathsstr + libstr + outfile
		os.system(cmd)