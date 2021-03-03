
import SCons.Script
from SCons.Environment import Environment

class CBLD():
	
	def __init__(this, debug = 0):
		this.path = {}
		this.debug = debug
		this.path['OUTPUT'] = 'build/variant1'
		this.path['OUT_ABS'] = Dir('.').srcnode().abspath + '/build/variant1'
		this.path['ROOT'] = Dir('.').srcnode().abspath 
		this.path['SRS_ROOT'] = Dir('.').srcnode().abspath + '/'

	VariantDir('build/variant1', '.')

	def AddProjects(this, paths):
		for i in range(len(paths)):
			paths[i] = this.path['OUTPUT'] + '/' + paths[i] + '/CBuild.py'

		SConscript(paths)

	def Log(this, params):		
		print(params)

	def Executable(this, name, files, incl, libs, libs_path):
		
		for incpath in range(len(incl)):
			incl[incpath] = this.path['SRS_ROOT'] + incl[incpath]

		env = Environment(CPPPATH=incl, LIBS=libs, LIBPATH=libs_path)
		if this.debug: 
			env.Append(CXXFLAGS = ['/DEBUG'])
		
		return env.Program(files)

	def StaticLibrary(this, name, files, incl, libs, libs_path):

		for incpath in range(len(incl)):
			incl[incpath] = this.path['SRS_ROOT'] + incl[incpath]

		env = Environment(CPPPATH=incl, LIBS=libs, LIBPATH=libs_path)
		if this.debug: 
			env.Append(CXXFLAGS = ['/DEBUG'])

		return env.StaticLibrary(name, files)


cbld = CBLD()
Export('cbld')


cbld.AddProjects(['Sourse/Types', 'Sourse/Application', 'Examples', 'Tests'])
