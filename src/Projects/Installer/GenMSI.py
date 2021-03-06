# Above line is for python
#
# Generate MSI files
#
# vbs scripts copied from:
# C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Samples\SysMgmt\Msi\Scripts
#
# Revision History
# 2020-01-26 Change default compiler to vc142
# 2019-09-21 Change to getopt
# 2019-09-20 Create distribution files automagically.
# 2017-05-10 Fix vcver/platformTools issues.
# 2017-04-09 Added vc141 support.
# 2016-10-19 Changed RmMinusRF to using shutil.rmtree()
# 2016-07-01 Add signing
# 2016-06-10 Convert to Python3
# 2015-07-23 Added vc14 support.
# 2014-11-17 wix3.9 deprecated -sadv
# 2014-11-17 Removed .mo files (moved into resource)
# 2013-11-27 Fix arbdll.dll access when building x64.
# 2013-08-15 Added support for multiple wxs files.
# 2013-08-12 Added vc11/12 support, remove pre-10 support.
# 2012-06-01 Changed VC10 output directory
# 2011-11-23 Add internet shortcut to start menu
# 2011-11-14 Treat wix warnings as errors
# 2011-09-23 Allow support of admin installs "msiexec /a msi"
# 2011-08-19 Fixed /test option (it was always writing the csv file)
# 2011-08-13 Change to vc10 for default.
# 2011-04-27 Generate unique prodcodes for each architecture.
#            Also discovered script wasn't writing the cvs file correctly.
# 2010-11-18 Optimize the build by caching the cab file.
# 2010-11-16 Turned on pedantic warnings, changed how languages are handled.
# 2010-10-08 Suppress wixpdb creation.
#            Allow packaging to pull from specified VC build (8/9/10)
# 2010-08-25 Stop using old prodcode. Detect same version as old.
# 2010-06-11 Convert vbs scripts to python
# 2010-05-22 Added /test2 option.
# 2010-05-07 Added /user option (default: perMachine)
#            Merge languages into one msi.
# 2009-12-23 Auto-detect current WiX installation (%WIX%)
# 2009-10-19 Added 'create' option.
# 2009-08-08 Tweaked code to remove ICE61 warning.
#            Allow user specification of where WiX is (/wix)
# 2009-07-26 Removed Win98 support.
# 2009-05-02 Upgraded to wix3.
# 2009-04-16 Enable v2 to upgrade v1 when the beta period ends.
#            Removed Inno support since I don't maintain it anymore.
# 2009-03-01 Made v2 coexist with v1, updated for wxWidgets
#            Fixed bug where french and arbhelp would always be installed
# 2008-10-14 Added x64 specific tags
# 2007-11-14 Wix works! (it's now the default)
# 2007-11-05 Add WiX back (addition)
#            Note, WiX doesn't work yet, that's why I added Inno
# 2007-10-31 Changed from WiX to InnoSetup
# 2007-03-07 Created

"""GenMSI.py [-w path] [-b user|32|64|all]* [-x] [-e] [-t target] [-d distrib]
	-w: Override internal wix path (c:\Tools\wix3, default %WIX%)
	-b: Build target (multiple can be specified
	    user: Create msi as a per-user install (default: per-machine)
	    32: Create 32bit Unicode msi
	    64: Create 64bit Unicode msi
	    all: Create all of them (default)
	-x: Do not clean up generated files
	-e: Generate .msi for test purposes (don't write to InstallGUIDs.csv)
	-t target: Generate msi using specified vc version (Default: vc142)
	-d distrib: Directory to write distribution files to (Default: ./distrib)
"""

import datetime
import getopt
import glob
import msilib
import os
import shutil
import stat
import string
import subprocess
import sys
import zipfile

# Where top-level AgilityBook directory is relative to this script.
AgilityBookDir = r'..\..\..'

# Where to assemble all the files
DistribDir = r'.\distrib'

FilesToCopy = [
	'AgilityBook.pdb',
	'AgilityBook.exe',
	'ARBHelp.pdb',
	'ARBHelp.exe',
	'ARBUpdater.pdb',
	'ARBUpdater.exe']

# Where is WiX? Can be overriden on command line.
# This is initially set to %WIX%/bin, if WiX is actually installed.
WiXdir = r'c:\Tools\wix3'

WinSrcDir = AgilityBookDir + r'\src'

# Code:
#  1: Win32/Unicode
#  2: Win32/MBCS
#  3: Win64/Unicode
code32 = 1
code64 = 3

# This is only used to update the InstallGUIDs file. Make sure it stays in
# sync with the current code in AgilityBook.wxi (UPGRADECODE)
UpgradeCode = '4D018FAD-2CBC-4A92-B6AC-4BAAECEED8F4'

# Name used for (wix culture name, language id) when building msi.
supportedLangs = [
	('en-us', '1033'),
	('fr-fr', '1036')]

wxsFiles = [
	('AgilityBook'),
	('Dialogs')]

CA_dll = 'arbdll.dll'


def errprint(*args):
	# Used to print exception messages
	strings = list(map(str, args))
	msg = ' '.join(strings)
	if msg[-1:] != '\n':
		msg += '\n'
	sys.stderr.write(msg)


# Kill an entire directory
def del_rw(func, path, exc_info):
	os.chmod(path, stat.S_IWUSR)
	func(path)
def RmMinusRF(name):
	if os.access(name, os.F_OK):
		mode = os.stat(name)[stat.ST_MODE]
		if stat.S_ISDIR(mode):
			shutil.rmtree(name, onerror=del_rw)
		else:
			if not os.access(name, os.W_OK):
				os.chmod(name, 0o666)
			if os.access(name, os.W_OK):
				try:
					os.remove(name)
				except OSError as msg:
					errprint('os.remove:', msg)


def getversion(numParts):
	ver = '0'
	ver2 = '0'
	for i in range(1, numParts):
		ver = ver + '.0'
		ver2 = ver2 + '_0'
	resStr = [
		'#define ARB_VER_MAJOR',
		'#define ARB_VER_MINOR',
		'#define ARB_VER_DOT',
		'#define ARB_VER_BUILD'
		]
	found = 0;
	version = ['0', '0', '0', '0']
	res = open(WinSrcDir + r'\Include\VersionNumber.h', 'r')
	while (1):
		line = res.readline()
		if line:
			line = line.strip()
			for i in range(0, 4):
				pos = line.find(resStr[i])
				if 0 == pos:
					found = found + 1
					version[i] = line[pos+len(resStr[i]):].strip()
			if found == 4:
				break
		else:
			break
	ver = version[0]
	ver2 = version[0]
	for i in range(1, numParts):
		ver = ver + '.' + version[i]
		ver2 = ver2 + '_' + version[i]
	return ver, ver2


# returns baseDir, outputFile
def getoutputvars(code, version, platformTools):
	outputFile = ''
	baseDir = ''
	if code32 == code:
		outputFile = 'AgilityBook-' + version + '-win'
		baseDir = AgilityBookDir + r'\bin\vc' + platformTools + 'Win32\Release'
		distDir = 'vc' + platformTools
	elif code64 == code:
		outputFile = 'AgilityBook-' + version + '-x64'
		baseDir = AgilityBookDir + r'\bin\vc' + platformTools + 'x64\Release'
		distDir = 'vc' + platformTools + 'x64'
	else:
		raise Exception('Invalid code')
	return baseDir, outputFile, distDir


def ReadPipe(logFile, cmd):
	while (1):
		line = cmd.readline()
		if line:
			line = str.rstrip(line.decode())
			print(line, file=logFile)
		else:
			break


def runcmd(command):
	print(command)
	p = subprocess.Popen(command, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
	ReadPipe(sys.stdout, p.stdout)


# This is the old WiSubStg.vbs script
#runcmd('cscript /nologo WiSubStg.vbs ' + baseMsi + ' ' + langId + '.mst ' + langId)
# No error checking - just let the exception kill it.
def WiSubStg(baseMsi, langId):
	database = msilib.OpenDatabase(baseMsi, msilib.MSIDBOPEN_TRANSACT)
	view = database.OpenView("SELECT `Name`,`Data` FROM _Storages")
	record = msilib.CreateRecord(2)
	record.SetString(1, langId)
	view.Execute(record)
	record.SetStream(2, langId + '.mst')
	view.Modify(msilib.MSIMODIFY_ASSIGN, record)
	database.Commit()
	view = None
	database = None


# This is the old WiLangId.vbs script
#runcmd('cscript /nologo WiLangId.vbs ' + baseMsi + ' Package ' + sumInfoStream)
# No error checking - just let the exception kill it.
def WiLangId(baseMsi, sumInfoStream):
	database = msilib.OpenDatabase(baseMsi, msilib.MSIDBOPEN_TRANSACT)
	sumInfo = database.GetSummaryInformation(1)
	template = sumInfo.GetProperty(7).decode()
	iDelim = template.find(';')
	platform = ';'
	if 0 <= iDelim:
		platform = template[0:iDelim+1]
	sumInfo.SetProperty(7, platform + sumInfoStream)
	sumInfo.Persist()
	database.Commit()
	database = None


def WriteCode(baseMsi, ver4Dot, code, vcver):
	database = msilib.OpenDatabase(baseMsi, msilib.MSIDBOPEN_READONLY)
	view = database.OpenView("SELECT `Value` FROM Property WHERE `Property`='ProductCode'")
	record = msilib.CreateRecord(1)
	view.Execute(record)
	data = view.Fetch()
	view = None
	database = None
	prodcode = data.GetString(1)

	if len(prodcode) > 0:
		d = datetime.datetime.now().isoformat(' ')
		codes = open(AgilityBookDir + r'\Misc\InstallGUIDs.csv', 'a')
		installs = ''
		if code == code64:
			installs = 'VC' + vcver + ',x64'
		else:
			installs = 'VC' + vcver + ',win32'
		print('v' + ver4Dot + ',' + d + ',' + prodcode + ',' + UpgradeCode + ',' + installs, file=codes)


def GetWxsFiles(extension):
	files = []
	for file in wxsFiles:
		files += [file + extension]
	return files


def GetWxsFilesAsString(extension):
	str = ''
	files = GetWxsFiles(extension)
	for file in files:
		str += ' ' + file
	return str


def CopyCAdll(ver4Line, platformTools):
	baseDir, outputFile, distDir = getoutputvars(code32, ver4Line, platformTools)
	arbdll = baseDir + '\\' + CA_dll
	if not os.access(arbdll, os.F_OK):
		print(arbdll + r' does not exist, MSI skipped')
		return 0
	shutil.copy(arbdll, '.')
	return 1


def genWiX(ver3Dot, ver4Dot, ver4Line, code, tidy, perUser, testing, vcver, platformTools, distrib):
	baseDir, outputFile, distDir = getoutputvars(code, ver4Line, platformTools)

	vcdir = os.path.join(distrib, distDir)
	if not os.access(vcdir, os.F_OK):
		os.mkdir(vcdir)

	for file in FilesToCopy:
		fullpath = os.path.join(baseDir, file)
		if not os.access(fullpath, os.F_OK):
			print(fullpath + r' does not exist, MSI skipped')
			return 0
		shutil.copy(fullpath, vcdir)

	cabcache = 'cabcache'
	RmMinusRF(cabcache)
	# -wx: Treat warnings as errors
	candleCmd = 'candle -nologo -wx -pedantic'
	candleCmd += ' -dCURRENT_VERSION=' + ver3Dot
	if code == code64:
		candleCmd += ' -arch x64'
	else:
		candleCmd += ' -arch x86'
	candleCmd += ' -ext WixUIExtension -ext WixUtilExtension'
	candleCmd += ' -dBASEDIR="' + baseDir + '"'
	candleCmd += ' -dINSTALL_SCOPE=' + perUser
	runcmd(candleCmd + GetWxsFilesAsString('.wxs'))
	processing = 0
	baseMsi = ''
	baseMsiZip = ''
	sumInfoStream = ''
	for culture, langId in supportedLangs:
		processing += 1
		basename = os.path.join(distrib, outputFile)
		if processing > 1:
			basename += '_' + culture
		# -wx: Treat warnings as errors
		lightCmd = 'light -nologo -wx -pedantic -spdb'
		lightCmd += ' -dcl:high -cc ' + cabcache
		if not processing == 1:
			lightCmd += ' -reusecab'
		lightCmd += ' -ext WixUIExtension -ext WixUtilExtension'
		lightCmd += ' -dWixUILicenseRtf=License_' + culture + '.rtf'
		lightCmd += ' -cultures:' + culture
		lightCmd += ' -loc AgilityBook_' + culture + '.wxl'
		lightCmd += ' -out ' + basename + '.msi'
		runcmd(lightCmd + GetWxsFilesAsString('.wixobj'))
		if processing == 1:
			baseMsi = basename + '.msi'
			baseMsiZip = basename + '.zip'
			sumInfoStream += langId
		else:
			sumInfoStream += ',' + langId
			runcmd('torch -nologo -p -t language ' + baseMsi + ' ' + basename + '.msi -out ' + langId + '.mst')
			WiSubStg(baseMsi, langId)
		if tidy:
			if os.access(langId + '.mst', os.F_OK):
				os.remove(langId + '.mst')
			if processing > 1:
				if os.access(basename + '.msi', os.F_OK):
					os.remove(basename + '.msi')

	if processing > 1:
		WiLangId(baseMsi, sumInfoStream)
	if tidy:
		RmMinusRF(cabcache)
	if not testing:
		WriteCode(baseMsi, ver4Dot, code, vcver)
	runcmd(r'python ..\SignStuff.py ' + baseMsi)

	zip = zipfile.ZipFile(baseMsiZip, 'w')
	zip.write(baseMsi, outputFile + '.msi')
	zip.close()

	if tidy:
		for file in GetWxsFiles('.wixobj'):
			if os.access(file, os.F_OK):
				os.remove(file)

	return 1


def main():
	global WiXdir
	# When WiX is installed, it sets "WIX" to point to the top-level directory
	if 'WIX' in os.environ:
		WiXdir = os.environ['WIX'] + r'\bin'
	# This must be set to a valid Package/@InstallScope value.
	perUser = "perMachine"
	b32 = 0
	b64 = 0
	tidy = 1
	testing = 0
	vcver = '142'
	platformTools = '142'
	distrib = DistribDir

	if 1 == len(sys.argv):
		print('Setting -b 64 -e')
		b64 = 1
		testing = 1
	else:
		try:
			opts, args = getopt.getopt(sys.argv[1:], "d:w:b:t:xe")
		except getopt.error as msg:
			print(msg)
			print('Usage', __doc__)
			return 1

		usage = False

		for o, a in opts:
			if '-d' == o:
				distrib = a
			elif '-w' == o:
				WiXdir = a
			elif '-b' == o:
				if a == '32':
					b32 = 1
				elif a == '64':
					b64 = 1
				elif a == 'all':
					b32 = 1
					b64 = 1
				elif a == 'user':
					perUser = "perUser"
			elif '-t' == o:
				if a == 'vc141':
					vcver = '141'
					platformTools = '141'
				elif a == 'vc142':
					vcver = '142'
					platformTools = '142'
			elif '-x' == o:
				tidy = 0
			elif '-e' == o:
				testing = 1
			else:
				usage = True
				break
		if usage:
			print('Usage:', __doc__)
			return 1

	if os.access(distrib, os.F_OK):
		print('ERROR: "' + distrib + '" exists. Please clean this up first.')
		return 1

	if not os.access(WiXdir, os.W_OK):
		print('ERROR: "' + WiXdir + '" does not exist!')
		return 1

	if b32 + b64 == 0:
		b32 = 1
		b64 = 1

	ver3Dot, ver3Line = getversion(3)
	ver4Dot, ver4Line = getversion(4)

	os.mkdir(distrib)

	# Wix
	os.environ['PATH'] += ';' + WiXdir
	if not CopyCAdll(ver4Line, platformTools):
		return 1
	if b32:
		genWiX(ver3Dot, ver4Dot, ver4Line, code32, tidy, perUser, testing, vcver, platformTools, distrib)
	if b64:
		genWiX(ver3Dot, ver4Dot, ver4Line, code64, tidy, perUser, testing, vcver, platformTools, distrib)
	if os.access(CA_dll, os.W_OK):
		os.remove(CA_dll)

	return 0


if __name__ == '__main__':
	sys.exit(main())
