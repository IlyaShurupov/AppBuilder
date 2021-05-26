from common import *
import os


class Gcc:
	def __init__(self):
		self.name = "gcc"

	def compile_ojs(self, cmpl):
		cmd = "g++ -c  -m" + cmpl.arch.split("x")[1] + " "
		if cmpl.debug: cmd += " -g "

		for cmpl_block in cmpl.compile:

			print("\n" + cmpl_block.out.path)

			definitions = to_str(cmpl_block.definitions, 1, " -D\"", 1, "\"")
			includes = to_str(cmpl_block.includes, 1, " -I\"", 1, "\"")

			if not os.path.isdir(cmpl_block.out.path): os.makedirs(os.path.abspath(cmpl_block.out.path))

			for file in cmpl_block.source_files:
				outfile = " -o" + cmpl_block.out.add_path(file.name).path + ".obj "
				os.system(cmd + file.path.path + definitions + includes + outfile)
				print("	" + file.name + ".obj ")

	def pack_obj(self, cmpl):
		print("\n")
		cmd = "ar rcs "
		for pack_block in cmpl.pack:
			file_paths = []
			for file in GetFiles(pack_block.from_dir, ".obj"):
				file_paths.append(file.path)

			os.system(cmd + pack_block.out.path + ".lib" + to_str(file_paths, 1, " ", 1, " "))
			print(pack_block.out.path + ".lib")

	def link_libs(self, cmpl):
		print("\n")
		cmd = "g++ -static -m" + cmpl.arch.split("x")[1] + " "
		if cmpl.debug: cmd = cmd + " -g "

		for link_block in cmpl.link:
			outfile = " -o " + link_block.out.path
			libs = to_str(link_block.libs, 1, " -l\"", 1, "\"")

			lib_paths = []
			for file in link_block.lib_dirs:
				lib_paths.append(file.path)

			lib_paths = to_str(lib_paths, 1, " -L\"", 1, "\"")

			os.system(cmd + lib_paths + libs + outfile)
			print(outfile)

	def compile(self, cmpl):
		self.compile_ojs(cmpl)
		self.pack_obj(cmpl)
		self.link_libs(cmpl)


class Msvc:
	def __init__(self):
		self.name = "msvc"

	def compile(self, compile_instructions):
		pass


compilers = [Gcc(), Msvc()]


def perform_compilation(compile_instructions, name):
	for compiler in compilers:
		if compiler.name == name:
			try:
				compiler.compile(compile_instructions)
				return 0
			except ZeroDivisionError as err:
				pass
	return -1
