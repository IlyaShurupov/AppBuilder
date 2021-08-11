
import os.path as path

class ENV:
	def __init__(self):
		self.root = path.dirname(__FILE__)
		self.out = path.join(__FILE__)

# ********************************** Entry ******************************************** #

import sys

if len(sys.argv) == 1:
	print(" Nodes Build Python Script - pass \"Help\" for use information. \n ")

else if sys.argv[1] == "Help":
	print(" Helping \n ")

else:
	try:
		for argi in range(1, len(sys.argv)):
			exec(sys.argv[argi])

	except Exception as exc:
		print("\n ERROR  : \n ")
		print(exc)