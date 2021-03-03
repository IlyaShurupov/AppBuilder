import os

def to_str(list, separetor):
	string = ''
	for item in list:
		string = string + separetor + item
	return string

def Log(out):
	print(out)

def GccCommand(command):
	os.system(command)


def GenObj(file, incl, output):
	Log("Generating Object: " + file + " --> " + file.rsplit('.', 1)[0] + '.o' )
	GccCommand("gcc -c " + file + ' '+ to_str(incl, ' -I'))

def PackObjs(files, name):
	Log("Packing Objects: " + to_str(files, ' ') + " --> " + name + '.o' )
	GccCommand("ar rcs " + " " + name + ".o" + " " + to_str(files, ' '))

def LinkObjs(name, files):
	Log("Linking Objects: " + to_str(files, ' ') + " --> " + name + '.exe' )
	GccCommand("gcc -o " + name + to_str(files, ' '))

