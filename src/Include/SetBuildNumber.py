# Update build number in VersionNumber.h (Number of days since ARB was born)
# and ../../configure.in
#
# This assumes the current directory is 'Include'
# @li 2009-10-06 DRC Add multiprocessing awareness. Kind of.
# @li 2009-08-12 DRC Make sure full version is the same in configure.in
# @li 2009-04-12 DRC Added CalVerNum.h to be autoincremented.
#                Check to see if file actually changed before re-writing

import datetime
import os
import string
import sys

def doWork():
	update = 0
	vMajARB = '0'
	vMinARB = '0'
	vDotARB = '0'
	vBldARB = '0'
	ver = open('VersionNumber.h', 'r')
	verOut = open('VersionNumber.h.new', 'w')
	while 1:
		line = ver.readline()
		if line:
			defineMaj = '#define ARB_VER_MAJOR'
			defineMin = '#define ARB_VER_MINOR'
			defineDot = '#define ARB_VER_DOT'
			defineBld = '#define ARB_VER_BUILD'
			pos = line.find(defineMaj)
			if 0 == pos:
				vMajARB = string.strip(line[pos+len(defineMaj):])
			pos = line.find(defineMin)
			if 0 == pos:
				vMinARB = string.strip(line[pos+len(defineMin):])
			pos = line.find(defineDot)
			if 0 == pos:
				vDotARB = string.strip(line[pos+len(defineDot):])
			pos = line.find(defineBld)
			if 0 == pos:
				vBldOld = string.strip(line[pos+len(defineBld):])
				vBldARB = str((datetime.date.today() - datetime.date(2002,12,28)).days)
				if not vBldOld == vBldARB:
					update = 1
					print >>verOut, defineBld + '\t\t\t\t\t' + vBldARB
				else:
					print >>verOut, line,
			else:
				print >>verOut, line,
		else:
			break
	ver.close()
	verOut.close()
	if update:
		print "VersionNumber.h updated to " + vMajARB + '.' + vMinARB + '.' + vDotARB + '.' + vBldARB
		os.remove('VersionNumber.h')
		os.rename('VersionNumber.h.new', 'VersionNumber.h')
	else:
		print "VersionNumber.h is up-to-date"
		os.remove('VersionNumber.h.new')

	update = 0
	vMajCAL = '0'
	vMinCAL = '0'
	vDotCAL = '0'
	vBldCAL = '0'
	ver = open('../cal_usdaa/CalVerNum.h', 'r')
	verOut = open('../cal_usdaa/CalVerNum.h.new', 'w')
	while 1:
		line = ver.readline()
		if line:
			defineMaj = '#define CAL_VER_MAJOR'
			defineMin = '#define CAL_VER_MINOR'
			defineDot = '#define CAL_VER_DOT'
			defineBld = '#define CAL_VER_BUILD'
			pos = line.find(defineMaj)
			if 0 == pos:
				vMajCAL = string.strip(line[pos+len(defineMaj):])
			pos = line.find(defineMin)
			if 0 == pos:
				vMinCAL = string.strip(line[pos+len(defineMin):])
			pos = line.find(defineDot)
			if 0 == pos:
				vDotCAL = string.strip(line[pos+len(defineDot):])
			pos = line.find(defineBld)
			if 0 == pos:
				vBldOld = string.strip(line[pos+len(defineBld):])
				vBldCAL = str((datetime.date.today() - datetime.date(2002,12,28)).days)
				if not vBldOld == vBldCAL:
					update = 1
					print >>verOut, defineBld + '\t\t\t\t\t' + vBldCAL
				else:
					print >>verOut, line,
			else:
				print >>verOut, line,
		else:
			break
	ver.close()
	verOut.close()
	if update:
		print "../cal_usdaa/CalVerNum.h updated to " + vMajCAL + '.' + vMinCAL + '.' + vDotCAL + '.' + vBldCAL
		os.remove('../cal_usdaa/CalVerNum.h')
		os.rename('../cal_usdaa/CalVerNum.h.new', '../cal_usdaa/CalVerNum.h')
	else:
		print "../cal_usdaa/CalVerNum.h is up-to-date"
		os.remove('../cal_usdaa/CalVerNum.h.new')

	update = 0
	conf = open('../../configure.in', 'r')
	confOut = open('../../configure.in.new', 'wb')
	while 1:
		line = conf.readline()
		if line:
			if 0 == line.find('AC_INIT('):
				newLine = 'AC_INIT(Agility Record Book, ' + vMajARB + '.' + vMinARB + '.' + vDotARB + '.' + vBldARB + ', [help@agilityrecordbook.com])\n'
				if not line == newLine:
					update = 1
					print >>confOut, newLine,
				else:
					print >>confOut, line,
			else:
				print >>confOut, line,
		else:
			break
	conf.close()
	confOut.close()
	if update:
		print "../../configure.in updated to " + vMajARB + '.' + vMinARB + '.' + vDotARB + '.' + vBldARB
		os.remove('../../configure.in')
		os.rename('../../configure.in.new', '../../configure.in')
	else:
		print "../../configure.in is up-to-date"
		os.remove('../../configure.in.new')


if __name__ == '__main__':
	# Not sure how to get locking across invokations. The multiprocessing lock
	# stuff seems aimed at spawned processes. This takes a crude path - there's
	# definitely still a window of opportunity.
	lockfile = 'SetBuildNumber.lck'
	if not os.access(lockfile, os.F_OK):
		lck = open(lockfile, 'wb')
		print >>lck, 'Locked'
		lck.close()
		doWork()
		os.remove(lockfile)
	else:
		print "SetBuildNumber is locked"
