
import os as os
import json
import sys
import shutil
from os.path import relpath

__FPFILE__ = __file__
SPL = "/"

if sys.platform == "win32":
	SPL = "\\"

if not os.path.isabs(__file__):
	__FPFILE__ = os.path.abspath(__file__)

def to_str(list, befor = False, separetor = ' ',  after = False, after_sep = ' '):
	string = ''
	for item in list:
		if befor and after:
			string = string + separetor + item + after_sep 
		else:
			if after:
				string = string + item + after_sep
			if befor:
				string = string + separetor + item

	return string

def RootDir(reponame):
	current = os.path.dirname(os.path.realpath(__FPFILE__))
	while True:
		names = current.rsplit(SPL, 1) 
		if names[1] == reponame:
			return current
		current = names[0]
		if not len(current):
			return 0

def FindFiles(files, abs_path, type, must_include = False, name = ''):
	subfolders = os.listdir(abs_path + SPL)
	for i in range(len(subfolders)):
		if subfolders[i].rfind('.') != -1:
			if  subfolders[i].rsplit('.', 1)[1] == type:
				if must_include:
					if name in subfolders[i].rsplit('.', 1)[0]:
						files.append(abs_path + SPL + subfolders[i])
				else:
					files.append(abs_path + SPL + subfolders[i])

	for subfld in subfolders:
		if os.path.isdir(abs_path + SPL + subfld):
			FindFiles(files, abs_path + SPL + subfld, type, must_include, name)