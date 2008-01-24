# Generate MSI files
#
# Revision History
# 2007-11-14 DRC Wix works! (it's now the default)
# 2007-11-05 DRC Add WiX back (addition)
#            Note, WiX doesn't work yet, that's why I added Inno
# 2007-10-31 DRC Changed from WiX to InnoSetup
# 2007-03-07 DRC Created

"""GenMSI.py [/32] [/64] [/w98] [/all] [/notidy] [/wix | /inno] /test
	32: Create 32bit Unicode msi
	64: Create 64bit Unicode msi
	w98: Create 32bit MBCS msi
	all: Create all of them (default)
	notidy: Do not clean up generated files
	wix: Use WiX installer (default)
	inno: Use Inno installer
	test: Generate .msi for test purposes (don't write to InstallGUIDs.csv)
"""

import datetime
import os
import string
import sys

AgilityBookDir = "..\\..\\.."

WiXdir = r"c:\Tools\wix2"
ISTool = "c:\\Program Files\\ISTool"

WinSrcDir = AgilityBookDir + "\\src"
code32 = 1
code64 = 3
code98 = 2
UpgradeCode = "DD9A3E2B-5363-4BA7-9870-B5E1D227E7DB"


def getversion(numParts):
	ver = "0"
	ver2 = "0"
	for i in range(1, numParts):
		ver = ver + ".0"
		ver2 = ver2 + "_0"
	res = open(WinSrcDir + r"\Include\VersionNumber.h", "r")
	while (1):
		line = res.readline()
		if line:
			line = string.strip(line)
			ResString = "#define ARB_VERSION"
			pos = line.find(ResString)
			if 0 == pos:
				line = string.strip(line[pos+len(ResString):])
				line = line.split(',')
				ver = line[0]
				ver2 = line[0]
				for i in range(1, numParts):
					ver = ver + '.' + line[i]
					ver2 = ver2 + '_' + line[i]
				break
		else:
			break
	return ver, ver2


def genuuid():
	(childin, childout) = os.popen4("uuidgen -c")
	childin.close()
	line = string.rstrip(childout.readline())
	#line = string.upper(line) [-c means upper, leaving in case I change how guids are gotten]
	childout.close()
	return line


# returns baseDir, outputFile
def getoutputvars(code, version):
	outputFile = ""
	baseDir = ""
	if code32 == code:
		outputFile = "AgilityBook_" + version + "-win"
		baseDir = AgilityBookDir + "\\bin\\VC9Win32\\Release\\"
	elif code98 == code:
		outputFile = "AgilityBook_" + version + "-win98"
		baseDir = AgilityBookDir + "\\bin\\VC8Win32\\Release\\"
	elif code64 == code:
		outputFile = "AgilityBook_" + version + "-x64"
		baseDir = AgilityBookDir + "\\bin\\VC9x64\\Release\\"
	else:
		raise Exception, "Invalid code"
	return baseDir, outputFile


def runcmd(command):
	print command
	(childin, childout) = os.popen4(command)
	childin.close()
	while (1):
		line = childout.readline()
		if line:
			print >>sys.stdout, line,
		else:
			break


# Code:
#  1: Win32/Unicode
#  2: Win32/MBCS
#  3: Win64/Unicode
def genWiX(productId, version, version2, code, tidy):
	baseDir, outputFile = getoutputvars(code, version2)
	if tidy and not os.access(baseDir + "AgilityBook.exe", os.F_OK):
		print baseDir + "AgilityBook.exe does not exist, MSI skipped"
		return 0

	setup = open(outputFile + ".wxs", "w")
	print >>setup, r'<?xml version="1.0" encoding="UTF-8"?>'
	print >>setup, r'<Wix xmlns="http://schemas.microsoft.com/wix/2003/01/wi">'
	print >>setup, r'  <Product Id="' + productId + r'"'
	print >>setup, r'      UpgradeCode="' + UpgradeCode + r'"'
	print >>setup, r'      Name="Agility Record Book ' + version + '"'
	print >>setup, r'      Language="1033"'
	print >>setup, r'      Version="' + version + '"'
	print >>setup, r'      Manufacturer="dcon Software">'
	print >>setup, r'    <Package Id="????????-????-????-????-????????????"'
	print >>setup, r'        Description="Agility Record Book"'
	print >>setup, r'        Comments="Track all your agility records in one convenient place."'
	print >>setup, r'        InstallerVersion="200"'
	print >>setup, r'        Compressed="yes"/>'
	print >>setup, r''
	print >>setup, r'    <Property Id="INSTALLLEVEL">100</Property>'
	print >>setup, r''
	print >>setup, r'    <Property Id="ARPCOMMENTS">Track all your agility records in one convenient place.</Property>'
	print >>setup, r'    <Property Id="ARPCONTACT">David Connet</Property>'
	print >>setup, r'    <Property Id="ARPHELPLINK">http://www.agilityrecordbook.com</Property>'
	print >>setup, r'    <Property Id="ARPURLINFOABOUT">http://www.agilityrecordbook.com</Property>'
	print >>setup, r'    <Property Id="ALLUSERS">2</Property>'
	print >>setup, r''
	if code32 == code:
		print >>setup, r'    <Condition Message="This application only runs on Windows NT and above">VersionNT</Condition>'
	elif code98 == code:
		print >>setup, r'    <Condition Message="This application only runs on Windows ME and above">Version9X&gt;=490 OR VersionNT</Condition>'
	elif code64 == code:
		print >>setup, r'    <Condition Message="This application only runs on 64bit Windows systems">VersionNT64</Condition>'
	print >>setup, r''
	print >>setup, r'    <Upgrade Id="' + UpgradeCode + '">'
	print >>setup, r'      <UpgradeVersion OnlyDetect="no" Property="OLDVERSIONFOUND"'
	print >>setup, r'          IncludeMinimum="yes" Minimum="0.0.0"/>'
	print >>setup, r'    </Upgrade>'
	print >>setup, r''
	print >>setup, r'    <Media Id="1" Cabinet="AgilityRecordBook.cab" EmbedCab="yes" DiskPrompt="Disk 1" VolumeLabel="DISK1"/>'
	print >>setup, r'    <Property Id="DiskPrompt" Value="[1]"/>'
	print >>setup, r''
	print >>setup, r'    <Directory Id="TARGETDIR" Name="SourceDir">'
	print >>setup, r'      <Directory Id="DesktopFolder"/>'
	print >>setup, r'      <Directory Id="ProgramMenuFolder">'
	print >>setup, r'        <Directory Id="ARBMenuFolder" Name="ARB" LongName="Agility Record Book"/>'
	print >>setup, r'      </Directory>'
	print >>setup, r'      <Directory Id="ProgramFilesFolder">'
	print >>setup, r'        <Directory Id="CompanyFolder" Name="dconSoft" LongName="dcon Software">'
	print >>setup, r'          <Directory Id="INSTALLDIR" Name="ARB" LongName="Agility Record Book">'
	print >>setup, r''
	print >>setup, r'            <Component Id="ApplicationCore" Guid="F8886313-42A0-4B12-B2EE-C40FB614101F">'
	print >>setup, r'              <File Id="App" Name="ARBexe" LongName="AgilityBook.exe"'
	print >>setup, r'                    Source="' + baseDir + r'AgilityBook.exe"'
	print >>setup, r'                    Vital="yes" DiskId="1">'
	print >>setup, r'                <Shortcut Id="AppDesktop" Name="ARB" LongName="Agility Record Book" Directory="DesktopFolder" />'
	print >>setup, r'                <Shortcut Id="AppMenu" Name="ARB" LongName="Agility Record Book" Directory="ARBMenuFolder" />'
	print >>setup, r'              </File>'
	print >>setup, r'              <File Id="Help" Name="ARBchm" LongName="AgilityBook.chm"'
	print >>setup, r'                    Source="' + baseDir + r'AgilityBook.chm"'
	print >>setup, r'                    Vital="yes" DiskId="1">'
	print >>setup, r'                <Shortcut Id="ChmShortcut" Name="Help" LongName="Help" Directory="ARBMenuFolder" />'
	print >>setup, r'              </File>'
	print >>setup, r'              <!-- The description is the same since MFC will set it to that -->'
	print >>setup, r'              <ProgId Id="AgilityBook.Document" Description="AgilityBook.Document">'
	print >>setup, r'                <Extension Id="arb">'
	print >>setup, r'                  <Verb Id="open" Sequence="1" Command="Open" Target="[!App]" Argument=' + "'\"%1\"'/>"
	print >>setup, r'                </Extension>'
	print >>setup, r'              </ProgId>'
	print >>setup, r'              <Registry Id="AppIcon1" Root="HKCR" Key="AgilityBook.Document\DefaultIcon" Action="write"'
	print >>setup, r'                        Type="string" Value="[INSTALLDIR]AgilityBook.exe,0" />'
	print >>setup, r'            </Component>'
	print >>setup, r'            <Component Id="French" Guid="BC52AC1B-B3CD-49E1-BD21-6F9955AB31D6">'
	print >>setup, r'              <File Id="ResFRA" Name="fradll" LongName="AgilityBookFRA.dll"'
	print >>setup, r'                    Source="' + baseDir + r'AgilityBookFRA.dll"'
	print >>setup, r'                    Vital="no" DiskId="1" />'
	print >>setup, r'              <File Id="HelpFRA" Name="ARBchm" LongName="AgilityBookFRA.chm"'
	print >>setup, r'                    Source="' + baseDir + r'AgilityBookFRA.chm"'
	print >>setup, r'                    Vital="yes" DiskId="1" />'
	print >>setup, r'            </Component>'
	print >>setup, r'            <Component Id="CALusdaa" Guid="DA49BBD9-D16D-4B03-80F2-524553F76FFF">'
	print >>setup, r'              <File Id="usdaa" Name="CALusdaa" LongName="cal_usdaa.dll"'
	print >>setup, r'                    Source="' + baseDir + r'cal_usdaa.dll"'
	print >>setup, r'                    Vital="no" DiskId="1" />'
	print >>setup, r'            </Component>'
	print >>setup, r''
	print >>setup, r'            <Component Id="empty" Guid="74004007-0948-46E6-9250-5733CC0D4607"/>'
	print >>setup, r''
	print >>setup, r'          </Directory>'
	print >>setup, r'        </Directory>'
	print >>setup, r'      </Directory>'
	print >>setup, r'    </Directory>'
	print >>setup, r''
	print >>setup, r'    <Feature Id="Complete" Display="expand" Level="1"'
	print >>setup, r'             AllowAdvertise="no"'
	print >>setup, r'             Title="Agility Record Book" Description="Main Agility Record Book files" ConfigurableDirectory="INSTALLDIR">'
	print >>setup, r'      <ComponentRef Id="ApplicationCore"/>'
	print >>setup, r'      <ComponentRef Id="French"/>'
	print >>setup, r'      <Feature Id="Languages" Display="expand" Level="100"'
	print >>setup, r'               AllowAdvertise="no"'
	print >>setup, r'               Title="Languages" Description="Supported languages that can be changed while running the program (English is always available)">'
	# Need an empty component to work around a bug with features (to suppress advertising)
	print >>setup, r'        <ComponentRef Id="empty"/>'
	print >>setup, r'        <Feature Id="French" Level="100"'
	print >>setup, r'                 AllowAdvertise="no"'
	print >>setup, r'                 Title="French" Description="French">'
	print >>setup, r'          <ComponentRef Id="French"/>'
	print >>setup, r'        </Feature>'
	print >>setup, r'      </Feature>'
	print >>setup, r'      <Feature Id="Plugins" Display="expand" Level="100"'
	print >>setup, r'               AllowAdvertise="no"'
	print >>setup, r'               Title="Calendar Addins" Description="Addins to download Calendar data directly from a website">'
	# Need an empty component to work around a bug with features (to suppress advertising)
	print >>setup, r'        <ComponentRef Id="empty"/>'
	print >>setup, r'        <Feature Id="Calendar" Level="100"'
	print >>setup, r'                 AllowAdvertise="no"'
	print >>setup, r'                 Title="USDAA" Description="Download Calendar entries from usdaa.com">'
	print >>setup, r'          <ComponentRef Id="CALusdaa"/>'
	print >>setup, r'        </Feature>'
	print >>setup, r'      </Feature>'
	print >>setup, r'    </Feature>'
	print >>setup, r''
	# Just FYI of how to do this.
	#print >>setup, r'    <CustomAction Id="LaunchFile" FileKey="App" ExeCommand="/register" Return="ignore"/>'
	print >>setup, r''
	print >>setup, r'    <InstallExecuteSequence>'
	print >>setup, r'      <RemoveExistingProducts After="InstallValidate"/>'
	# FYI
	#print >>setup, r'      <Custom Action="LaunchFile" After="InstallFinalize">NOT Installed</Custom>'
	print >>setup, r'    </InstallExecuteSequence>'
	print >>setup, r''
	print >>setup, r'    <UIRef Id="WixUI_FeatureTree"/>'
	print >>setup, r'    <UIRef Id="WixUI_ErrorProgressText"/>'
	print >>setup, r''
	print >>setup, r'    <Icon Id="AgilityBook.exe" SourceFile="..\..\..\src\win\res\AgilityBook.ico"/>'
	print >>setup, r'  </Product>'
	print >>setup, r'</Wix>'

	setup.close()

	if os.access(baseDir + "AgilityBook.exe", os.F_OK):
		runcmd('candle -nologo ' + outputFile + '.wxs')
		runcmd('light -nologo -b "' + WiXdir + '" -out "' + outputFile + '.msi" ' + outputFile + '.wixobj "' + WiXdir + r'\wixui.wixlib" -loc "' + WiXdir + r'\WixUI_en-us.wxl"')
		if tidy:
			if os.access(outputFile + ".wxs", os.F_OK):
				os.remove(outputFile + ".wxs")
			if os.access(outputFile + ".wixobj", os.F_OK):
				os.remove(outputFile + ".wixobj")
	return 1


def genInno(version, version2, code, tidy):
	baseDir, outputFile = getoutputvars(code, version2)
	if tidy and not os.access(baseDir + "AgilityBook.exe", os.F_OK):
		print baseDir + "AgilityBook.exe does not exist, MSI skipped"
		return 0

	setup = open(outputFile + ".iss", "w")
	print >>setup, r'#define ARBName "Agility Record Book"'
	print >>setup, r'#define ARBVersion "' + version + '"'
	print >>setup, r''
	print >>setup, r'[Setup]'
	print >>setup, r'AppName={#ARBName}'
	print >>setup, r'AppId={#ARBName} Setup'
	print >>setup, r'AppVersion={#ARBVersion}'
	print >>setup, r'AppVerName={#ARBName} {#ARBVersion}'
	print >>setup, r'AppPublisher=David Connet'
	print >>setup, r'AppPublisherURL=http://www.agilityrecordbook.com/'
	print >>setup, r'AppSupportURL=http://www.agilityrecordbook.com/'
	print >>setup, r'AppUpdatesURL=http://www.agilityrecordbook.com/'
	print >>setup, r'DefaultDirName={pf}\dcon Software\{#ARBName}'
	print >>setup, r'DefaultGroupName={#ARBName}'
	print >>setup, r'Compression=lzma/ultra'
	print >>setup, r'InternalCompressLevel=ultra'
	print >>setup, r'SolidCompression=true'
	print >>setup, r'UninstallDisplayIcon={app}\AgilityBook.exe'
	print >>setup, r'SetupIconFile=..\..\win\res\AgilityBook.ico'
	print >>setup, r'MinVersion=4.90.3000,4.0.1381'
	print >>setup, r'PrivilegesRequired=none'
	print >>setup, r'OutputBaseFilename=ARBSetup_{#ARBVersion}'
	print >>setup, r'OutputDir=InnoSetup'
	print >>setup, r''
	print >>setup, r'[Files]'
	print >>setup, r'Source: ' + baseDir + r'AgilityBook.exe; DestDir: {app}'
	print >>setup, r'Source: ' + baseDir + r'AgilityBook.chm; DestDir: {app}'
	print >>setup, r'Source: ' + baseDir + r'AgilityBookFRA.dll; DestDir: {app}'
	print >>setup, r'Source: ' + baseDir + r'cal_usdaa.dll; DestDir: {app}'
	print >>setup, r''
	print >>setup, r'[Icons]'
	print >>setup, r'Name: {commondesktop}\{#ARBName}; Filename: {app}\AgilityBook.exe'
	print >>setup, r'Name: {group}\{#ARBName}; Filename: {app}\AgilityBook.exe; IconFilename: {app}\AgilityBook.exe; IconIndex: 0'
	print >>setup, r'Name: {group}\{#ARBName} Help; Filename: {app}\AgilityBook.chm'
	print >>setup, r'Name: {group}\{#ARBName} Web Site; Filename: http://www.agilityrecordbook.com/'
	print >>setup, r'Name: {group}\Yahoo Discussion Group; Filename: http://groups.yahoo.com/group/AgilityRecordBook/'
	print >>setup, r'Name: "{group}\{cm:UninstallProgram,{#ARBName}}"; Filename: "{uninstallexe}"'
	print >>setup, r''
	print >>setup, r'[Run]'
	print >>setup, r'Filename: "{app}\AgilityBook.exe"; Parameters: "/register"; StatusMsg: "Registering File Associations..."'
	print >>setup, r''
	print >>setup, r'[UninstallRun]'
	print >>setup, r'Filename: "{app}\AgilityBook.exe"; Parameters: "/unregister"; RunOnceId: "RemoveARBAssoc"'
	setup.close()

	if os.access(baseDir + "AgilityBook.exe", os.F_OK):
		fullpath = os.getcwd() + '\\' + outputFile + '.iss'
		runcmd('istool -compile ' + fullpath)
		if tidy:
			if os.access(outputFile + ".iss", os.F_OK):
				os.remove(outputFile + ".iss")
	return 1


def main():
	b32 = 0
	b64 = 0
	b98 = 0
	tidy = 1
	doWiX = 1
	doInno = 0
	bTesting = 0
	for i in range(1, len(sys.argv)):
		o = sys.argv[i]
		if o == "/32":
			b32 = 1
		elif o == "/64":
			b64 = 1
		elif o == "/w98":
			b98 = 1
		elif o == "/all":
			b32 = 1
			b64 = 1
			b98 = 1
		elif o == "/notidy":
			tidy = 0
		elif o == "/wix":
			doWiX = 1
			doInno = 0
		elif o == "/inno":
			doWiX = 0
			doInno = 1
		elif o == "/test":
			bTesting = 1
		else:
			print "Usage:", __doc__
			return

	if b32 + b64 + b98 == 0:
		b32 = 1
		b64 = 1
		b98 = 1

	b32ok = 0
	b64ok = 0
	b98ok = 0

	# Wix
	if doWiX:
		productId = genuuid()
		version, version2 = getversion(3)
		os.environ['PATH'] += ';' + WiXdir
		if b32:
			if genWiX(productId, version, version2, code32, tidy):
				b32ok = 1
		if b64:
			if genWiX(productId, version, version2, code64, tidy):
				b64ok = 1
		if b98:
			if genWiX(productId, version, version2, code98, tidy):
				b98ok = 1
		if not bTesting and (b32ok or b64ok or b98ok):
			d = datetime.datetime.now().isoformat(' ')
			codes = open(AgilityBookDir + r"\Misc\InstallGUIDs.csv", "a")
			installs = ""
			if b32ok:
				installs = "VC9,win32"
			if b64ok:
				installs = "VC9,x64"
			if b98ok:
				installs = "VC8,win98"
			print >>codes, "v" + version + "," + d + "," + productId + "," + UpgradeCode + "," + installs

	# Inno
	if doInno:
		version, version2 = getversion(4)
		os.environ['PATH'] += ';' + ISTool
		if b32:
			if genInno(version, version2, code32, tidy):
				b32ok = 1
		if b64:
			if genInno(version, version2, code64, tidy):
				b64ok = 1
		if b98:
			if genInno(version, version2, code98, tidy):
				b98ok = 1

main()
