/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Defines the class behaviors for the application.
 * @author David Connet
 *
 * Revision History
 * @li 2012-12-15 DRC Prevent TabView from getting activation.
 * @li 2012-08-28 DRC Rework how wxCmdLineParser is initialized.
 * @li 2012-07-27 DRC Disable spell checking on OSX 10.8. It crashes.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-26 DRC Fix file autoload failure so it opens new document.
 * @li 2008-12-14 DRC Ported to wxWidgets.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-10-19 DRC Fixed a problem with CFile::GetStatus (see AgilityBook.cpp).
 * @li 2005-02-10 DRC There was a problem initializing RICHED*.DLL on Win98.
 * @li 2004-07-20 DRC Changed the updating to auto-update configurations also.
 *                    Moved the user-request updates to the document.
 * @li 2004-06-16 DRC Changed ARBDate::GetString to put leadingzero into format.
 * @li 2004-06-02 DRC Moved ShellExecute code here.
 * @li 2004-03-05 DRC Added check-for-updates feature.
 * @li 2003-12-07 DRC When opening the last opened file fails, open a new doc.
 */

#include "stdafx.h"
#include "AgilityBook.h"

#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "Element.h"
#include "ImageManager.h"
#include "LanguageManager.h"
#include "MainFrm.h"
#include "Print.h"
#include "RegItems.h"
#include "StringUtil.h"
#include "TabView.h"
#include <vector>
#include <wx/choicdlg.h>
#include <wx/cmdline.h>
#include <wx/cmndata.h>
#include <wx/config.h>
#include <wx/display.h>
#include <wx/docview.h>
#include <wx/file.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/stdpaths.h>
#include <wx/sysopt.h>
#include <wx/url.h>
#include <wx/utils.h>
#include <wx/xrc/xmlres.h>	// XRC XML resources

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif


IMPLEMENT_APP(CAgilityBookApp)

/////////////////////////////////////////////////////////////////////////////

class CAgilityBookDocManager : public wxDocManager
{
	CAgilityBookDocManager(); // not implemented
public:
	CAgilityBookDocManager(size_t historySize);
	virtual wxFileHistory* OnCreateFileHistory();
	virtual void ActivateView(wxView *view, bool activate = true);
private:
	size_t m_History;

	DECLARE_EVENT_TABLE()
	void OnPrint(wxCommandEvent& evt);
	void OnPreview(wxCommandEvent& evt);
};


BEGIN_EVENT_TABLE(CAgilityBookDocManager, wxDocManager)
	EVT_MENU(wxID_PRINT, CAgilityBookDocManager::OnPrint)
	EVT_MENU(wxID_PREVIEW, CAgilityBookDocManager::OnPreview)
END_EVENT_TABLE()


CAgilityBookDocManager::CAgilityBookDocManager(size_t historySize)
	: m_History(historySize)
{
	if (wxMAX_FILE_HISTORY < m_History)
		m_History = wxMAX_FILE_HISTORY;
}


wxFileHistory* CAgilityBookDocManager::OnCreateFileHistory()
{
	return new wxFileHistory(m_History);
}


void CAgilityBookDocManager::ActivateView(wxView *view, bool activate)
{
	CTabView* pTabView = wxDynamicCast(view, CTabView);
	if (!pTabView)
		wxDocManager::ActivateView(view, activate);
}


void CAgilityBookDocManager::OnPrint(wxCommandEvent& evt)
{
	// Copied wxDocManager implementation so I could put my printer data in
	wxView *view = GetCurrentView();
	if (!view)
		return;
	wxPrintout *printout = view->OnCreatePrintout();
	if (printout)
	{
		wxPrinter printer(wxGetApp().GetPrintData());
		printer.Print(view->GetFrame(), printout, true);
		wxGetApp().SavePrintData(printer.GetPrintDialogData());
		delete printout;
	}
}


void CAgilityBookDocManager::OnPreview(wxCommandEvent& evt)
{
	// Copied wxDocManager implementation so I could put my printer data in
	wxView *view = GetCurrentView();
	if (!view)
		return;
	wxPrintout *printout = view->OnCreatePrintout();
	if (printout)
	{
		// Pass two printout objects: for preview, and possible printing.
		wxPrintPreviewBase *preview = new CPrintPreview(printout, view->OnCreatePrintout(), wxGetApp().GetPrintData());
		if (!preview->Ok())
		{
			delete preview;
			wxMessageBox(_("Sorry, print preview needs a printer to be installed."));
			return;
		}
		wxPreviewFrame *frame = new wxPreviewFrame(preview, wxGetApp().GetTopWindow(), _("Print Preview"), wxDefaultPosition, wxGetApp().GetTopWindow()->GetSize());
		frame->Centre(wxBOTH);
		frame->Initialize();
		frame->Show(true);
	}
}

/////////////////////////////////////////////////////////////////////////////

CAgilityBookApp::CAgilityBookApp()
	: m_LangMgr(NULL)
	, m_UpdateInfo()
	, m_manager(NULL)
	, m_printDialogData(NULL)
	, m_Prn(NULL)
{
}


bool CAgilityBookApp::SelectLanguage(wxWindow* parent)
{
	assert(m_LangMgr);
	return m_LangMgr->SelectLanguage(parent);
}


void CAgilityBookApp::AutoCheckConfiguration(CAgilityBookDoc* pDoc)
{
	assert(m_LangMgr);
	m_UpdateInfo.AutoCheckConfiguration(pDoc, *m_LangMgr);
}


void CAgilityBookApp::UpdateConfiguration(
		CAgilityBookDoc* pDoc,
		bool& outClose)
{
	assert(m_LangMgr);
	m_UpdateInfo.UpdateConfiguration(pDoc, *m_LangMgr, outClose);
}


void CAgilityBookApp::SetMessageText(
		std::wstring const& msg,
		bool bFiltered)
{
	CMainFrame* pFrame = wxDynamicCast(GetTopWindow(), CMainFrame);
	if (pFrame)
	{
		pFrame->SetMessageText(msg, bFiltered);
	}
}


void CAgilityBookApp::SetMessageText2(std::wstring const& msg)
{
	CMainFrame* pFrame = wxDynamicCast(GetTopWindow(), CMainFrame);
	if (pFrame)
	{
		pFrame->SetMessageText2(msg);
	}
}


wxPrintDialogData* CAgilityBookApp::GetPrintData()
{
	if (!m_printDialogData)
	{
		m_printDialogData = new wxPrintDialogData();
		bool val = true;
		wxConfig::Get()->Read(CFG_SETTINGS_PRINTLAND, &val);
		m_printDialogData->GetPrintData().SetOrientation(val ? wxLANDSCAPE : wxPORTRAIT);
	}
	return m_printDialogData;
}


void CAgilityBookApp::SavePrintData(wxPrintDialogData const& data)
{
	assert(m_printDialogData);
	*m_printDialogData = data;
	bool val = (m_printDialogData->GetPrintData().GetOrientation() == wxLANDSCAPE);
	wxConfig::Get()->Write(CFG_SETTINGS_PRINTLAND, val);
}


CHtmlEasyPrinting* CAgilityBookApp::GetHtmlPrinter()
{
	if (!m_Prn)
		m_Prn = new CHtmlEasyPrinting(GetTopWindow());
	return m_Prn;
}


bool CAgilityBookApp::OnInit()
{
#if defined(__WXMSW__)
	// By default, the path directories are tweaked to remove debug/release.
	// I assume my files are in the same location as the binary.
	// Now I don't need to tweak the wx source!
	wxStandardPaths::Get().DontIgnoreAppSubDir();
#endif

	std::wstring errMsg;
	if (!Element::Initialize(errMsg))
	{
		wxMessageBox(errMsg.c_str(), wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxICON_ERROR);
		return false;
	}

	// We need at least 800x600 (the event(run) dialog is big!)
	if (wxSYS_SCREEN_DESKTOP != wxSystemSettings::GetScreenType())
	{
		if (wxNO == wxMessageBox(_("IDS_MIN_RESOLUTION"), wxMessageBoxCaptionStr, wxYES_NO | wxCENTRE | wxICON_ERROR))
			return false;
	}

	SetAppName(L"Agility Record Book");
	wxConfig::Set(new wxConfig(L"Agility Record Book", L"dcon Software"));

	wxImage::AddHandler(new wxGIFHandler);
	wxFileSystem::AddHandler(new wxZipFSHandler);
	wxXmlResource::Get()->InitAllHandlers();
	m_LangMgr = new CLanguageManager();

	wxCmdLineParser cmdline(argc, argv);
	cmdline.AddParam(_("Agility Record Book file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	if (0 != cmdline.Parse(false))
	{
		cmdline.Usage();
		Element::Terminate();
		delete m_LangMgr;
		m_LangMgr = NULL;
		return false;
	}

	wxString filename;
	if (0 < cmdline.GetParamCount())
	{
		filename = cmdline.GetParam(0);
		// Normalize filename - windows sometimes passes in short form
		wxFileName fName(filename);
		fName.Normalize(wxPATH_NORM_LONG | wxPATH_NORM_DOTS | wxPATH_NORM_TILDE | wxPATH_NORM_ABSOLUTE);
		filename = fName.GetFullPath();
	}

	m_manager = new CAgilityBookDocManager(CAgilityBookOptions::GetMRUFileCount());
	m_manager->SetMaxDocsOpen(1);
	wxConfig::Get()->SetPath(L"Recent File List"); // Named this way for compatibility with existing MFC app
	m_manager->FileHistoryLoad(*wxConfig::Get());
	wxConfig::Get()->SetPath(L"..");

	(void)new wxDocTemplate(m_manager, L"ARB", L"*.arb", L"", L"arb", L"ARB Doc", L"ARB View",
		CLASSINFO(CAgilityBookDoc), CLASSINFO(CTabView));
#ifdef __WXMAC__
#ifndef __WXOSX_COCOA__
	wxFileName::MacRegisterDefaultTypeAndCreator(L"arb", 'ARBB', 'ARBA');
#endif
	{
		bool useSpellChecker = true;
#ifndef __WXOSX_COCOA__
		wxPlatformInfo info;
		int majVer = info.GetOSMajorVersion();
		int minVer = info.GetOSMinorVersion();
#if !wxCHECK_VERSION(2, 9, 4)
		majVer = (majVer == 16 ? 10 : majVer);
		minVer = (minVer >> 4);
#endif
		if (majVer == 10 && minVer < 8)
		{
			useSpellChecker = false;
		}
#endif
		if (useSpellChecker)
		{
			wxSystemOptions::SetOption(wxMAC_TEXTCONTROL_USE_SPELL_CHECKER, 1);
		}
	}
	// Sorting is broken in the native sorting in wx 2.8.10 and earlier
	wxSystemOptions::SetOption(L"mac.listctrl.always_use_generic", 1);
#endif

	CMainFrame *frame = new CMainFrame(m_manager);

	int x, y, width, height;
	x = y = wxDefaultCoord;
	frame->GetSize(&width, &height);
	int defWidth = width;
	int defHeight = height;
	wxConfig::Get()->Read(CFG_SETTINGS_LASTXPOS, &x, x);
	wxConfig::Get()->Read(CFG_SETTINGS_LASTYPOS, &y, y);
	wxConfig::Get()->Read(CFG_SETTINGS_LASTCX, &width, width);
	wxConfig::Get()->Read(CFG_SETTINGS_LASTCY, &height, height);
	long state = wxConfig::Get()->Read(CFG_SETTINGS_LASTSTATE, 0L);
	bool bCompute = false;
	wxMouseState mouseState = ::wxGetMouseState();
	if (wxDefaultCoord != x)
	{
		bCompute = true;
		mouseState.SetX(x);
	}
	if (wxDefaultCoord != y)
	{
		bCompute = true;
		mouseState.SetY(y);
	}
	wxSize curSize(defWidth, defHeight);
	if (defWidth != width)
	{
		bCompute = true;
		curSize.SetWidth(width);
	}
	if (defHeight != height)
	{
		bCompute = true;
		curSize.SetHeight(height);
	}
	if (bCompute)
	{
		wxPoint curPt(mouseState.GetX(), mouseState.GetY());
		int display = wxDisplay::GetFromPoint(curPt);
		if (wxNOT_FOUND == display)
			display = 0; // If the display can't be found, use the primary.
		wxDisplay monitor(display);
		wxRect rWorkSpace = monitor.GetClientArea();
		wxRect rect(curPt, curSize);
		// Make sure window is not bigger.
		if (rect.GetWidth() > rWorkSpace.GetWidth())
			rect.SetRight(rect.GetLeft() + rWorkSpace.GetWidth());
		if (rect.GetHeight() > rWorkSpace.GetHeight())
			rect.SetBottom(rect.GetTop() + rWorkSpace.GetHeight());
		// Make sure the window is fully visible in the screen.
		if (!rWorkSpace.Contains(rect.GetTopLeft()))
		{
			if (rect.GetLeft() < rWorkSpace.GetLeft())
				rect.Offset(rWorkSpace.GetLeft() - rect.GetLeft(), 0);
			if (rect.GetTop() < rWorkSpace.GetTop())
				rect.Offset(0, rWorkSpace.GetTop() - rect.GetTop());
		}
		// Only check the bottom-right if the rect has size.
		if (!rect.IsEmpty() && !rWorkSpace.Contains(rect.GetBottomRight()))
		{
			if (rect.GetRight() > rWorkSpace.GetRight())
				rect.Offset(rWorkSpace.GetRight() - rect.GetRight(), 0);
			if (rect.GetBottom() > rWorkSpace.GetBottom())
				rect.Offset(0, rWorkSpace.GetBottom() - rect.GetBottom());
		}
		if (wxDefaultCoord != x)
			x = rect.GetLeft();
		if (wxDefaultCoord != y)
			y = rect.GetTop();
		if (defWidth != width)
			width = rect.GetWidth();
		if (defHeight != height)
			height = rect.GetHeight();
	}
	frame->SetSize(x, y, width, height);

	// Should we open the last open file?
	if (filename.empty())
	{
		// Don't open it if the shift key is down.
		if (!::wxGetKeyState(WXK_SHIFT))
		{
			filename = wxConfig::Get()->Read(CFG_SETTINGS_LASTFILE, L"");
		}
	}
	// If a file is being opened, verify it exists first!
	// This catches both the case where the remembered last file (above)
	// doesn't exist and the case where the user types a command line where
	// it doesn't exist.
	if (!filename.empty())
	{
		if (!wxFile::Access(filename, wxFile::read))
		{
			filename.clear();
		}
	}
	bool bNew = true;
	if (!filename.empty())
	{
		if (m_manager->CreateDocument(filename, wxDOC_SILENT))
			bNew = false;
	}
	if (bNew)
		m_manager->CreateDocument(wxEmptyString, wxDOC_NEW);

	if (0 < state)
		frame->Maximize();
	frame->Show(true);

	std::wstring proxy = CAgilityBookOptions::GetProxy();
	if (!proxy.empty())
		wxURL::SetDefaultProxy(proxy.c_str());

	// Check for updates every 30 days.
	if (CAgilityBookOptions::GetAutoUpdateCheck())
	{
		std::wstring ver = StringUtil::stringW(wxConfig::Get()->Read(CFG_SETTINGS_LASTVERCHECK, L""));
		ARBDate date = ARBDate::FromString(ver, ARBDate::eISO);
		if (date.IsValid())
		{
			ARBDate today = ARBDate::Today();
			date += 30;
			if (date < today)
			{
				bool close = false;
				m_UpdateInfo.AutoUpdateProgram(NULL, *m_LangMgr, close);
				if (close)
				{
					// Must close so installation will work.
					return false;
				}
			}
		}
	}
	CUpdateInfo::CleanupUpdate();

	return true;
}


int CAgilityBookApp::OnExit()
{
	wxConfig::Get()->SetPath(L"Recent File List");
	m_manager->FileHistorySave(*wxConfig::Get());
	wxConfig::Get()->SetPath(L"..");
	delete m_LangMgr;
	m_LangMgr = NULL;
	delete m_manager;
	m_manager = NULL;
	delete m_printDialogData;
	m_printDialogData = NULL;
	delete m_Prn;
	m_Prn = NULL;
	CImageManager::Delete();
	Element::Terminate();
	return wxApp::OnExit();
}
