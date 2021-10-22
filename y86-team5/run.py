#!/usr/bin/python3

import os
import sys

files = ['error1','error2','error3',
		 'error4','error5','error6','error7','error8','error9',
		 'error10','error11','error12','lab6a','lab6b','lab6c',
		 'lab7a','lab7b','lab7c','lab7d','lab7e','lab7f']
testdir = '/u/css/classes/3481/211/lab7/Test/'
outputdir = 'Test/'
#---------------------------------------------------------------------
# The following was used to generate master .dump files

# efb = '/u/css/classes/3481/201/lab7/efbyess'
# os.system('rm -f '+testdir+'*.dump')
# for fname in files:
#	 input = testdir+fname+".yo"
#	 os.system(efb + ' -fdemw ' + input + ' > ' +testdir+fname+'.dump')
# sys.exit(0)
#---------------------------------------------------------------------


if not (os.path.isdir(outputdir)):
	print("Need to create Test directory first\n")
	quit()

if not (os.path.exists('yess')):
	print("No executable 'yess' found")
	quit()

passing = 0

for fname in files:
	input = testdir+fname+".yo"
	efboutput = testdir+fname+".dump"
	studentoutput = outputdir+fname+'.dump'
	studentproblems = outputdir+fname+'.problems'
	
	#make sure input file exists
	if not os.path.exists(input):
		print('missing input file: '+input)
		break
	
	print('Testing '+fname+'...',end="")
	
	#remove old dump and problem files
	os.system('rm -f '+studentoutput+' '+studentproblems)
	
	#run student yess on input file - trace all stages: -fdemw
	os.system('./yess -fdemw ' + input + ' > ' + studentoutput)
	
	#compare student output with efb output
	os.system('diff '+ studentoutput + ' ' + efboutput 
	          + ' > ' + studentproblems)
	
	#if there are differences, keep problem and dump files
	if not os.system('test -s '+studentproblems):
		print('Failed.')
	else:
		print('Passed.')
		os.system('rm -f '+studentoutput+' '+studentproblems)
		passing += 1

total = len(files)
print(str(passing) +' out of '+str(total)+' passed.\n')
if passing != total:
	print('See Test directory for failed tests\n')
	
		

 


