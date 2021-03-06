/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Parse and cache "version2.xml" on www.agilityrecordbook.com
 * @author David Connet
 *
 * File Format: See below.
 *
 * Revision History
 * 2019-01-18 Change debugging to use ARB_WWW_ROOT instead of hardcoded path.
 *            Fix a config-only update (broken since moving to version2.xml)
 *            Fix loading local config file.
 * 2018-12-16 Convert to fmt.
 * 2014-03-05 Change wxFileSystem usage to CLibArchive.
 * 2013-12-05 Remove "?os=..." from url (website redesign)
 * 2013-10-30 Fixed a problem where arbupdater was spawned hidden.
 * 2013-10-23 Added 'minOS'.
 * 2012-03-16 Renamed LoadXML functions, added stream version.
 * 2011-07-26 Moved 'Arch' to ARBAgilityRecordBook.
 * 2010-05-07 Removed 'lang' from 'Platform'. (Finally figured out how
 *            to create transforms and embed in msi.)
 * 2010-02-07 Changed to version2.xml.
 * 2009-09-13 Add support for wxWidgets 2.9, deprecate tstring.
 * 2009-07-26 Removed Win98 support.
 * 2009-07-18 Updated version.txt to support different versions/platforms.
 * 2009-01-06 Ported to wxWidgets.
 * 2008-06-29 When looking for language ids, it was searching the wrong node.
 * 2008-01-01 Fix a bug parsing Element data (GetElementNode may return null)
 * 2007-08-03 Separated HTTP reading code from UpdateInfo.cpp
 * 2005-10-26 Added option to prevent auto-update user query.
 * 2005-09-09 Modified URL parsing to handle redirection.
 *            I can now advertise the url as "www.agilityrecordbook.com"
 * 2004-09-28 Changed how error reporting is done when loading.
 * 2004-08-03 Created
 */

/*
 * (post v2.1.4): Changed to version2.xml.
 *  See comments in code (where it's parsed).
 *
 * [v2.1.4 and earlier: version.txt]
 * line 1:
 *  "ARB Version n1.n2.n3.n4"
 * --- as of v2, the real version isn't here - it's in 'Config'
 * line 2-n: xml
 *  <!ELEMENT Data (Config+, Download*, DisableCalPlugin*) >
 *  <!--
 *  Before v1.10:
 *  pcdata is a message that can be displayed to the user explaining
 *  the changes. This is only used when doing a config update, not a
 *  version update. [version update is 1st line in file]
 *  v1.10: Moved PCDATA to 'lang' (We can change the format of the file
 *  since this data is never accessed when there's a pgm version change.)
 *  v2.0: Allow multiple copies of 'Config' for different platforms.
 *  If 'platform' is not set, it's the default. This allows creating
 *  one entry with 'mac', one with none - then all non-macs default to
 *  second. Current recognized values: 'win'/'mac'
 *  v2.0: Added 'version' to 'Config': This replaces the version in the
 *  first line (if not present, line 1 is used).
 *  -->
 *  <!ELEMENT Config (Lang+) >
 *    <!ATTLIST Config
 *      platform CDATA #IMPLIED
 *      version CDATA #IMPLIED
 *      ver CDATA #REQUIRED
 *      file CDATA #REQUIRED
 *      >
 *  <!--
 *  'id' specified the LANGID of the plugin language DLL, '0' is used
 *  for the program default.
 *  v2.0: 'id2' is gettext lang code, id no longer used.
 *  -->
 *  <!ELEMENT Lang (#PCDATA) >
 *    <!-- <!ATTLIST Lang id CDATA #REQUIRED > -->
 *    <!ATTLIST Lang id2 CDATA #REQUIRED >
 *  <!--
 *  if Download is not set, defaults to 'LinkArbDownloadUrl',
 *  which is http://www.agilityrecordbook.com/download.php
 *  -->
 *  <!ELEMENT Download (#PCDATA) >
 *  <!--
 *  When we know a calendar plugin is obsolete due to website changes,
 *  use this to force the plugin to disable. Only disables the specified
 *  version. If we prematurely disabled the plugin, the 'enable' flag
 *  will reenable. [disabling info is stored in the registry]
 *  -->
 *  <!ELEMENT DisableCalPlugin EMPTY >
 *    <!ATTLIST DisableCalPlugin
 *      file CDATA #REQUIRED
 *      ver CDATA #REQUIRED
 *      enable (0|1) '1'
 *      >
 */

#include "stdafx.h"
#include "UpdateInfo.h"

#include "AgilityBook.h"
#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "ConfigHandler.h"
#include "RegItems.h"
#include "VersionNumber.h"
#include <fstream>

#include "ARBCommon/ARBMisc.h"
#include "ARBCommon/ARBMsgDigest.h"
#include "ARBCommon/ARBUtils.h"
#include "ARBCommon/Element.h"
#include "ARBCommon/VersionNum.h"
#include "LibARBWin/DlgProgress.h"
#include "LibARBWin/ReadHttp.h"
#include "LibArchive/LibArchive.h"
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>

#ifdef WIN32
#include <wuapi.h>
#endif

#include "Platform/arbWarningPush.h"

#include <wx/zipstrm.h>

#include "Platform/arbWarningPop.h"

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

static wxString VersionFile()
{
	return L"version2.xml";
}


#if defined(__WXMSW__)
static wxString ARBUpdater()
{
	return L"ARBUpdater.exe";
}


static wxString UpdateFile()
{
	return wxFileName::GetTempDir() + wxFileName::GetPathSeparator() + ARBUpdater();
}
#endif


static bool GetARBFilename(wxString const& filename, std::wstring& fullpath)
{
	fullpath.clear();

	bool bUseLocal = false;
#ifdef _DEBUG
	/// ARB_WWW_ROOT points to the directory where version2.xml lives.
	wxString path;
	if (wxGetEnv(L"ARB_WWW_ROOT", &path))
	{
		wxFileName file(path, filename);
		fullpath = StringUtil::stringW(file.GetFullPath());
		bUseLocal = true;
	}
#endif
	if (!bUseLocal)
		fullpath = fmt::format(L"http://www.agilityrecordbook.com/{}", filename);

	return bUseLocal;
}

/////////////////////////////////////////////////////////////////////////////

bool CUpdateInfo::UpdateConfig(CAgilityBookDoc* ioDoc, wchar_t const* inMsg)
{
	wxString msg(_("IDS_UPDATED_CONFIG"));
	if (inMsg && *inMsg)
	{
		msg += L"\n\n";
		msg += inMsg;
	}

	bool b = (wxYES == wxMessageBox(msg, wxMessageBoxCaptionStr, wxYES_NO | wxCENTRE | wxICON_QUESTION));
	/* TODO: Change to include 'never' option
	if (!b)
	{
		ioDoc->GetConfig().SetUpdateStatus(false);
		ioDoc->Modify(true);
	}
	*/
	return b;
}


void CUpdateInfo::CleanupUpdate()
{
#if defined(__WXMSW__)
	wxString updateFile = UpdateFile();
	if (wxFileName::FileExists(updateFile))
	{
		if (!::wxRemoveFile(updateFile))
		{
			// If we get here really quick, the updater may not have finished
			// yet. Go to the Win API now.
			Sleep(3000);
			if (!DeleteFile(updateFile.wx_str()))
			{
				// Ok, delete failed after 3 seconds. Punt (quietly).
				MoveFileEx(updateFile.wx_str(), nullptr, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

CUpdateInfo::CUpdateInfo()
	: m_VersionNum()
	, m_VerConfig(0)
	, m_size(0)
	, m_hash()
	, m_hashType(ARBMsgDigest::ARBDigest::Unknown)
	, m_NewFile()
	, m_ConfigFileName()
	, m_InfoMsg()
	, m_UpdateDownload()
	, m_usernameHint(L"default")
	, m_CalSiteSuppression()
{
}


/**
 * This will read the version2.xml file and cache it.
 * In addition, it will ask to update if a newer version is found.
 * @param bVerbose Show error message.
 */
bool CUpdateInfo::ReadVersionFile(bool bVerbose)
{
	// Clear everything.
	m_VersionNum.clear();
	m_VerConfig = 0;
	m_size = 0;
	m_hash.clear();
	m_hashType = ARBMsgDigest::ARBDigest::Unknown;
	m_NewFile.clear();
	m_ConfigFileName.clear();
	m_InfoMsg.clear();
	m_UpdateDownload.clear();
	m_usernameHint = L"default";
	m_CalSiteSuppression.clear();

	// Set the default values.
	m_UpdateDownload = _("LinkArbDownloadUrl");

	// Read the file.
	std::wstring url;
	bool bUseLocal = GetARBFilename(VersionFile(), url);
	std::string data; // must be 'char' for XML parsing
	std::wstring errMsg;

	if (bUseLocal)
	{
		wxTextFile file;
		if (file.Open(url))
		{
			for (size_t line = 0; line < file.GetLineCount(); ++line)
			{
				data += file[line].ToUTF8();
				data += '\n';
			}
			file.Close();
		}
	}
	else
	{
		CReadHttp file;
		//std::wstring userName = CAgilityBookOptions::GetUserName(m_usernameHint);
		if (file.ReadHttpFileSync(errMsg, url, data))
		{
			//CAgilityBookOptions::SetUserName(m_usernameHint, userName);
		}
		else
		{
			if (bVerbose)
			{
				data = wxString(_("IDS_UPDATE_UNKNOWN")).mb_str(wxMBConvUTF8());
				wxString tmp = StringUtil::stringWX(data);
				if (!errMsg.empty())
					tmp << errMsg.c_str();
				wxMessageBox(tmp, wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_EXCLAMATION);
			}
			return false;
		}
	}

	bool bLoadedVersion = false;
	if (!data.empty())
	{
		/*
		 * <!ELEMENT Data (Platform, Config, Download*, DisableCalPlugin*) >
		 * <!ELEMENT Platform (EMPTY) >
		 *   <!--
		 *   Important: 'arch' elements for must be ordered in decending
		 *   order on 'minOS'. On Windows, this is just a coarse check.
		 *   If there is an SP requirement, the installer will catch it. Not
		 *   a good UI experience, but there's only so much that can be done!
		 *   Right now, XP has the SP requirements. The only issue is if a future
		 *   version (let's say 8.1sp3). If that happens, then I can rename the
		 *   'arch' tags again and deal with it then (like adding support for
		 *   'minOS' is happening now).
		 *   -->
		 *   <!ATTLIST Platform
		 *     arch CDATA (win32,win64,osx) [pre-v2.4.0: x86,x64,mac]
		 *     minOS CDATA #IMPLIED (maj.min)
		 *     ver CDATA #REQUIRED
		 *     config CDATA #REQUIRED
		 *     md5 CDATA #REQUIRED [one of md5/sha1/sha256 is required]
		 *     sha1 CDATA #REQUIRED [one of md5/sha1/sha256 is required]
		 *     sha256 CDATA #REQUIRED [one of md5/sha1/sha256 is required]
		 *     file CDATA #REQUIRED >
		 * <!ELEMENT Config (Lang+) >
		 *   <!ATTLIST Config
		 *     ver CDATA #REQUIRED
		 *     file CDATA #REQUIRED
		 *     >
		 * <!ELEMENT Lang (#PCDATA) >
		 *   <!ATTLIST Lang id2 CDATA #REQUIRED >
		 *  <!--
		 *  if Download is not set, defaults to 'LinkArbDownloadUrl',
		 *  which is http://www.agilityrecordbook.com/download.php
		 *  -->
		 *  <!ELEMENT Download (#PCDATA) >
		 *  <!--
		 *  When we know a calendar plugin is obsolete due to website changes,
		 *  use this to force the plugin to disable. Only disables the specified
		 *  version. If we prematurely disabled the plugin, the 'enable' flag
		 *  will reenable. [disabling info is stored in the registry]
		 *  -->
		 *  <!ELEMENT DisableCalPlugin EMPTY >
		 *    <!ATTLIST DisableCalPlugin
		 *      file CDATA #REQUIRED
		 *      ver CDATA #REQUIRED
		 *      enable (0|1) '1'
		 *      >
		 */
		fmt::wmemory_buffer errMsg2;
		ElementNodePtr tree(ElementNode::New());
		bool bOk = false;
		{
			wxBusyCursor wait;
			bOk = tree->LoadXML(data.c_str(), data.length(), errMsg2);
		}
		if (!bOk)
		{
			if (bVerbose)
			{
				fmt::wmemory_buffer msg;
				fmt::format_to(msg, _("IDS_LOAD_FAILED").wx_str(), VersionFile());
				if (0 < errMsg2.size())
				{
					fmt::format_to(msg, L"\n\n{}", fmt::to_string(errMsg2));
				}
				wxMessageBox(fmt::to_string(msg), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_EXCLAMATION);
			}
		}
		else if (tree->GetName() == L"Data")
		{
			bool bConfigLoaded = false;
			for (int nIndex = 0; nIndex < tree->GetElementCount(); ++nIndex)
			{
				ElementNodePtr node = tree->GetElementNode(nIndex);
				if (!node)
					continue;
				if (!bLoadedVersion && node->GetName() == L"Platform")
				{
					bool bSkip = true;
					std::wstring value;
					if (ARBAttribLookup::Found == node->GetAttrib(L"arch", value))
					{
						if (ARBAgilityRecordBook::GetArch() == value)
						{
							bSkip = false;
							ARBVersion minOS;
							if (ARBAttribLookup::Found == node->GetAttrib(L"minOS", minOS))
							{
								int majVer;
								int minVer;
								GetOSInfo(majVer, minVer);
								ARBVersion ver(
									static_cast<unsigned short>(majVer),
									static_cast<unsigned short>(minVer));
								if (ver < minOS)
									bSkip = true;
							}
							else
							{
								// It's an error, but don't expose this to the user.
								bSkip = true;
							}
						}
					}
					// Wrong architecture
					if (bSkip)
						continue;
					if (ARBAttribLookup::Found == node->GetAttrib(L"ver", value))
					{
						m_VersionNum.Parse(value);
						if (m_VersionNum.Valid())
							bLoadedVersion = true;
					}
					if (ARBAttribLookup::Found != node->GetAttrib(L"config", m_VerConfig)
						|| ARBAttribLookup::Found != node->GetAttrib(L"size", m_size)
						|| ARBAttribLookup::Found != node->GetAttrib(L"file", m_NewFile))
					{
						bLoadedVersion = false;
					}
					else
					{
						m_ConfigFileName = fmt::format(L"Config{}.txt", m_VerConfig);
						if (ARBAttribLookup::Found == node->GetAttrib(L"sha256", m_hash))
							m_hashType = ARBMsgDigest::ARBDigest::SHA256;
						else if (ARBAttribLookup::Found == node->GetAttrib(L"sha1", m_hash))
							m_hashType = ARBMsgDigest::ARBDigest::SHA1;
						else if (ARBAttribLookup::Found == node->GetAttrib(L"md5", m_hash))
							m_hashType = ARBMsgDigest::ARBDigest::MD5;
						else
							bLoadedVersion = false;
					}

					if (!bLoadedVersion)
					{
						if (bVerbose)
						{
							wxMessageBox(
								_("IDS_UPDATE_UNKNOWN"),
								wxMessageBoxCaptionStr,
								wxOK | wxCENTRE | wxICON_EXCLAMATION);
						}
						return false;
					}
				}
				else if (!bConfigLoaded && node->GetName() == L"Config")
				{
					short config;
					node->GetAttrib(L"ver", config);
					if (config == m_VerConfig)
					{
						wxString value;
						bConfigLoaded = true;
						node->GetAttrib(L"file", m_ConfigFileName);
						for (int nLang = 0; nLang < node->GetElementCount(); ++nLang)
						{
							ElementNodePtr lang = node->GetElementNode(nLang);
							if (!lang)
								continue;
							if (lang->GetName() == L"Lang")
							{
								std::wstring langIdStr;
								lang->GetAttrib(L"id2", langIdStr);
								m_InfoMsg[langIdStr] = lang->GetValue();
							}
						}
					}
				}
				else if (node->GetName() == L"Download")
				{
					m_UpdateDownload = node->GetValue();
				}
				else if (node->GetName() == L"DisableCalPlugin")
				{
					std::wstring filename, ver;
					node->GetAttrib(L"file", filename);
					node->GetAttrib(L"ver", ver);
					// The 'enable' attribute is in case we prematurely disable
					bool bEnable = false;
					node->GetAttrib(L"enable", bEnable);
					CVersionNum vernum;
					vernum.Parse(ver);
					if (vernum.Valid())
					{
						CAgilityBookOptions::SuppressCalSitePermanently(filename, vernum, !bEnable);
					}
				}
			}
		}
	}

	return bLoadedVersion;
}


/**
 * Check the version against the web.
 */
bool CUpdateInfo::CheckProgram(CAgilityBookDoc* pDoc, std::wstring const& lang, bool& outClose, bool& canInstall)
{
	outClose = false;
	canInstall = CanInstall();
	bool bNeedsUpdating = false;
	ARBDate today = ARBDate::Today();
	if (IsOutOfDate())
	{
		bNeedsUpdating = true;
		if ((pDoc && !pDoc->OnSaveModified()) || !canInstall)
		{
			// Return that it needs updating, but don't record that we checked.
			return bNeedsUpdating;
		}
		wxConfig::Get()->Write(CFG_SETTINGS_LASTVERCHECK, today.GetString(ARBDateFormat::ISO).c_str());
		std::wstring msg = fmt::format(_("IDS_VERSION_AVAILABLE").wx_str(), m_VersionNum.GetVersionString());
		if (wxYES == wxMessageBox(msg, wxMessageBoxCaptionStr, wxYES_NO | wxCENTRE | wxICON_QUESTION))
		{
			bool bGotoWeb = false;
			// If running in standalone mode, just go to the web.
			if (m_NewFile.empty() || wxGetApp().IsStandloneMode())
				bGotoWeb = true;
			else
			{
				wxFileName name(StringUtil::stringWX(m_NewFile));
				wxString filename;
#if defined(__WXMAC__)
				wxFileDialog dlg(
					wxGetApp().GetTopWindow(),
					wxFileSelectorPromptStr,
					wxStandardPaths::Get().GetDocumentsDir(),
					name.GetFullName(),
					wxFileSelectorDefaultWildcardStr,
					wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
				if (wxID_OK == dlg.ShowModal())
				{
					filename = dlg.GetPath();
				}
				else
				{
					bGotoWeb = true;
				}
#elif defined(__WXMSW__)
				filename = wxFileName::GetTempDir() + wxFileName::GetPathSeparator() + name.GetFullName();
#endif
				if (!bGotoWeb)
				{
					fmt::wmemory_buffer errMsg;
					wxFileOutputStream output(filename);
					if (!output.IsOk())
					{
						bGotoWeb = true;
						fmt::format_to(errMsg, _("IDS_CANNOT_OPEN").wx_str(), filename.wx_str());
					}
					else
					{
						IDlgProgress* progress = IDlgProgress::CreateProgress(1, wxGetApp().GetTopWindow());
						progress->SetCaption(StringUtil::stringW(_("IDS_DOWNLOADING")));
						progress->SetRange(1, m_size);
						progress->SetMessage(StringUtil::stringW(name.GetFullName()));
						progress->ShowProgress();
						progress->SetForegroundWindow();

						CReadHttp http;
						std::wstring err;
						if (!http.ReadHttpFileSync(err, m_NewFile, &output, progress))
						{
							bGotoWeb = true;
							// If user canceled, no message is generated.
							if (!err.empty())
							{
								if (0 < errMsg.size())
									fmt::format_to(errMsg, L"\n\n");
								fmt::format_to(errMsg, L"{}", err);
							}
						}
						output.Close();
						if (!bGotoWeb && !m_hash.empty())
						{
							progress->SetCaption(StringUtil::stringW(_("IDS_VALIDATING")));
							wxFileInputStream file(filename);
							wxStdInputStream stdfile(file);
							if (ARBMsgDigest::Compute(stdfile, m_hashType) != m_hash)
							{
								bGotoWeb = true;
								if (0 < errMsg.size())
									fmt::format_to(errMsg, L"\n\n");
								fmt::format_to(errMsg, L"{}", _("IDS_ERROR_DOWNLOAD").wx_str());
							}
						}
						if (bGotoWeb)
							::wxRemoveFile(filename);
						progress->Dismiss();
					}
					if (0 < errMsg.size() && bGotoWeb)
					{
						wxMessageBox(fmt::to_string(errMsg));
					}
				}
				if (!bGotoWeb)
				{
#if defined(__WXMAC__)
					outClose = true;
					wxMessageBox(fmt::format(_("IDS_DOWNLOAD_AND_RESTART").wx_str(), filename));
#elif defined(__WXMSW__)
					wxString msiFilename;
					{ // Can't remove file until stream is closed.
						wxFileInputStream inStream(filename);
						if (!inStream.IsOk())
						{
							bGotoWeb = true;
							wxMessageBox(_("IDS_ERROR_DOWNLOAD"));
						}
						else
						{
							wxZipInputStream inZip(inStream);
							if (!inZip.IsOk())
							{
								bGotoWeb = true;
								wxMessageBox(_("IDS_ERROR_DOWNLOAD"));
							}
							else
							{
								wxZipEntry* entry = inZip.GetNextEntry();
								if (!entry)
								{
									bGotoWeb = true;
									wxMessageBox(_("IDS_ERROR_DOWNLOAD"));
								}
								else
								{
									wxFileName msiName(entry->GetName());
									if (msiName.GetExt().Upper() != L"MSI")
									{
										bGotoWeb = true;
										wxMessageBox(_("IDS_ERROR_UNEXPECTED"));
									}
									else
									{
										msiFilename = wxFileName::GetTempDir() + wxFileName::GetPathSeparator()
													  + msiName.GetFullName();
										wxFileOutputStream output(msiFilename);
										inZip.Read(output);
									}
									delete entry;
								}
								inZip.CloseEntry();
							}
						}
					}
					::wxRemoveFile(filename);
					if (!bGotoWeb)
					{
						wxString updater = UpdateFile();
						{
							std::wstring outFile(StringUtil::stringW(updater));
#if defined(ARB_HAS_OSTREAM_WCHAR)
							std::ofstream output(outFile.c_str(), std::ios::out | std::ios::binary);
#else
							char const* pFile = nullptr;
							std::string filename = StringUtil::stringA(outFile);
							pFile = filename.c_str();
							std::ofstream output(pFile, std::ios::out | std::ios::binary);
#endif
							wxFileName fileName(wxStandardPaths::Get().GetExecutablePath());
							wxString zipfile
								= GetARBResourceDir() + wxFileName::GetPathSeparator() + fileName.GetName() + L".dat";
							CLibArchive filesys(StringUtil::stringW(zipfile));
							if (!filesys.ExtractFile(StringUtil::stringW(ARBUpdater()), output))
							{
								bGotoWeb = true;
								wxMessageBox(_("IDS_ERROR_AUTOUPDATE"));
							}
						}
						if (!bGotoWeb)
						{
							std::wstring args = fmt::format(L"-f \"{}\"", msiFilename.wx_str());
							SHELLEXECUTEINFO info;
							ZeroMemory(&info, sizeof(info));
							info.cbSize = sizeof(info);
							info.lpVerb = L"open";
							info.lpFile = updater.wx_str();
							info.lpParameters = args.c_str();
							info.nShow = SW_SHOWNORMAL;
							if (ShellExecuteEx(&info))
								outClose = true;
							else
							{
								bGotoWeb = true;
								::wxRemoveFile(updater);
								wxMessageBox(_("IDS_ERROR_AUTOUPDATE"));
							}
						}
					}
#endif
				}
			}
			if (bGotoWeb)
			{
				wxLaunchDefaultBrowser(StringUtil::stringWX(m_UpdateDownload));
			}
		}
	}
	else
		wxConfig::Get()->Write(CFG_SETTINGS_LASTVERCHECK, today.GetString(ARBDateFormat::ISO).c_str());
	return bNeedsUpdating;
}


/**
 * Check the version against the web.
 */
bool CUpdateInfo::IsOutOfDate()
{
	// If we haven't parsed the internet file yet, assume we're out-of-date.
	if (!m_VersionNum.Valid())
		return true;
	CVersionNum verThis(ARB_VER_MAJOR, ARB_VER_MINOR, ARB_VER_DOT, ARB_VER_BUILD);
	assert(verThis.Valid());
	if (verThis < m_VersionNum)
		return true;
	else
		return false;
}


/**
 * Can we install?
 * Check to see if Window Update is in process.
 */
bool CUpdateInfo::CanInstall() const
{
	bool bAllowInstall = true;
#ifdef WIN32
	// Note: CoInitialize already called
	IUpdateInstaller* iInstaller = nullptr;
	if (SUCCEEDED(CoCreateInstance(
			CLSID_UpdateInstaller,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_IUpdateInstaller,
			(LPVOID*)&iInstaller)))
	{
		VARIANT_BOOL bVal;
		if (SUCCEEDED(iInstaller->get_IsBusy(&bVal)))
		{
			if (bVal)
				bAllowInstall = false;
		}
		if (SUCCEEDED(iInstaller->get_RebootRequiredBeforeInstallation(&bVal)))
		{
			if (bVal)
				bAllowInstall = false;
		}

		iInstaller->Release();
	}
#endif
	return bAllowInstall;
}


void CUpdateInfo::CheckConfig(CAgilityBookDoc* pDoc, bool bVerbose)
{
	// If the parse was successful, check for the posted config version.
	bool bUpToDate = true;
	// Cool! New config!
	if (0 < m_ConfigFileName.length() && m_VerConfig > pDoc->Book().GetConfig().GetVersion())
	{
		bUpToDate = false;
		std::wstring msg;
		if (0 < m_InfoMsg.size())
		{
			std::map<std::wstring, std::wstring>::iterator iMsg = m_InfoMsg.find(wxGetApp().CurrentLanguage());
			if (iMsg == m_InfoMsg.end())
				iMsg = m_InfoMsg.begin();
			if (iMsg != m_InfoMsg.end() && 0 < iMsg->second.length())
			{
				// If the info contains a note, append it.
				// A note will often give a brief description of things
				// the user must do. For instance, from v4->5, USDAA
				// titling pts were removed from Tournament Jumpers and
				// Snooker to allow for non-titling runs. In case the
				// user saved some that way, we need to warn them.
				msg += iMsg->second;
			}
		}
		if (UpdateConfig(pDoc, msg.c_str()))
		{
			// Load the config.
			std::wstring url;
			bool bUseLocal = GetARBFilename(m_ConfigFileName, url);
			std::string strConfig;
			std::wstring errMsg;

			if (bUseLocal)
			{
				wxTextFile file;
				if (file.Open(url))
				{
					for (size_t line = 0; line < file.GetLineCount(); ++line)
					{
						strConfig += file[line].ToUTF8();
						strConfig += '\n';
					}
					file.Close();
				}
			}
			else
			{
				CReadHttp file;
				//std::wstring userName = CAgilityBookOptions::GetUserName(m_usernameHint);
				if (file.ReadHttpFileSync(errMsg, url, strConfig))
				{
					//CAgilityBookOptions::SetUserName(m_usernameHint, userName);
				}
				else
				{
					strConfig.clear();
					// TODO: if (bVerbose), error msg (like above)
					return;
				}
			}

			ElementNodePtr tree(ElementNode::New());
			fmt::wmemory_buffer errMsg2;
			bool bOk = false;
			{
				wxBusyCursor wait;
				bOk = tree->LoadXML(strConfig.c_str(), strConfig.length(), errMsg2);
			}
			if (!bOk)
			{
				fmt::wmemory_buffer msg2;
				fmt::format_to(msg2, _("IDS_LOAD_FAILED").wx_str(), url);
				if (0 < errMsg2.size())
				{
					fmt::format_to(msg2, L"\n\n{}", fmt::to_string(errMsg2));
				}
				wxMessageBox(fmt::to_string(msg2), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_EXCLAMATION);
			}
			else if (tree->GetName() == L"DefaultConfig")
			{
				strConfig.clear();
				ARBVersion version = ARBAgilityRecordBook::GetCurrentDocVersion();
				tree->GetAttrib(ATTRIB_BOOK_VERSION, version);
				int nConfig = tree->FindElement(TREE_CONFIG);
				if (0 <= nConfig)
				{
					CErrorCallback err;
					ARBAgilityRecordBook book;
					if (!book.GetConfig().Load(tree->GetElementNode(nConfig), version, err))
					{
						if (0 < err.m_ErrMsg.size())
							wxMessageBox(
								StringUtil::stringWX(fmt::to_string(err.m_ErrMsg)),
								wxMessageBoxCaptionStr,
								wxOK | wxCENTRE | wxICON_WARNING);
					}
					else
					{
						if (pDoc->ImportConfiguration(book.GetConfig()))
							pDoc->Modify(true);
					}
				}
			}
		}
	}
	if (bUpToDate && bVerbose)
	{
		wxMessageBox(_("IDS_UPDATE_CURRENT"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_INFORMATION);
	}
}


void CUpdateInfo::AutoUpdateProgram(CAgilityBookDoc* pDoc, bool& outClose)
{
	outClose = false;
	if (ReadVersionFile(false))
	{
		// When checking if we can install, silently ignore the case when we
		// are due to update, but can't because Windows is updating.
		bool canInstall;
		CheckProgram(pDoc, wxGetApp().CurrentLanguage(), outClose, canInstall);
	}
}


void CUpdateInfo::AutoCheckConfiguration(CAgilityBookDoc* pDoc)
{
	// If we're opening a doc and we've checked the internet
	// and there is a more current version, do not continue
	// to check the config. This prevents loading a config that
	// uses a newer file version.
	if (IsOutOfDate())
		return;
	CheckConfig(pDoc, false);
}


void CUpdateInfo::UpdateConfiguration(CAgilityBookDoc* pDoc, bool& outClose)
{
	outClose = false;
	// Only continue if we parsed the version.txt file
	// AND the version is up-to-date.
	if (!ReadVersionFile(true))
		return;
	bool canInstall;
	if (CheckProgram(pDoc, wxGetApp().CurrentLanguage(), outClose, canInstall))
	{
		if (!canInstall)
			wxMessageBox(_("IDS_INSTALL_BUSY"), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_WARNING);
		return;
	}
	CheckConfig(pDoc, true);
}
