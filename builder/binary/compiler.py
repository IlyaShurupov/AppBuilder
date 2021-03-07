
import os as os
from cparser import to_str 


def GenObj(file, incl, output, dbg = False):
	outfile = output + "\\" + file.rsplit('\\', 1)[1] + '.o'	
	if not os.path.isdir(output):
		os.makedirs(os.path.abspath(output))
	cmd = "g++ -c " 
	if dbg:
		cmd = cmd + " -g "
	cmd = cmd + file + '.cpp '+ to_str(incl, True, ' -I') + ' -o ' + outfile
	os.system(cmd)

def PackObjs(files, outdir, name):
	cmd = "ar rcs " 
	cmd = cmd + outdir + '/' + name + '.lib ' + to_str(files, False, ' ', True, '.o ')
	os.system(cmd)

def LinkObjs(name, output, objs, objpaths, dbg = False):
	outfile = " -o " + output + "\\" + name + "\\" + name
	libstr = to_str(objs, True, " -l")
	libpathsstr = to_str(objpaths, True, " -L")
	cmd = "g++ -static " 
	if dbg:
		cmd = cmd + " -g "
	cmd = cmd + libpathsstr + libstr + outfile
	os.system(cmd)