/*
 * Copyright (c) 2002-2009 David Connet. All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies, that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of David Connet, dcon Software,
 * AgilityBook, AgilityRecordBook or "Agility Record Book" not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. David Connet makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * DAVID CONNET DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL DAVID CONNET BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * http://opensource.org
 * Open Source Historical Permission Notice and Disclaimer.
 */

/**
 * @file
 *
 * @brief implementation of the CDlgRun class
 * @author David Connet
 *
 * Revision History
 * @li 2009-10-18 DRC Fix prepending of '0' to title points.
 * @li 2009-10-14 DRC Add dog's name to dialog caption.
 * @li 2009-10-14 DRC Fix initialization of opening pts in eScoreThenTime.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-12 DRC Fixed division/level initialization.
 * @li 2009-03-16 DRC Merged DlgRun* into here.
 * @li 2009-02-09 DRC Ported to wxWidgets.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2003-10-13 DRC Make ref run dlg default to perfect score.
 * DlgRunCRCD
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-05-04 DRC Added ability to suppress metafile.
 * @li 2004-04-27 DRC Added some error recovery.
 * DlgRunComments
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * DlgRunLink
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2004-06-02 DRC Moved ShellExecute code to AgilityBook.cpp, added icons.
 * @li 2004-03-30 DRC Created
 * DlgRunReference
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-09-20 DRC Added yourself was not getting up-to-date scoring info.
 * @li 2005-07-10 DRC Add button to add yourself to ref-runs.
 *                    Make default ref-run a 'Q'.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2004-09-28 DRC Accumulate all heights for refrun dlg.
 * @li 2003-12-27 DRC Changed FindEvent to take a date.
 * @li 2003-10-13 DRC Make ref run dlg default to perfect score.
 * DlgRunScore
 * @li 2008-02-01 DRC Make 'Notes' button change selection.
 * @li 2008-11-21 DRC Enable tallying runs that have only lifetime points.
 * @li 2007-12-03 DRC Refresh judge list after invoking 'notes' button.
 * @li 2007-07-01 DRC Fixed a problem with table flag on a run.
 * @li 2006-11-05 DRC Trim Divisions/Levels if no events are available on date.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-12-13 DRC Added direct access to Notes dialog.
 * @li 2005-12-04 DRC Added support for NADAC bonus titling points.
 * @li 2005-11-20 DRC Allow 'E's on non-titling runs.
 * @li 2005-08-11 DRC Removed QQ checkbox.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2005-01-02 DRC Added subnames to events.
 * @li 2005-01-01 DRC Renamed MachPts to SpeedPts.
 * @li 2004-11-13 DRC Also compute score for NA runs that have no titling pts.
 * @li 2004-09-07 DRC Time+Fault scoring shouldn't include time faults.
 * @li 2004-03-20 DRC The date never got set if the initial entry had no date
 *                    and we didn't change it (first run in a trial).
 *                    Plus, the table-in-yps flag was backwards and didn't
 *                    properly initialize when the event type changed.
 * @li 2004-02-14 DRC Added Table-in-YPS flag.
 * @li 2004-02-09 DRC Update YPS when the time is changed.
 *                    When date changes, update controls.
 * @li 2004-01-19 DRC Total faults weren't updated when course faults changed.
 * @li 2004-01-18 DRC Calling UpdateData during data entry causes way too much
 *                    validation. Only call during OnOK (from dlgsheet)
 * @li 2003-12-27 DRC Changed FindEvent to take a date. Also, update the
 *                    controls when the date changes as the scoring config may
 *                    change.
 * @li 2003-12-09 DRC Fixed a bug where the QQ checkbox didn't get set right.
 * @li 2003-10-13 DRC Made Time/CourseFaults common to all scoring methods.
 *                    This allows faults for things like language!
 * @li 2003-09-29 DRC Required pts were being overwriten with default values
 *                    during dialog initialization.
 * @li 2003-09-01 DRC Total faults weren't being shown when there was no SCT.
 * @li 2003-08-17 DRC Title points were being computed on 'NQ' and the score was
 *                    always being computed. Fixed both.
 * @li 2003-07-14 DRC Changed 'Score' to show data on 'Q' and 'NQ'.
 */

#include "stdafx.h"
#include "DlgRun.h"

#include "AgilityBook.h"
#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "ARBConfig.h"
#include "ARBConfigOtherPoints.h"
#include "ARBConfigVenue.h"
#include "ARBDogClub.h"
#include "ARBDogRun.h"
#include "ARBDogTrial.h"
#include "CheckLink.h"
#include "ClipBoard.h"
#include "ComboBoxes.h"
#include "DlgCRCDViewer.h"
#include "DlgInfoNote.h"
#include "DlgListCtrl.h"
#include "DlgReferenceRun.h"
#include "DlgSelectURL.h"
#include "Globals.h"
#include "ListCtrl.h"
#include "ListData.h"
#include "NoteButton.h"
#include "RichEditCtrl2.h"
#include "Validators.h"
#include <set>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/dcbuffer.h>

#include "res/CalEmpty.xpm"
#include "res/CalPlan.xpm"
#include "res/CalTentative.xpm"

#ifdef WIN32
// You can currently only enable this on Win32. I directly use the win32
// apis to play the metafile into a dc. In theory, I could use wxWidgets
// to play them - but I still need the direct win32 access to copy a
// metafile from the clipboard - wxWidgets only has support to copy >to<
// the clipboard.
#define HAS_ENHMETAFILE
#endif

/////////////////////////////////////////////////////////////////////////////

static struct
{
	int fmt;
	wxChar const* idText;
} const scRefRunColumns[] = {
	{wxLIST_FORMAT_CENTRE, _("IDS_COL_Q")},
	{wxLIST_FORMAT_CENTRE, _("IDS_COL_PLACE")},
	{wxLIST_FORMAT_CENTRE, _("IDS_COL_SCORE")},
	{wxLIST_FORMAT_LEFT, _("IDS_COL_TIME")},
	{wxLIST_FORMAT_LEFT, _("IDS_COL_NAME")},
	{wxLIST_FORMAT_LEFT, _("IDS_COL_HEIGHT")},
	{wxLIST_FORMAT_LEFT, _("IDS_COL_BREED")},
	{wxLIST_FORMAT_LEFT, _("IDS_COL_NOTE")},
};
static int const scNumRefRunColumns = sizeof(scRefRunColumns) / sizeof(scRefRunColumns[0]);

static struct
{
	CDlgRun* pThis;
	CColumnOrder* pCols;
} s_SortInfo;

/////////////////////////////////////////////////////////////////////////////

class CDlgDogDivData : public wxClientData
{
public:
	CDlgDogDivData(ARBConfigDivisionPtr div)
		: m_Div(div)
	{
	}
	ARBConfigDivisionPtr m_Div;
};


class CDlgDogLevelData : public wxClientData
{
public:
	CDlgDogLevelData(ARBConfigLevelPtr pLevel)
		: m_pLevel(pLevel)
		, m_pSubLevel()
	{
	}
	CDlgDogLevelData(
			ARBConfigLevelPtr pLevel,
			ARBConfigSubLevelPtr pSubLevel)
		: m_pLevel(pLevel)
		, m_pSubLevel(pSubLevel)
	{
	}
	ARBConfigLevelPtr m_pLevel;
	ARBConfigSubLevelPtr m_pSubLevel;
};

/////////////////////////////////////////////////////////////////////////////

class CDlgDogRefRunData : public CListData
{
public:
	CDlgDogRefRunData(ARBDogReferenceRunPtr refRun)
		: m_RefRun(refRun)
	{
	}
	virtual wxString OnNeedText(long iCol) const;
	ARBDogReferenceRunPtr GetData() const		{return m_RefRun;}
	void SetData(ARBDogReferenceRunPtr data)	{m_RefRun = data;}
private:
	ARBDogReferenceRunPtr m_RefRun;
};


wxString CDlgDogRefRunData::OnNeedText(long iCol) const
{
	wxString str;
	switch (iCol)
	{
	default:
		break;
	case 0: // Q
		str = m_RefRun->GetQ().str();
		break;
	case 1: // Place
		str << m_RefRun->GetPlace();
		break;
	case 2: // Score
		str = m_RefRun->GetScore();
		break;
	case 3: // Time
		str = ARBDouble::str(m_RefRun->GetTime());
		break;
	case 4: // Name
		str = m_RefRun->GetName();
		break;
	case 5: // Height
		str = m_RefRun->GetHeight();
		break;
	case 6: // Breed
		str = m_RefRun->GetBreed();
		break;
	case 7: // Note
		str = m_RefRun->GetNote();
		str.Replace(wxT("\n"), wxT(" "));
		break;
	}
	return str;
}


#if wxCHECK_VERSION(2,9,1)
int wxCALLBACK CompareRefRuns(long item1, long item2, wxIntPtr sortData)
#else
int wxCALLBACK CompareRefRuns(long item1, long item2, long sortData)
#endif
{
	CDlgDogRefRunDataPtr pData1 = s_SortInfo.pThis->GetReferenceDataByData(item1);
	CDlgDogRefRunDataPtr pData2 = s_SortInfo.pThis->GetReferenceDataByData(item2);
	ARBDogReferenceRunPtr pTitle1 = pData1->GetData();
	ARBDogReferenceRunPtr pTitle2 = pData2->GetData();
	int rc = 0;
	for (int i = 0; i < s_SortInfo.pCols->GetSize(); ++i)
	{
		int col = s_SortInfo.pCols->GetColumnAt(i);
		switch (col)
		{
		default:
		case 3: // Time
			if (pTitle1->GetTime() < pTitle2->GetTime())
				rc = -1;
			else if (pTitle1->GetTime() > pTitle2->GetTime())
				rc = 1;
			break;
		case 0: // Q
			if (pTitle1->GetQ() < pTitle2->GetQ())
				rc = -1;
			else if (pTitle1->GetQ() > pTitle2->GetQ())
				rc = 1;
			break;
		case 1: // Place
			if (pTitle1->GetPlace() < pTitle2->GetPlace())
				rc = -1;
			else if (pTitle1->GetPlace() > pTitle2->GetPlace())
				rc = 1;
			break;
		case 2: // Score
			if (pTitle1->GetScore() < pTitle2->GetScore())
				rc = -1;
			else if (pTitle1->GetScore() > pTitle2->GetScore())
				rc = 1;
			break;
		case 4: // Name
			if (pTitle1->GetName() < pTitle2->GetName())
				rc = -1;
			else if (pTitle1->GetName() > pTitle2->GetName())
				rc = 1;
			break;
		case 5: // Height
			if (pTitle1->GetHeight() < pTitle2->GetHeight())
				rc = -1;
			else if (pTitle1->GetHeight() > pTitle2->GetHeight())
				rc = 1;
			break;
		case 6: // Breed
			if (pTitle1->GetBreed() < pTitle2->GetBreed())
				rc = -1;
			else if (pTitle1->GetBreed() > pTitle2->GetBreed())
				rc = 1;
			break;
		case 7: // Notes
			if (pTitle1->GetNote() < pTitle2->GetNote())
				rc = -1;
			else if (pTitle1->GetNote() > pTitle2->GetNote())
				rc = 1;
			break;
		}
		if (rc)
		{
			if (s_SortInfo.pCols->IsDescending(col))
				rc *= -1;
			break;
		}
	}
	return rc;
}

/////////////////////////////////////////////////////////////////////////////

class CMetaDataDisplay : public CTextCtrl
{
public:
	CMetaDataDisplay(
			wxWindow* parent,
			ARBDogRunPtr pRun);
	~CMetaDataDisplay();

	bool HasMetafileSupport() const
	{
#ifdef HAS_ENHMETAFILE
		return true;
#else
		return false;
#endif
	}
	bool HasMetafile()
	{
#ifdef HAS_ENHMETAFILE
		return NULL != m_metaFile;
#else
		return false;
#endif
	}
	bool ViewText() const		{return m_ViewText;}
	bool HasCourse() const		{return !m_Insert;}
	void SetCRCDData();
	void ToggleView();
	void OnCopy();
	void OnCRCDImage(bool checked);
	void Display();

private:
	void DeleteMetaFile();

	ARBDogRunPtr m_Run;
#ifdef HAS_ENHMETAFILE
	HENHMETAFILE m_metaFile;
	HENHMETAFILE m_metaFileBack;
#endif
	bool m_ViewText;
	bool m_Insert;

	DECLARE_EVENT_TABLE()
	void OnPaint(wxPaintEvent& evt);
};


BEGIN_EVENT_TABLE(CMetaDataDisplay, CTextCtrl)
	EVT_PAINT(CMetaDataDisplay::OnPaint)
END_EVENT_TABLE()


CMetaDataDisplay::CMetaDataDisplay(
		wxWindow* parent,
		ARBDogRunPtr pRun)
	: m_Run(pRun)
#ifdef HAS_ENHMETAFILE
	, m_metaFile(NULL)
	, m_metaFileBack(NULL)
#endif
	, m_ViewText(true)
	, m_Insert(pRun->GetCRCD().empty())
{
	CTextCtrl::Create(parent, wxID_ANY,
		m_Run->GetCRCD(),
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	AllowMultilineTabstop(true);

#ifdef HAS_ENHMETAFILE
	if (0 < m_Run->GetCRCDRawMetaData().length())
	{
		ARBMetaDataPtr metaData = m_Run->GetCRCDMetaData();
		m_metaFile = SetEnhMetaFileBits(static_cast<UINT>(metaData->length()), reinterpret_cast<BYTE const*>(metaData->data()));
		m_metaFileBack = m_metaFile;
		if (m_metaFile)
		{
			m_ViewText = false;
			Enable(m_ViewText);
		}
	}
#endif
}


CMetaDataDisplay::~CMetaDataDisplay()
{
	DeleteMetaFile();
}


void CMetaDataDisplay::DeleteMetaFile()
{
#ifdef HAS_ENHMETAFILE
	if (!m_metaFile && m_metaFileBack)
		m_metaFile = m_metaFileBack;
	if (m_metaFile)
	{
		DeleteEnhMetaFile(m_metaFile);
		m_metaFile = NULL;
		m_metaFileBack = NULL;
	}
#endif
}


void CMetaDataDisplay::SetCRCDData()
{
#ifdef HAS_ENHMETAFILE
	if (m_metaFile)
	{
		ENHMETAHEADER header;
		GetEnhMetaFileHeader(m_metaFile, sizeof(header), &header);
		UINT nSize = GetEnhMetaFileBits(m_metaFile, 0, NULL);
		LPBYTE bits = new BYTE[nSize+1];
		GetEnhMetaFileBits(m_metaFile, nSize, bits);
		assert(sizeof(BYTE) == sizeof(char));
		m_Run->SetCRCDMetaData(bits, nSize);
		delete [] bits;
	}
	else
#endif
		m_Run->SetCRCDMetaData(NULL, 0);
}


void CMetaDataDisplay::ToggleView()
{
	m_ViewText = !m_ViewText;
	Enable(m_ViewText);
	Refresh();
}


void CMetaDataDisplay::OnCopy()
{
	if (m_Insert)
	{
		bool bText = false;
		if (wxTheClipboard->Open())
		{
			bText = wxTheClipboard->IsSupported(wxDF_TEXT);
			wxTheClipboard->Close();
		}
		bool bMeta = false;
#ifdef HAS_ENHMETAFILE
		bMeta = ::IsClipboardFormatAvailable(CF_ENHMETAFILE) ? true : false;
		if (!CAgilityBookOptions::GetIncludeCRCDImage())
			bMeta = FALSE;
#endif
		if (bText || bMeta)
		{
			CClipboardDataReader clpData;
			if (clpData.Open())
			{
				SetLabel(wxT(""));
				DeleteMetaFile();
				if (bText)
				{
					m_ViewText = true;
					wxString str;
					clpData.GetData(str);
					str.Trim(true);
					str.Trim(false);
					// We do the replace since CRCD3 has "\n\nhdrs\r\netc"
					// Standardize to \n.
					str.Replace(wxT("\r\n"), wxT("\n"));
					m_Run->SetCRCD(str);
					SetLabel(str);
					if (0 < str.length())
						m_Insert = false;
					// Only create the metafile if we pasted text. Otherwise
					// we could end up with a non-CRCD metafile. It's still
					// possible this may not be CRCD data - the user can
					// just clear it then.
#ifdef HAS_ENHMETAFILE
					if (bMeta)
					{
						HENHMETAFILE hData = reinterpret_cast<HENHMETAFILE>(GetClipboardData(CF_ENHMETAFILE));
						m_metaFile = CopyEnhMetaFile(hData, NULL);
						m_metaFileBack = m_metaFile;
						SetCRCDData();
						if (m_metaFile)
						{
							m_ViewText = false;
							m_Insert = false;
						}
					}
#endif
				}
			}
		}
	}
	else
	{
		m_Insert = true;
		m_ViewText = true;
		SetLabel(wxT(""));
		DeleteMetaFile();
		m_Run->SetCRCD(wxT(""));
		m_Run->SetCRCDMetaData(NULL, 0);
	}
	Enable(m_ViewText);
	Refresh();
}


void CMetaDataDisplay::OnCRCDImage(bool checked)
{
#ifdef HAS_ENHMETAFILE
	CAgilityBookOptions::SetIncludeCRCDImage(checked);
	if (checked)
	{
		if (!m_metaFile && m_metaFileBack)
			m_metaFile = m_metaFileBack;
	}
	else
	{
		if (m_metaFile)
		{
			m_metaFileBack = m_metaFile;
			m_metaFile = NULL;
		}
	}
	SetCRCDData();
	if (m_metaFile)
	{
		m_ViewText = false;
		m_Insert = false;
	}
	else
		m_ViewText = true;
	Enable(m_ViewText);
	Refresh();
#endif
}


void CMetaDataDisplay::Display()
{
#ifdef HAS_ENHMETAFILE
	if (m_metaFile)
	{
		CDlgCRCDViewer viewer(m_metaFile, this);
		viewer.ShowModal();
	}
#endif
}


void CMetaDataDisplay::OnPaint(wxPaintEvent& evt)
{
	if (m_ViewText)
		evt.Skip(true);
	else
	{
		wxBufferedPaintDC dc(this);
		dc.SetBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
		dc.Clear();
#ifdef HAS_ENHMETAFILE
		wxSize sz = GetClientSize();
		RECT r;
		r.left = 0;
		r.top = 0;
		r.right = std::min(sz.x, sz.y);
		r.bottom = std::min(sz.x, sz.y);
		HDC hdc = (HDC)dc.GetHDC();
		PlayEnhMetaFile(hdc, m_metaFile, &r);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

// This is just to get the text in a sunken static control to look better
static wxString Pad(wxString const& val)
{
	wxString padded(wxT(" "));
	padded += val;
	padded += wxT(" ");
	return padded;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CDlgRun, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgRun::OnOk)
END_EVENT_TABLE()


CDlgRun::CDlgRun(
		CAgilityBookDoc* pDoc,
		ARBDogTrialPtr pTrial,
		ARBDogRunPtr pRun,
		wxWindow* pParent,
		int iSelectPage)
	: wxDialog()
	, m_pDoc(pDoc)
	, m_pTrial(pTrial)
	, m_pRealRun(pRun)
	, m_Run(pRun->Clone())
	, m_pRefRunMe()
	, m_Club()
	, m_pVenue()
	, m_panelScore(NULL)
	, m_Date(pRun->GetDate())
	, m_ctrlDivisions(NULL)
	, m_ctrlLevels(NULL)
	, m_ctrlEvents(NULL)
	, m_ctrlSubNamesText(NULL)
	, m_ctrlSubNames(NULL)
	, m_SubName(pRun->GetSubName())
	, m_ctrlTable(NULL)
	, m_Table(false)
	, m_ctrlHeight(NULL)
	, m_Height(pRun->GetHeight())
	, m_ctrlJudge(NULL)
	, m_Judge(pRun->GetJudge())
	, m_ctrlHandler(NULL)
	, m_Handler(pRun->GetHandler())
	, m_ctrlConditions(NULL)
	, m_Conditions(pRun->GetConditions())
	, m_ctrlDesc(NULL)
	, m_ctrlPartnerEdit(NULL)
	, m_ctrlPartner(NULL)
	, m_ctrlSCTText(NULL)
	, m_ctrlSCT(NULL)
	, m_SCT(0.0)
	, m_textYardsReqOpeningPts(NULL)
	, m_ctrlYardsReqOpeningPts(NULL)
	, m_Opening(0)
	, m_Yards(0.0)
	, m_textMinYPSClosingTime(NULL)
	, m_ctrlMinYPSClosingTime(NULL)
	, m_SCT2(0.0)
	, m_ctrlClosingText(NULL)
	, m_ctrlClosing(NULL)
	, m_Closing(0)
	, m_ctrlObstacles(NULL)
	, m_Obstacles(pRun->GetScoring().GetObstacles())
	, m_ctrlTimeText(NULL)
	, m_ctrlTime(NULL)
	, m_Time(0.0)
	, m_ctrlFaultsText(NULL)
	, m_ctrlFaults(NULL)
	, m_Faults(0)
	, m_textYPSOpeningPts(NULL)
	, m_ctrlYPSOpeningPts(NULL)
	, m_Open(0)
	, m_textClosingPtsTotalFaults(NULL)
	, m_ctrlClosingPtsTotalFaults(NULL)
	, m_Close(0)
	, m_ctrlObstaclesPS(NULL)
	, m_ctrlPlace(NULL)
	, m_Place(pRun->GetPlace())
	, m_ctrlInClass(NULL)
	, m_InClass(pRun->GetInClass())
	, m_ctrlDogsQd(NULL)
	, m_DogsQd(pRun->GetDogsQd())
	, m_ctrlQ(NULL)
	, m_ctrlBonusPtsText(NULL)
	, m_ctrlBonusPts(NULL)
	, m_BonusPts(pRun->GetScoring().GetBonusPts())
	, m_ctrlSpeedPtsText(NULL)
	, m_ctrlSpeedPts(NULL)
	, m_ctrlTitlePointsText(NULL)
	, m_ctrlTitlePoints(NULL)
	, m_ctrlScore(NULL)
	, m_Comments(pRun->GetNote())
	, m_sortRefRuns(wxT("RefRuns"))
	, m_idxRefRunPage(-1)
	, m_ctrlFaultsList(NULL)
	, m_ctrlRefRuns(NULL)
	, m_ctrlRefAddMe(NULL)
	, m_ctrlRefEdit(NULL)
	, m_ctrlRefDelete(NULL)
	, m_CRCDDisplay(NULL)
	, m_ctrlCourse(NULL)
	, m_ctrlCRCDView(NULL)
	, m_ctrlCRCDCopy(NULL)
	, m_ctrlIncImage(NULL)
	, m_ctrlLinks(NULL)
	, m_ImageList(16,16)
	, m_imgEmpty(-1)
	, m_imgOk(-1)
	, m_imgMissing(-1)
	, m_ctrlLinkEdit(NULL)
	, m_ctrlLinkDelete(NULL)
	, m_ctrlLinkOpen(NULL)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);
	if (!pParent)
		pParent = wxGetApp().GetTopWindow();
	Create(pParent, wxID_ANY, pDoc->AddDogToCaption(_("IDS_RUN_PROPERTIES")),
			wxDefaultPosition, wxDefaultSize,
			wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	m_clrBack = GetBackgroundColour();

	pTrial->GetClubs().GetPrimaryClub(&m_Club);
	assert(NULL != m_Club.get());
	pDoc->Book().GetConfig().GetVenues().FindVenue(m_Club->GetVenue(), &m_pVenue);
	assert(NULL != m_pVenue.get());

	m_sortRefRuns.Initialize(scNumRefRunColumns);

	if (!m_Run->GetDate().IsValid())
		m_Date.SetToday();
	if (m_Height.empty())
	{
		wxString last = CAgilityBookOptions::GetLastEnteredHeight();
		if (!last.empty())
			m_Height = last;
	}
	if (m_Judge.empty())
	{
		wxString last = CAgilityBookOptions::GetLastEnteredJudge();
		if (!last.empty())
			m_Judge = last;
	}
	if (m_Handler.empty())
	{
		wxString last = CAgilityBookOptions::GetLastEnteredHandler();
		if (!last.empty())
			m_Handler = last;
	}

	switch (m_Run->GetScoring().GetType())
	{
	default:
		break;
	case ARBDogRunScoring::eTypeByTime:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Table = m_Run->GetScoring().HasTable();
		m_Yards = m_Run->GetScoring().GetYards();
		m_SCT = m_Run->GetScoring().GetSCT();
		break;
	case ARBDogRunScoring::eTypeByOpenClose:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Opening = m_Run->GetScoring().GetNeedOpenPts();
		m_Closing = m_Run->GetScoring().GetNeedClosePts();
		m_Open = m_Run->GetScoring().GetOpenPts();
		m_Close = m_Run->GetScoring().GetClosePts();
		m_SCT = m_Run->GetScoring().GetSCT();
		m_SCT2 = m_Run->GetScoring().GetSCT2();
		break;
	case ARBDogRunScoring::eTypeByPoints:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Opening = m_Run->GetScoring().GetNeedOpenPts();
		m_Open = m_Run->GetScoring().GetOpenPts();
		m_SCT = m_Run->GetScoring().GetSCT();
		break;
	}

	// Controls (these are done first to control tab order)

	wxNotebook* notebook = new wxNotebook(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, 0);
	notebook->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(CDlgRun::OnPageChanged), NULL, this);

	// Score

	m_panelScore = new wxPanel(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxStaticText* textDate = new wxStaticText(m_panelScore, wxID_ANY,
		wxT("Date"), wxDefaultPosition, wxDefaultSize, 0);
	textDate->Wrap(-1);

	wxDatePickerCtrl* ctrlDate = new wxDatePickerCtrl(m_panelScore, wxID_ANY, wxDefaultDateTime,
		wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN|wxDP_SHOWCENTURY,
		CGenericValidator(&m_Date));
	ctrlDate->Connect(wxEVT_DATE_CHANGED, wxDateEventHandler(CDlgRun::OnScoreDateChanged), NULL, this);
	ctrlDate->SetHelpText(_("HIDC_RUNSCORE_DATE"));
	ctrlDate->SetToolTip(_("HIDC_RUNSCORE_DATE"));

	wxStaticText* textVenue = new wxStaticText(m_panelScore, wxID_ANY,
		Pad(m_pVenue->GetName()),
		wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	textVenue->Wrap(-1);

	wxStaticText* textClub = new wxStaticText(m_panelScore, wxID_ANY,
		Pad(m_Club->GetName()),
		wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	textClub->Wrap(-1);

	wxStaticText* textLocation = new wxStaticText(m_panelScore, wxID_ANY,
		Pad(pTrial->GetLocation()),
		wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
	textLocation->Wrap(-1);

	wxStaticText* textDiv = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_DIVISION"),
		wxDefaultPosition, wxDefaultSize, 0);
	textDiv->Wrap(-1);

	m_ctrlDivisions = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_ctrlDivisions->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CDlgRun::OnSelchangeDivision), NULL, this);
	m_ctrlDivisions->SetHelpText(_("HIDC_RUNSCORE_DIVISION"));
	m_ctrlDivisions->SetToolTip(_("HIDC_RUNSCORE_DIVISION"));

	wxStaticText* textLevel = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_LEVEL"),
		wxDefaultPosition, wxDefaultSize, 0);
	textLevel->Wrap(-1);

	m_ctrlLevels = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_ctrlLevels->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CDlgRun::OnSelchangeLevel), NULL, this);
	m_ctrlLevels->SetHelpText(_("HIDC_RUNSCORE_LEVEL"));
	m_ctrlLevels->SetToolTip(_("HIDC_RUNSCORE_LEVEL"));

	wxStaticText* textEvent = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_EVENT"),
		wxDefaultPosition, wxDefaultSize, 0);
	textEvent->Wrap(-1);

	m_ctrlEvents = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN|wxCB_READONLY);
	m_ctrlEvents->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(CDlgRun::OnSelchangeEvent), NULL, this);
	m_ctrlEvents->SetHelpText(_("HIDC_RUNSCORE_EVENT"));
	m_ctrlEvents->SetToolTip(_("HIDC_RUNSCORE_EVENT"));

	m_ctrlSubNamesText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_SUBNAME"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlSubNamesText->Wrap(-1);

	m_ctrlSubNames = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN,
		CTrimValidator(&m_SubName, TRIMVALIDATOR_TRIM_BOTH));
	m_ctrlSubNames->SetHelpText(_("HIDC_RUNSCORE_SUBNAME"));
	m_ctrlSubNames->SetToolTip(_("HIDC_RUNSCORE_SUBNAME"));

	m_ctrlTable = new wxCheckBox(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_TABLE"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_Table));
	m_ctrlTable->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CDlgRun::OnBnClickedTableYps), NULL, this);
	m_ctrlTable->SetHelpText(_("HIDC_RUNSCORE_TABLE"));
	m_ctrlTable->SetToolTip(_("HIDC_RUNSCORE_TABLE"));

	wxStaticText* textHeight = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_HEIGHT"),
		wxDefaultPosition, wxDefaultSize, 0);
	textHeight->Wrap(-1);

	m_ctrlHeight = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition,
#ifdef __WXMAC__
		wxSize(100, -1), // Just not wide enough on a Mac...
#else
		wxSize(50, -1),
#endif
		0, NULL, wxCB_DROPDOWN,
		CTrimValidator(&m_Height, TRIMVALIDATOR_TRIM_BOTH));
	m_ctrlHeight->SetHelpText(_("HIDC_RUNSCORE_HEIGHT"));
	m_ctrlHeight->SetToolTip(_("HIDC_RUNSCORE_HEIGHT"));
	std::set<wxString> names;
	m_pDoc->Book().GetAllHeights(names);
	std::set<wxString>::const_iterator iter;
	for (iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlHeight->Append((*iter));
	}

	wxStaticText* textJudge = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_JUDGE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textJudge->Wrap(-1);

	m_ctrlJudge = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN,
		CTrimValidator(&m_Judge, _("IDS_SELECT_JUDGE")));
	m_ctrlJudge->SetHelpText(_("HIDC_RUNSCORE_JUDGE"));
	m_ctrlJudge->SetToolTip(_("HIDC_RUNSCORE_JUDGE"));

	CNoteButton* ctrlJudgeNote = new CNoteButton(m_panelScore);
	ctrlJudgeNote->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnJudgeNotes), NULL, this);
	ctrlJudgeNote->SetHelpText(_("HIDC_RUNSCORE_JUDGE_NOTES"));
	ctrlJudgeNote->SetToolTip(_("HIDC_RUNSCORE_JUDGE_NOTES"));

	wxStaticText* textHandler = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_HANDLER"),
		wxDefaultPosition, wxDefaultSize, 0);
	textHandler->Wrap(-1);

	m_ctrlHandler = new wxComboBox(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxDefaultSize,
		0, NULL, wxCB_DROPDOWN,
		CTrimValidator(&m_Handler, TRIMVALIDATOR_TRIM_BOTH));
	m_ctrlHandler->SetHelpText(_("HIDC_RUNSCORE_HANDLER"));
	m_ctrlHandler->SetToolTip(_("HIDC_RUNSCORE_HANDLER"));
	m_pDoc->Book().GetAllHandlers(names);
	for (iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlHandler->Append((*iter));
	}

	wxStaticText* textConditions = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_CONDITIONS"),
		wxDefaultPosition, wxDefaultSize, 0);
	textConditions->Wrap(-1);

	m_ctrlConditions = new CTextCtrl(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxSize(-1, 50),
		wxTE_MULTILINE|wxTE_WORDWRAP,
		CTrimValidator(&m_Conditions, TRIMVALIDATOR_TRIM_BOTH));
	m_ctrlConditions->SetHelpText(_("HIDC_RUNSCORE_CONDITIONS"));
	m_ctrlConditions->SetToolTip(_("HIDC_RUNSCORE_CONDITIONS"));

	m_ctrlDesc = new CRichEditCtrl2(m_panelScore, wxID_ANY,
		wxEmptyString, wxDefaultPosition, wxSize(-1, 70), true);
	m_ctrlDesc->SetHelpText(_("HIDC_RUNSCORE_DESC"));
	m_ctrlDesc->SetToolTip(_("HIDC_RUNSCORE_DESC"));

	m_ctrlPartnerEdit = new wxButton(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_PARTNERS_EDIT"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlPartnerEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnPartnersEdit), NULL, this);
	m_ctrlPartnerEdit->SetHelpText(_("HIDC_RUNSCORE_PARTNERS_EDIT"));
	m_ctrlPartnerEdit->SetToolTip(_("HIDC_RUNSCORE_PARTNERS_EDIT"));

	m_ctrlPartner = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
	m_ctrlPartner->SetBackgroundColour(m_clrBack);
	m_ctrlPartner->SetHelpText(_("HIDC_RUNSCORE_PARTNER"));
	m_ctrlPartner->SetToolTip(_("HIDC_RUNSCORE_PARTNER"));

	m_ctrlSCTText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_SCT"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlSCTText->Wrap(-1);

	m_ctrlSCT = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_SCT));
	m_ctrlSCT->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnSCTChange), NULL, this);
	m_ctrlSCT->SetHelpText(_("HIDC_RUNSCORE_SCT"));
	m_ctrlSCT->SetToolTip(_("HIDC_RUNSCORE_SCT"));

	m_textYardsReqOpeningPts = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_OPENING_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_textYardsReqOpeningPts->Wrap(-1);

	m_ctrlYardsReqOpeningPts = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0);
	m_ctrlYardsReqOpeningPts->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnReqOpeningYPSChange), NULL, this);
	m_ctrlYardsReqOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPENING_PTS"));
	m_ctrlYardsReqOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPENING_PTS"));

	m_textMinYPSClosingTime = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_MIN_YPS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_textMinYPSClosingTime->Wrap(-1);

	m_ctrlMinYPSClosingTime = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0);
	m_ctrlMinYPSClosingTime->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnClosingTimeChange), NULL, this);
	m_ctrlMinYPSClosingTime->SetHelpText(_("HIDC_RUNSCORE_SCT2"));
	m_ctrlMinYPSClosingTime->SetToolTip(_("HIDC_RUNSCORE_SCT2"));

	m_ctrlClosingText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_CLOSING_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlClosingText->Wrap(-1);

	m_ctrlClosing = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_Closing));
	m_ctrlClosing->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnReqClosingChange), NULL, this);
	m_ctrlClosing->SetHelpText(_("HIDC_RUNSCORE_CLOSING_PTS"));
	m_ctrlClosing->SetToolTip(_("HIDC_RUNSCORE_CLOSING_PTS"));

	wxStaticText* ctrlObstaclesText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_OBSTACLES"),
		wxDefaultPosition, wxDefaultSize, 0);
	ctrlObstaclesText->Wrap(-1);

	m_ctrlObstacles = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_Obstacles));
	m_ctrlObstacles->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnNumObsChange), NULL, this);
	m_ctrlObstacles->SetHelpText(_("HIDC_RUNSCORE_OBSTACLES"));
	m_ctrlObstacles->SetToolTip(_("HIDC_RUNSCORE_OBSTACLES"));

	m_ctrlTimeText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_TIME"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlTimeText->Wrap(-1);

	m_ctrlTime = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_Time));
	m_ctrlTime->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnTimeChange), NULL, this);
	m_ctrlTime->SetHelpText(_("HIDC_RUNSCORE_TIME"));
	m_ctrlTime->SetToolTip(_("HIDC_RUNSCORE_TIME"));

	m_ctrlFaultsText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_FAULTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlFaultsText->Wrap(-1);

	m_ctrlFaults = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_Faults));
	m_ctrlFaults->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnFaultsChange), NULL, this);
	m_ctrlFaults->SetHelpText(_("HIDC_RUNSCORE_FAULTS"));
	m_ctrlFaults->SetToolTip(_("HIDC_RUNSCORE_FAULTS"));

	m_textYPSOpeningPts = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_OPEN_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_textYPSOpeningPts->Wrap(-1);

	m_ctrlYPSOpeningPts = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0);
	m_ctrlYPSOpeningPts->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnOpenChange), NULL, this);
	m_ctrlYPSOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPEN_PTS"));
	m_ctrlYPSOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPEN_PTS"));

	m_textClosingPtsTotalFaults = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_CLOSE_PTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_textClosingPtsTotalFaults->Wrap(-1);

	m_ctrlClosingPtsTotalFaults = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0);
	m_ctrlClosingPtsTotalFaults->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnCloseChange), NULL, this);
	m_ctrlClosingPtsTotalFaults->SetHelpText(_("HIDC_RUNSCORE_CLOSE_PTS"));
	m_ctrlClosingPtsTotalFaults->SetToolTip(_("HIDC_RUNSCORE_CLOSE_PTS"));

	wxStaticText* ctrlObstaclesPSText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_OBSTACLES_PER_SEC"),
		wxDefaultPosition, wxDefaultSize, 0);
	ctrlObstaclesPSText->Wrap(-1);

	m_ctrlObstaclesPS = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), wxTE_READONLY);
	m_ctrlObstaclesPS->SetBackgroundColour(m_clrBack);
	m_ctrlObstaclesPS->SetHelpText(_("HIDC_RUNSCORE_OBSTACLES_PER_SEC"));
	m_ctrlObstaclesPS->SetToolTip(_("HIDC_RUNSCORE_OBSTACLES_PER_SEC"));

	wxStaticText* textPlace = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_PLACE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textPlace->Wrap(-1);

	m_ctrlPlace = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_Place));
	m_ctrlPlace->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnPlaceChange), NULL, this);
	m_ctrlPlace->SetHelpText(_("HIDC_RUNSCORE_PLACE"));
	m_ctrlPlace->SetToolTip(_("HIDC_RUNSCORE_PLACE"));

	wxStaticText* textPlaceOf = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_IN_CLASS"),
		wxDefaultPosition, wxDefaultSize, 0);
	textPlaceOf->Wrap(-1);

	m_ctrlInClass = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_InClass));
	m_ctrlInClass->SetHelpText(_("HIDC_RUNSCORE_IN_CLASS"));
	m_ctrlInClass->SetToolTip(_("HIDC_RUNSCORE_IN_CLASS"));

	wxStaticText* textDogsQd = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_DOGS_QD"),
		wxDefaultPosition, wxDefaultSize, 0);
	textDogsQd->Wrap(-1);

	m_ctrlDogsQd = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(30, -1), 0,
		CGenericValidator(&m_DogsQd));
	m_ctrlDogsQd->SetHelpText(_("HIDC_RUNSCORE_DOGS_QD"));
	m_ctrlDogsQd->SetToolTip(_("HIDC_RUNSCORE_DOGS_QD"));

	wxStaticText* textQ = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_Q"),
		wxDefaultPosition, wxDefaultSize, 0);
	textQ->Wrap(-1);

	m_ctrlQ = new CQualifyingComboBox(m_panelScore, m_Run);
	m_ctrlQ->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CDlgRun::OnSelchangeQ), NULL, this);
	m_ctrlQ->SetHelpText(_("HIDC_RUNSCORE_Q"));
	m_ctrlQ->SetToolTip(_("HIDC_RUNSCORE_Q"));

	m_ctrlBonusPtsText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_BONUSPTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlBonusPtsText->Wrap(-1);

	m_ctrlBonusPts = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), 0,
		CGenericValidator(&m_BonusPts));
	m_ctrlBonusPts->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CDlgRun::OnBonusChange), NULL, this);
	m_ctrlBonusPts->SetHelpText(_("HIDC_RUNSCORE_BONUSPTS"));
	m_ctrlBonusPts->SetToolTip(_("HIDC_RUNSCORE_BONUSPTS"));

	m_ctrlSpeedPtsText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_SPEEDPTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlSpeedPtsText->Wrap(-1);

	m_ctrlSpeedPts = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), wxTE_READONLY);
	m_ctrlSpeedPts->SetBackgroundColour(m_clrBack);

	m_ctrlTitlePointsText = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_TITLE_POINTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlTitlePointsText->Wrap(-1);

	m_ctrlTitlePoints = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), wxTE_READONLY);
	m_ctrlTitlePoints->SetBackgroundColour(m_clrBack);

	wxStaticText* textScore = new wxStaticText(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_SCORE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textScore->Wrap(-1);

	m_ctrlScore = new CTextCtrl(m_panelScore, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(50, -1), wxTE_READONLY);
	m_ctrlScore->SetBackgroundColour(m_clrBack);

	wxButton* btnOtherPoints = new wxButton(m_panelScore, wxID_ANY,
		_("IDC_RUNSCORE_OTHERPOINTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	btnOtherPoints->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnOtherpoints), NULL, this);
	btnOtherPoints->SetHelpText(_("HIDC_RUNSCORE_OTHERPOINTS"));
	btnOtherPoints->SetToolTip(_("HIDC_RUNSCORE_OTHERPOINTS"));
	if (0 < m_pDoc->Book().GetConfig().GetOtherPoints().size())
		btnOtherPoints->Enable(true);
	else
		btnOtherPoints->Enable(false);

	// Comments

	wxPanel* panelComments = new wxPanel(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	CTextCtrl* ctrlComments = new CTextCtrl(panelComments, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP,
		CTrimValidator(&m_Comments, TRIMVALIDATOR_TRIM_BOTH));
	ctrlComments->SetHelpText(_("HIDC_RUNCOMMENT_COMMENTS"));
	ctrlComments->SetToolTip(_("HIDC_RUNCOMMENT_COMMENTS"));

	m_ctrlFaultsList = new wxListBox(panelComments, wxID_ANY,
		wxDefaultPosition, wxDefaultSize,
		0, NULL, 0);
	m_ctrlFaultsList->SetHelpText(_("HIDC_RUNCOMMENT_FAULTS_LIST"));
	m_ctrlFaultsList->SetToolTip(_("HIDC_RUNCOMMENT_FAULTS_LIST"));

	wxButton* btnFaults = new wxButton(panelComments, wxID_ANY,
		_("IDC_RUNCOMMENT_COMMENTS_FAULTS"),
		wxDefaultPosition, wxDefaultSize, 0);
	btnFaults->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnCommentsFaults), NULL, this);
	btnFaults->SetHelpText(_("HIDC_RUNCOMMENT_COMMENTS_FAULTS"));
	btnFaults->SetToolTip(_("HIDC_RUNCOMMENT_COMMENTS_FAULTS"));

	// RefRuns

	wxPanel* panelRefRuns = new wxPanel(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	m_ctrlRefRuns = new CReportListCtrl(panelRefRuns,
		true, CReportListCtrl::eSortHeader, true);
	m_ctrlRefRuns->Connect(wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler(CDlgRun::OnRefRunColumnClick), NULL, this);
	m_ctrlRefRuns->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CDlgRun::OnRefRunItemSelected), NULL, this);
	m_ctrlRefRuns->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(CDlgRun::OnRefRunItemActivated), NULL, this);
	m_ctrlRefRuns->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CDlgRun::OnRefRunKeyDown), NULL, this);
	m_ctrlRefRuns->SetHelpText(_("HIDC_RUNREF_REF_RUNS"));
	m_ctrlRefRuns->SetToolTip(_("HIDC_RUNREF_REF_RUNS"));
	int index;
	for (index = 0; index < scNumRefRunColumns; ++index)
	{
		m_ctrlRefRuns->InsertColumn(index, wxGetTranslation(scRefRunColumns[index].idText), scRefRunColumns[index].fmt);
	}
	SetRefRunColumnHeaders();
	for (index = 0; index < scNumRefRunColumns; ++index)
		m_ctrlRefRuns->SetColumnWidth(index, wxLIST_AUTOSIZE_USEHEADER);

	wxButton* btnRefNew = new wxButton(panelRefRuns, wxID_ANY,
		_("IDC_RUNREF_NEW"),
		wxDefaultPosition, wxDefaultSize, 0);
	btnRefNew->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnRefRunNew), NULL, this);
	btnRefNew->SetHelpText(_("HIDC_RUNREF_NEW"));
	btnRefNew->SetToolTip(_("HIDC_RUNREF_NEW"));

	m_ctrlRefAddMe = new wxButton(panelRefRuns, wxID_ANY,
		_("IDC_RUNREF_ADDDOG"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlRefAddMe->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnRefRunAddMe), NULL, this);
	m_ctrlRefAddMe->SetHelpText(_("HIDC_RUNREF_ADDDOG"));
	m_ctrlRefAddMe->SetToolTip(_("HIDC_RUNREF_ADDDOG"));

	m_ctrlRefEdit = new wxButton(panelRefRuns, wxID_ANY,
		_("IDC_RUNREF_EDIT"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlRefEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnRefRunEdit), NULL, this);
	m_ctrlRefEdit->SetHelpText(_("HIDC_RUNREF_EDIT"));
	m_ctrlRefEdit->SetToolTip(_("HIDC_RUNREF_EDIT"));

	m_ctrlRefDelete = new wxButton(panelRefRuns, wxID_ANY,
		_("IDC_RUNREF_DELETE"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlRefDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnRefRunDelete), NULL, this);
	m_ctrlRefDelete->SetHelpText(_("HIDC_RUNREF_DELETE"));
	m_ctrlRefDelete->SetToolTip(_("HIDC_RUNREF_DELETE"));

	// CRCD

	wxPanel* panelCRCD = new wxPanel(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxStaticText* textCRCD = new wxStaticText(panelCRCD, wxID_ANY,
		_("IDC_RUN_CRCD"),
		wxDefaultPosition, wxDefaultSize, 0);
	textCRCD->Wrap(500);

	m_CRCDDisplay = new CMetaDataDisplay(panelCRCD, m_Run);

	if (m_CRCDDisplay->HasMetafileSupport())
	{
		m_ctrlCourse = new wxButton(panelCRCD, wxID_ANY,
			_("IDC_RUNCRCD_EDIT"),
			wxDefaultPosition, wxDefaultSize, 0);
		m_ctrlCourse->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnCRCDEdit), NULL, this);
		m_ctrlCourse->SetHelpText(_("HIDC_RUNCRCD_EDIT"));
		m_ctrlCourse->SetToolTip(_("HIDC_RUNCRCD_EDIT"));

		m_ctrlCRCDView = new wxButton(panelCRCD, wxID_ANY,
			_("IDC_RUNCRCD_VIEW"),
			wxDefaultPosition, wxDefaultSize, 0);
		m_ctrlCRCDView->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnCRCDView), NULL, this);
		m_ctrlCRCDView->SetHelpText(_("HIDC_RUNCRCD_VIEW"));
		m_ctrlCRCDView->SetToolTip(_("HIDC_RUNCRCD_VIEW"));
	}

	m_ctrlCRCDCopy = new wxButton(panelCRCD, wxID_ANY,
		_("IDC_RUNCRCD_COPY"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlCRCDCopy->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnCRCDCopy), NULL, this);
	m_ctrlCRCDCopy->SetHelpText(_("HIDC_RUNCRCD_COPY"));
	m_ctrlCRCDCopy->SetToolTip(_("HIDC_RUNCRCD_COPY"));

	wxStaticText* textImageDesc = NULL;
	if (m_CRCDDisplay->HasMetafileSupport())
	{
		m_ctrlIncImage = new wxCheckBox(panelCRCD, wxID_ANY,
			_("IDC_RUNCRCD_IMAGE"),
			wxDefaultPosition, wxDefaultSize, 0);
		m_ctrlIncImage->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CDlgRun::OnCRCDImage), NULL, this);
		m_ctrlIncImage->SetHelpText(_("HIDC_RUNCRCD_IMAGE"));
		m_ctrlIncImage->SetToolTip(_("HIDC_RUNCRCD_IMAGE"));
		bool setCheck = CAgilityBookOptions::GetIncludeCRCDImage();
		if (m_CRCDDisplay->HasMetafile())
			setCheck = true;
		m_ctrlIncImage->SetValue(setCheck);

		textImageDesc = new wxStaticText(panelCRCD, wxID_ANY,
			_("IDC_RUNCRCD_IMAGE_TEXT"),
			wxDefaultPosition, wxDefaultSize, 0);
		textImageDesc->Wrap(80);
	}

	// Links

	wxPanel* panelLinks = new wxPanel(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	m_ctrlLinks = new CListCtrl(panelLinks, wxID_ANY,
		wxDefaultPosition, wxDefaultSize,
		wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL);
	m_ctrlLinks->SetImageList(&m_ImageList, wxIMAGE_LIST_SMALL);
	m_imgEmpty = m_ImageList.Add(wxIcon(CalEmpty_xpm));
	m_imgOk = m_ImageList.Add(wxIcon(CalPlan_xpm));
	m_imgMissing = m_ImageList.Add(wxIcon(CalTentative_xpm));
	m_ctrlLinks->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(CDlgRun::OnLinksItemSelected), NULL, this);
	m_ctrlLinks->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(CDlgRun::OnLinksItemActivated), NULL, this);
	m_ctrlLinks->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CDlgRun::OnLinksKeyDown), NULL, this);
	m_ctrlLinks->SetHelpText(_("HIDC_RUNLINK_LIST"));
	m_ctrlLinks->SetToolTip(_("HIDC_RUNLINK_LIST"));
	m_ctrlLinks->InsertColumn(0, wxT(""));

	wxButton* btnLinkNew = new wxButton(panelLinks, wxID_ANY,
		_("IDC_RUNLINK_NEW"),
		wxDefaultPosition, wxDefaultSize, 0);
	btnLinkNew->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnLinksNew), NULL, this);
	btnLinkNew->SetHelpText(_("HIDC_RUNLINK_NEW"));
	btnLinkNew->SetToolTip(_("HIDC_RUNLINK_NEW"));

	m_ctrlLinkEdit = new wxButton(panelLinks, wxID_ANY,
		_("IDC_RUNLINK_EDIT"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlLinkEdit->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnLinksEdit), NULL, this);
	m_ctrlLinkEdit->SetHelpText(_("HIDC_RUNLINK_EDIT"));
	m_ctrlLinkEdit->SetToolTip(_("HIDC_RUNLINK_EDIT"));

	m_ctrlLinkDelete = new wxButton(panelLinks, wxID_ANY,
		_("IDC_RUNLINK_DELETE"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlLinkDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnLinksDelete), NULL, this);
	m_ctrlLinkDelete->SetHelpText(_("HIDC_RUNLINK_DELETE"));
	m_ctrlLinkDelete->SetToolTip(_("HIDC_RUNLINK_DELETE"));

	m_ctrlLinkOpen = new wxButton(panelLinks, wxID_ANY,
		_("IDC_RUNLINK_OPEN"),
		wxDefaultPosition, wxDefaultSize, 0);
	m_ctrlLinkOpen->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CDlgRun::OnLinksOpen), NULL, this);
	m_ctrlLinkOpen->SetHelpText(_("HIDC_RUNLINK_OPEN"));
	m_ctrlLinkOpen->SetToolTip(_("HIDC_RUNLINK_OPEN"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizerScorePanel = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizerDate = new wxBoxSizer(wxHORIZONTAL);
	sizerDate->Add(textDate, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerDate->Add(ctrlDate, 0, wxALL, 5);
	sizerDate->Add(textVenue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerDate->Add(textClub, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerDate->Add(textLocation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerScorePanel->Add(sizerDate, 0, wxEXPAND, 5);

	wxBoxSizer* sizerDivHt = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* sizerEvent = new wxFlexGridSizer(5, 2, 0, 0);
	sizerEvent->SetFlexibleDirection(wxBOTH);
	sizerEvent->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizerEvent->Add(textDiv, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerEvent->Add(m_ctrlDivisions, 0, wxALL, 5);
	sizerEvent->Add(textLevel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerEvent->Add(m_ctrlLevels, 0, wxALL, 5);
	sizerEvent->Add(textEvent, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerEvent->Add(m_ctrlEvents, 0, wxALL, 5);
	sizerEvent->Add(m_ctrlSubNamesText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerEvent->Add(m_ctrlSubNames, 0, wxALL, 5);
	sizerEvent->Add(0, 0, 1, wxEXPAND, 5);
	sizerEvent->Add(m_ctrlTable, 0, wxALL, 5);

	sizerDivHt->Add(sizerEvent, 0, wxEXPAND, 5);

	wxBoxSizer* sizerHtCond = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizerHt = new wxBoxSizer(wxHORIZONTAL);
	sizerHt->Add(textHeight, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerHt->Add(m_ctrlHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerHt->Add(textJudge, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerHt->Add(m_ctrlJudge, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerHt->Add(ctrlJudgeNote, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxTOP, 5);
	sizerHt->Add(textHandler, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerHt->Add(m_ctrlHandler, 0, wxALL, 5);

	sizerHtCond->Add(sizerHt, 0, wxEXPAND, 5);

	wxBoxSizer* sizerCond = new wxBoxSizer(wxHORIZONTAL);
	sizerCond->Add(textConditions, 0, wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerCond->Add(m_ctrlConditions, 1, wxALL|wxEXPAND, 5);

	sizerHtCond->Add(sizerCond, 1, wxEXPAND, 5);

	wxBoxSizer* sizerComments = new wxBoxSizer(wxHORIZONTAL);
	sizerComments->Add(m_ctrlDesc, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerPartner = new wxBoxSizer(wxVERTICAL);
	sizerPartner->Add(m_ctrlPartnerEdit, 0, wxALL, 5);
	sizerPartner->Add(m_ctrlPartner, 0, wxALL, 5);

	sizerComments->Add(sizerPartner, 0, wxEXPAND, 5);

	sizerHtCond->Add(sizerComments, 0, wxEXPAND, 5);

	sizerDivHt->Add(sizerHtCond, 0, wxEXPAND, 5);

	sizerScorePanel->Add(sizerDivHt, 0, wxEXPAND, 5);

	wxFlexGridSizer* sizerResults = new wxFlexGridSizer(5, 4, 0, 0);
	sizerResults->SetFlexibleDirection(wxBOTH);
	sizerResults->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	wxBoxSizer* sizerSCT = new wxBoxSizer(wxHORIZONTAL);
	sizerSCT->Add(m_ctrlSCTText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerSCT->Add(m_ctrlSCT, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerResults->Add(sizerSCT, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerTime = new wxBoxSizer(wxHORIZONTAL);
	sizerTime->Add(m_ctrlTimeText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerTime->Add(m_ctrlTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerResults->Add(sizerTime, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerPlace = new wxBoxSizer(wxHORIZONTAL);
	sizerPlace->Add(textPlace, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerPlace->Add(m_ctrlPlace, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerPlace->Add(textPlaceOf, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxTOP, 5);
	sizerPlace->Add(m_ctrlInClass, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerResults->Add(sizerPlace, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerBonus = new wxBoxSizer(wxHORIZONTAL);
	sizerBonus->Add(m_ctrlBonusPtsText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	sizerBonus->Add(m_ctrlBonusPts, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerResults->Add(sizerBonus, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerYardsReqOpeningPts = new wxBoxSizer(wxHORIZONTAL);
	sizerYardsReqOpeningPts->Add(m_textYardsReqOpeningPts, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerYardsReqOpeningPts->Add(m_ctrlYardsReqOpeningPts, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerYardsReqOpeningPts, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerFaults = new wxBoxSizer(wxHORIZONTAL);
	sizerFaults->Add(m_ctrlFaultsText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerFaults->Add(m_ctrlFaults, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerFaults, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerDogsQd = new wxBoxSizer(wxHORIZONTAL);
	sizerDogsQd->Add(textDogsQd, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerDogsQd->Add(m_ctrlDogsQd, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerDogsQd, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerSpeed = new wxBoxSizer(wxHORIZONTAL);
	sizerSpeed->Add(m_ctrlSpeedPtsText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerSpeed->Add(m_ctrlSpeedPts, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerSpeed, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerMinYPSClosingTime = new wxBoxSizer(wxHORIZONTAL);
	sizerMinYPSClosingTime->Add(m_textMinYPSClosingTime, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerMinYPSClosingTime->Add(m_ctrlMinYPSClosingTime, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerMinYPSClosingTime, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerYPSOpeningPts = new wxBoxSizer(wxHORIZONTAL);
	sizerYPSOpeningPts->Add(m_textYPSOpeningPts, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerYPSOpeningPts->Add(m_ctrlYPSOpeningPts, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerYPSOpeningPts, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerQ = new wxBoxSizer(wxHORIZONTAL);
	sizerQ->Add(textQ, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerQ->Add(m_ctrlQ, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerQ, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerTitlePts = new wxBoxSizer(wxHORIZONTAL);
	sizerTitlePts->Add(m_ctrlTitlePointsText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerTitlePts->Add(m_ctrlTitlePoints, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerTitlePts, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerReqClosingPts = new wxBoxSizer(wxHORIZONTAL);
	sizerReqClosingPts->Add(m_ctrlClosingText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerReqClosingPts->Add(m_ctrlClosing, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerReqClosingPts, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerClosingPtsTotalFaults = new wxBoxSizer(wxHORIZONTAL);
	sizerClosingPtsTotalFaults->Add(m_textClosingPtsTotalFaults, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerClosingPtsTotalFaults->Add(m_ctrlClosingPtsTotalFaults, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerClosingPtsTotalFaults, 0, wxALIGN_RIGHT, 5);

	sizerResults->Add(0, 0, 1, wxEXPAND, 5);

	wxBoxSizer* sizerScore = new wxBoxSizer(wxHORIZONTAL);
	sizerScore->Add(textScore, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerScore->Add(m_ctrlScore, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerScore, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerNumObs = new wxBoxSizer(wxHORIZONTAL);
	sizerNumObs->Add(ctrlObstaclesText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerNumObs->Add(m_ctrlObstacles, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerNumObs, 0, wxALIGN_RIGHT, 5);

	wxBoxSizer* sizerObsSec = new wxBoxSizer(wxHORIZONTAL);
	sizerObsSec->Add(ctrlObstaclesPSText, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5);
	sizerObsSec->Add(m_ctrlObstaclesPS, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxRIGHT, 5);

	sizerResults->Add(sizerObsSec, 0, wxALIGN_RIGHT, 5);
	sizerResults->Add(0, 0, 1, wxEXPAND, 5);
	sizerResults->Add(btnOtherPoints, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5);

	sizerScorePanel->Add(sizerResults, 1, wxEXPAND, 5);

	m_panelScore->SetSizer(sizerScorePanel);
	m_panelScore->Layout();
	sizerScorePanel->Fit(m_panelScore);
	notebook->AddPage(m_panelScore, _("IDD_RUN_SCORE"), false);

	wxBoxSizer* sizerCommentsFaults = new wxBoxSizer(wxHORIZONTAL);
	sizerCommentsFaults->Add(ctrlComments, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerFaultsList = new wxBoxSizer(wxVERTICAL);
	sizerFaultsList->Add(m_ctrlFaultsList, 1, wxALL, 5);
	sizerFaultsList->Add(btnFaults, 0, wxALL, 5);

	sizerCommentsFaults->Add(sizerFaultsList, 0, wxEXPAND, 5);

	panelComments->SetSizer(sizerCommentsFaults);
	panelComments->Layout();
	sizerCommentsFaults->Fit(panelComments);
	notebook->AddPage(panelComments, _("IDD_RUN_COMMENTS"), false);

	wxBoxSizer* sizerRefRuns = new wxBoxSizer(wxVERTICAL);
	sizerRefRuns->Add(m_ctrlRefRuns, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerRefBtns = new wxBoxSizer(wxHORIZONTAL);
	sizerRefBtns->Add(btnRefNew, 0, wxALL, 5);
	sizerRefBtns->Add(m_ctrlRefAddMe, 0, wxALL, 5);
	sizerRefBtns->Add(m_ctrlRefEdit, 0, wxALL, 5);
	sizerRefBtns->Add(m_ctrlRefDelete, 0, wxALL, 5);
	sizerRefRuns->Add(sizerRefBtns, 0, wxEXPAND, 5);

	panelRefRuns->SetSizer(sizerRefRuns);
	panelRefRuns->Layout();
	sizerRefRuns->Fit(panelRefRuns);
	notebook->AddPage(panelRefRuns, _("IDD_RUN_REFERENCE"), false);
	m_idxRefRunPage = static_cast<int>(notebook->GetPageCount()) - 1;

	wxBoxSizer* sizerCRCD = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizerDisplay = new wxBoxSizer(wxVERTICAL);
	sizerDisplay->Add(textCRCD, 0, wxALL, 5);
	sizerDisplay->Add(m_CRCDDisplay, 1, wxALL|wxEXPAND, 5);

	sizerCRCD->Add(sizerDisplay, 1, wxEXPAND, 5);

	wxBoxSizer* sizerBtnsCRCD = new wxBoxSizer(wxVERTICAL);
	if (m_ctrlCourse)
		sizerBtnsCRCD->Add(m_ctrlCourse, 0, wxALL, 5);
	if (m_ctrlCRCDView)
		sizerBtnsCRCD->Add(m_ctrlCRCDView, 0, wxALL, 5);
	sizerBtnsCRCD->Add(m_ctrlCRCDCopy, 0, wxALL, 5);
	if (m_ctrlIncImage)
		sizerBtnsCRCD->Add(m_ctrlIncImage, 0, wxALL, 5);
	if (textImageDesc)
		sizerBtnsCRCD->Add(textImageDesc, 0, wxALL, 5);

	sizerCRCD->Add(sizerBtnsCRCD, 0, wxEXPAND, 5);

	panelCRCD->SetSizer(sizerCRCD);
	panelCRCD->Layout();
	sizerCRCD->Fit(panelCRCD);
	notebook->AddPage(panelCRCD, _("IDD_RUN_CRCD"), false);

	wxBoxSizer* sizerLinks = new wxBoxSizer(wxVERTICAL);
	sizerLinks->Add(m_ctrlLinks, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerLinkBtns = new wxBoxSizer(wxHORIZONTAL);
	sizerLinkBtns->Add(btnLinkNew, 0, wxALL, 5);
	sizerLinkBtns->Add(m_ctrlLinkEdit, 0, wxALL, 5);
	sizerLinkBtns->Add(m_ctrlLinkDelete, 0, wxALL, 5);
	sizerLinkBtns->Add(0, 0, 1, wxEXPAND, 5);
	sizerLinkBtns->Add(m_ctrlLinkOpen, 0, wxALL, 5);

	sizerLinks->Add(sizerLinkBtns, 0, wxEXPAND, 5);

	panelLinks->SetSizer(sizerLinks);
	panelLinks->Layout();
	sizerLinks->Fit(panelLinks);
	notebook->AddPage(panelLinks, _("IDD_RUN_LINK"), false);

	bSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

	wxSizer* sdbSizer = CreateButtonSizer(wxOK|wxCANCEL);
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	FillDivisions(); // This will call UpdateControls();
	FillJudges();
	SetPartnerText();
	SetFaultsText();
	ListRefRuns();
	UpdateCRCDButtons();
	ListLinkFiles(NULL);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnParent();

	notebook->SetSelection(iSelectPage);
}


CDlgDogDivData* CDlgRun::GetDivisionData(int index) const
{
	return dynamic_cast<CDlgDogDivData*>(m_ctrlDivisions->GetClientObject(index));
}


CDlgDogLevelData* CDlgRun::GetLevelData(int index) const
{
	return dynamic_cast<CDlgDogLevelData*>(m_ctrlLevels->GetClientObject(index));
}


bool CDlgRun::GetEvent(ARBConfigEventPtr* outEvent) const
{
	if (outEvent)
		outEvent->reset();
	int index = m_ctrlEvents->GetSelection();
	if (wxNOT_FOUND == index)
		return false;
	wxString str = m_ctrlEvents->GetString(index);
	if (str.empty())
		return false;
	return m_pVenue->GetEvents().FindEvent(str, outEvent);
}


bool CDlgRun::GetScoring(ARBConfigScoringPtr* outScoring) const
{
	bool bFound = false;
	if (outScoring)
		outScoring->reset();
	wxString div, level;
	int index = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != index)
	{
		div = m_ctrlDivisions->GetString(index);
	}
	index = m_ctrlLevels->GetSelection();
	if (wxNOT_FOUND != index)
	{
		level = m_ctrlLevels->GetString(index);
		CDlgDogLevelData* pLevel = GetLevelData(index);
		assert(pLevel);
		ARBConfigEventPtr pEvent;
		if (GetEvent(&pEvent))
		{
			if (0 < div.length() && 0 < level.length())
				bFound = pEvent->FindEvent(div, pLevel->m_pLevel->GetName(), m_Run->GetDate(), outScoring);
		}
	}
	return bFound;
}


void CDlgRun::FillDivisions()
{
	wxString div;
	long index = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != index)
	{
		div = m_ctrlDivisions->GetString(index);
	}
	if (div.empty())
		div = m_Run->GetDivision();
	m_ctrlDivisions->Clear();

	for (ARBConfigDivisionList::const_iterator iterDiv = m_pVenue->GetDivisions().begin();
		iterDiv != m_pVenue->GetDivisions().end();
		++iterDiv)
	{
		ARBConfigDivisionPtr pDiv = (*iterDiv);
		// Does this division have any events possible on this date?
		for (ARBConfigEventList::const_iterator iterEvent = m_pVenue->GetEvents().begin();
			iterEvent != m_pVenue->GetEvents().end();
			++iterEvent)
		{
			if ((*iterEvent)->VerifyEvent(pDiv->GetName(), WILDCARD_LEVEL, m_Run->GetDate()))
			{
				index = m_ctrlDivisions->Append(pDiv->GetName());
				m_ctrlDivisions->SetClientObject(index, new CDlgDogDivData(pDiv));
				if (pDiv->GetName() == div)
					m_ctrlDivisions->SetSelection(index);
				break;
			}
		}
	}
	// First try to find 'div' (in case the divisions changed)
	if (wxNOT_FOUND == m_ctrlDivisions->GetSelection() && !div.empty())
	{
		index = m_ctrlDivisions->FindString(div, true);
		if (0 <= index)
			m_ctrlDivisions->SetSelection(index);
	}
	// Then try to find the last entered
	if (wxNOT_FOUND == m_ctrlDivisions->GetSelection())
	{
		wxString last = CAgilityBookOptions::GetLastEnteredDivision();
		if (0 < last.length())
		{
			index = m_ctrlDivisions->FindString(last, true);
			if (0 <= index)
				m_ctrlDivisions->SetSelection(index);
		}
	}
	if (wxNOT_FOUND == m_ctrlDivisions->GetSelection())
	{
		if (1 == m_ctrlDivisions->GetCount())
			m_ctrlDivisions->SetSelection(0);
	}
	// Force the division into the run. SetSelection will not cause an update.
	int idxDiv = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != idxDiv)
		m_Run->SetDivision(m_ctrlDivisions->GetString(idxDiv));
	FillLevels();
}


void CDlgRun::FillLevels()
{
	wxString level;
	int index = m_ctrlLevels->GetSelection();
	if (wxNOT_FOUND != index)
	{
		level = m_ctrlLevels->GetString(index);
	}
	if (level.empty())
		level = m_Run->GetLevel();
	m_ctrlLevels->Clear();
	index = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != index)
	{
		ARBConfigDivisionPtr pDiv = GetDivisionData(index)->m_Div;
		for (ARBConfigLevelList::const_iterator iter = pDiv->GetLevels().begin();
			iter != pDiv->GetLevels().end();
			++iter)
		{
			ARBConfigLevelPtr pLevel = (*iter);
			// Does this level have any events possible on this date?
			for (ARBConfigEventList::const_iterator iterEvent = m_pVenue->GetEvents().begin();
				iterEvent != m_pVenue->GetEvents().end();
				++iterEvent)
			{
				if ((*iterEvent)->VerifyEvent(pDiv->GetName(), pLevel->GetName(), m_Run->GetDate()))
				{
					if (0 < pLevel->GetSubLevels().size())
					{
						for (ARBConfigSubLevelList::const_iterator iterSub = pLevel->GetSubLevels().begin();
							iterSub != pLevel->GetSubLevels().end();
							++iterSub)
						{
							ARBConfigSubLevelPtr pSubLevel = (*iterSub);
							int idx = m_ctrlLevels->Append(pSubLevel->GetName());
							m_ctrlLevels->SetClientObject(idx, new CDlgDogLevelData(pLevel, pSubLevel));
							if (level == pSubLevel->GetName())
								m_ctrlLevels->SetSelection(idx);
						}
					}
					else
					{
						int idx = m_ctrlLevels->Append(pLevel->GetName());
						m_ctrlLevels->SetClientObject(idx, new CDlgDogLevelData(pLevel));
						if (level == pLevel->GetName())
							m_ctrlLevels->SetSelection(idx);
					}
					break;
				}
			}
		}
		if (wxNOT_FOUND == m_ctrlLevels->GetSelection())
		{
			wxString last = CAgilityBookOptions::GetLastEnteredLevel();
			if (0 < last.length())
			{
				int idx = m_ctrlLevels->FindString(last, true);
				if (0 <= idx)
					m_ctrlLevels->SetSelection(idx);
			}
		}
	}
	if (wxNOT_FOUND == m_ctrlLevels->GetSelection())
	{
		if (1 == m_ctrlLevels->GetCount())
			m_ctrlLevels->SetSelection(0);
	}
	int idxLvl = m_ctrlLevels->GetSelection();
	if (wxNOT_FOUND != idxLvl)
		m_Run->SetLevel(m_ctrlLevels->GetString(idxLvl));
	FillEvents();
	SetTitlePoints();
}


void CDlgRun::FillEvents()
{
	wxString evt;
	int index = m_ctrlEvents->GetSelection();
	if (wxNOT_FOUND != index)
	{
		evt = m_ctrlEvents->GetString(index);
	}
	if (evt.empty())
		evt = m_Run->GetEvent();
	m_ctrlEvents->Clear();
	int idxDiv = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != idxDiv)
	{
		ARBConfigDivisionPtr pDiv = GetDivisionData(idxDiv)->m_Div;
		int idxLevel = m_ctrlLevels->GetSelection();
		if (wxNOT_FOUND != idxLevel)
		{
			CDlgDogLevelData* pData = GetLevelData(idxLevel);
			for (ARBConfigEventList::const_iterator iter = m_pVenue->GetEvents().begin();
				iter != m_pVenue->GetEvents().end();
				++iter)
			{
				ARBConfigEventPtr pEvent = (*iter);
				if (pEvent->FindEvent(pDiv->GetName(), pData->m_pLevel->GetName(), m_Run->GetDate()))
				{
					int idx = m_ctrlEvents->Append(pEvent->GetName());
					if (evt == pEvent->GetName())
					{
						m_ctrlEvents->SetSelection(idx);
						SetEventDesc(pEvent);
					}
				}
			}
		}
	}
	FillSubNames();
	UpdateControls();
}


void CDlgRun::FillSubNames()
{
	ARBConfigEventPtr pEvent;
	if (GetEvent(&pEvent))
	{
		if (pEvent->HasSubNames())
		{
			std::set<wxString> names;
			m_pDoc->Book().GetAllEventSubNames(m_pVenue->GetName(), pEvent, names);
			m_ctrlSubNames->Clear();
			for (std::set<wxString>::const_iterator iter = names.begin();
				iter != names.end();
				++iter)
			{
				m_ctrlSubNames->Append(*iter);
			}
			m_ctrlSubNamesText->Show(true);
			m_ctrlSubNames->Show(true);
		}
		else
		{
			m_ctrlSubNamesText->Show(false);
			m_ctrlSubNames->Show(false);
		}
	}
	else
	{
		m_ctrlSubNamesText->Show(false);
		m_ctrlSubNames->Show(false);
	}
}


void CDlgRun::FillJudges()
{
	std::set<wxString> names;
	m_pDoc->Book().GetAllJudges(names, true, true);
	if (!m_Run->GetJudge().empty())
		names.insert(m_Run->GetJudge());
	m_ctrlJudge->Clear();
	for (std::set<wxString>::const_iterator iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlJudge->Append(*iter);
	}
	m_ctrlJudge->SetValue(m_Judge);
}


void CDlgRun::SetEventDesc(ARBConfigEventPtr inEvent)
{
	wxString desc;
	if (inEvent)
		desc += inEvent->GetDesc();
	ARBConfigScoringPtr pScoring;
	if (GetScoring(&pScoring))
	{
		wxString const& note = pScoring->GetNote();
		if (!desc.empty() && 0 < note.length())
			desc += wxT("\n==========\n");
		desc += note;
	}
	m_ctrlDesc->ChangeValue(desc);
}


void CDlgRun::SetPartnerText()
{
	wxString partners;
	ARBConfigEventPtr pEvent;
	if (GetEvent(&pEvent))
	{
		if (pEvent->HasPartner())
		{
			for (ARBDogRunPartnerList::const_iterator iter = m_Run->GetPartners().begin(); iter != m_Run->GetPartners().end(); ++iter)
			{
				if (!partners.empty())
					partners += wxT(", ");
				partners += (*iter)->GetHandler();
				partners += wxT("/");
				partners += (*iter)->GetDog();
			}
		}
	}
	m_ctrlPartner->ChangeValue(partners);
}


void CDlgRun::SetMinYPS()
{
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		wxString str;
		double yps;
		if (m_Run->GetScoring().GetMinYPS(CAgilityBookOptions::GetTableInYPS(), yps))
		{
			str = ARBDouble::str(yps, 3);
		}
		m_ctrlMinYPSClosingTime->ChangeValue(str);
	}
}


void CDlgRun::SetYPS()
{
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		wxString str;
		double yps;
		if (m_Run->GetScoring().GetYPS(CAgilityBookOptions::GetTableInYPS(), yps))
		{
			str = ARBDouble::str(yps, 3);
		}
		m_ctrlYPSOpeningPts->ChangeValue(str);
	}
}


void CDlgRun::SetObstacles()
{
	wxString str;
	double ops;
	if (m_Run->GetScoring().GetObstaclesPS(CAgilityBookOptions::GetTableInYPS(), ops))
	{
		str = ARBDouble::str(ops, 3);
	}
	m_ctrlObstaclesPS->ChangeValue(str);
}


void CDlgRun::SetTotalFaults()
{
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		wxString total;
		ARBConfigScoringPtr pScoring;
		GetScoring(&pScoring);
		double faults = m_Run->GetScoring().GetCourseFaults() + m_Run->GetScoring().GetTimeFaults(pScoring);
		total = ARBDouble::str(faults, 3);
		m_ctrlClosingPtsTotalFaults->ChangeValue(total);
	}
}


void CDlgRun::SetTitlePoints()
{
	int index = m_ctrlQ->GetSelection();
	if (wxNOT_FOUND == index)
	{
		m_ctrlQ->SetFocus();
		return;
	}
	ARB_Q q = m_ctrlQ->GetQ(index);

	wxString strBonus(wxT("0"));
	wxString strSpeed(wxT("0"));
	wxString strTitle(wxT("0"));
	wxString strScore(wxT(""));
	ARBConfigScoringPtr pScoring;
	if (GetScoring(&pScoring))
	{
		// 8/17/03: Only compute title points on Q runs.
		if (q.Qualified())
		{
			if (pScoring->HasBonusPts())
			{
				strBonus.clear();
				strBonus << m_Run->GetScoring().GetBonusPts();
			}
			if (pScoring->HasSpeedPts())
			{
				strSpeed.clear();
				strSpeed << m_Run->GetSpeedPoints(pScoring);
			}
			strTitle.clear();
			strTitle << m_Run->GetTitlePoints(pScoring);
		}
		// 8/17/03: Only compute score on Q and NQ runs.
		// 11/13/04: Also compute score for NA runs that have no titling pts.
		if (q.Qualified()
		|| ARB_Q::eQ_NQ == q
		|| (ARB_Q::eQ_NA == q && ARBDouble::equal(0.0, static_cast<double>(pScoring->GetTitlePoints().size()))))
			strScore = ARBDouble::str(m_Run->GetScore(pScoring));
	}
	// Doesn't matter if they're hidden,..
	m_ctrlBonusPts->ChangeValue(strBonus);
	m_ctrlSpeedPts->ChangeValue(strSpeed);
	m_ctrlTitlePoints->ChangeValue(strTitle);
	m_ctrlScore->ChangeValue(strScore);
}


void CDlgRun::SetReadOnlyFlag(CTextCtrl* ctrl, bool bReadOnly)
{
	if (!ctrl)
		return;
	if (bReadOnly)
	{
		ctrl->SetEditable(false);
		ctrl->SetBackgroundColour(m_clrBack);
	}
	else
	{
		ctrl->SetEditable(true);
		ctrl->SetBackgroundColour(wxNullColour);
	}
}


void CDlgRun::FixScoreLayout()
{
	// If not set, we're in the constructor still and that will do the layout.
	if (!GetSizer())
		return;
	m_panelScore->Layout();
	m_panelScore->GetSizer()->Fit(m_panelScore);
	Layout();
	GetSizer()->Fit(this);
}


void CDlgRun::UpdateControls(bool bOnEventChange)
{
	m_ctrlHeight->Enable(false);
	m_ctrlJudge->Enable(false);
	m_ctrlHandler->Enable(false);
	m_ctrlConditions->Enable(false);
	m_ctrlPartnerEdit->Show(false);
	m_ctrlPartner->Show(false);
	m_ctrlTable->Show(false);
	m_ctrlSCTText->Show(false);
	m_ctrlSCT->Show(false);
	m_textYardsReqOpeningPts->Show(false);
	m_ctrlYardsReqOpeningPts->Show(false);
	m_textMinYPSClosingTime->Show(false);
	m_ctrlMinYPSClosingTime->Show(false);
	m_ctrlClosingText->Show(false);
	m_ctrlClosing->Show(false);
	m_ctrlObstacles->Enable(false);
	m_ctrlTimeText->Show(false);
	m_ctrlTime->Show(false);
	m_ctrlFaultsText->Show(false);
	m_ctrlFaults->Show(false);
	m_textYPSOpeningPts->Show(false);
	m_ctrlYPSOpeningPts->Show(false);
	m_textClosingPtsTotalFaults->Show(false);
	m_ctrlClosingPtsTotalFaults->Show(false);
	m_ctrlObstaclesPS->Enable(false);
	m_ctrlPlace->Enable(false);
	m_ctrlInClass->Enable(false);
	m_ctrlDogsQd->Enable(false);
	m_ctrlQ->Enable(false);
	m_ctrlBonusPtsText->Show(false);
	m_ctrlBonusPts->Show(false);
	m_ctrlSpeedPtsText->Show(false);
	m_ctrlSpeedPts->Show(false);
	m_ctrlTitlePointsText->Show(false);
	m_ctrlTitlePoints->Show(false);

	ARBConfigScoringPtr pScoring;
	if (!GetScoring(&pScoring))
	{
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeUnknown, false);
		FixScoreLayout();
		return;
	}

	ARBConfigEventPtr pEvent;
	if (!GetEvent(&pEvent))
	{
		FixScoreLayout();
		return;
	}

	m_ctrlHeight->Enable(true);
	m_ctrlJudge->Enable(true);
	m_ctrlHandler->Enable(true);
	m_ctrlConditions->Enable(true);

	if (pEvent->HasPartner())
	{
		m_ctrlPartnerEdit->Show(true);
		m_ctrlPartner->Show(true);
	}

	// 7/1/07: Resetting the table status must always be done - not just when
	// the new event has a table. (which is what was done before)
	if (bOnEventChange)
	{
		if (m_Run->GetScoring().HasTable() != pEvent->HasTable())
		{
			m_Run->GetScoring().SetHasTable(pEvent->HasTable());
			// Plus, we need to recompute the YPS.
			SetMinYPS();
			SetYPS();
			SetObstacles();
		}
	}
	if (pEvent->HasTable())
	{
		m_ctrlTable->Show(true);
		m_Table = m_Run->GetScoring().HasTable();
		m_ctrlTable->SetValue(m_Table);
	}

	wxString str;
	switch (pScoring->GetScoringStyle())
	{
	default:
		assert(0);
		break;
	case ARBConfigScoring::eFaultsThenTime:
	case ARBConfigScoring::eFaults100ThenTime:
	case ARBConfigScoring::eFaults200ThenTime:
	case ARBConfigScoring::eTimePlusFaults:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByTime, pScoring->DropFractions());
		m_ctrlSCTText->Show(true);
		m_ctrlSCT->Show(true);
		m_textYardsReqOpeningPts->SetLabel(_("IDC_RUNSCORE_YARDS"));
		m_textYardsReqOpeningPts->Show(true);
		m_ctrlYardsReqOpeningPts->SetHelpText(_("HIDC_RUNSCORE_YARDS"));
		m_ctrlYardsReqOpeningPts->SetToolTip(_("HIDC_RUNSCORE_YARDS"));
		m_ctrlYardsReqOpeningPts->ChangeValue(ARBDouble::str(m_Yards));
		m_ctrlYardsReqOpeningPts->Show(true);
		m_textMinYPSClosingTime->SetLabel(_("IDC_RUNSCORE_MIN_YPS"));
		m_textMinYPSClosingTime->Show(true);
		m_ctrlMinYPSClosingTime->Show(true);
		m_ctrlMinYPSClosingTime->SetHelpText(wxEmptyString);
		m_ctrlMinYPSClosingTime->SetToolTip(wxEmptyString);
		SetReadOnlyFlag(m_ctrlMinYPSClosingTime, true);
		m_ctrlTimeText->Show(true);
		m_ctrlTime->Show(true);
		m_ctrlFaultsText->Show(true);
		m_ctrlFaults->Show(true);
		m_textYPSOpeningPts->SetLabel(_("IDC_RUNSCORE_YPS"));
		m_textYPSOpeningPts->Show(true);
		m_ctrlYPSOpeningPts->SetHelpText(wxEmptyString);
		m_ctrlYPSOpeningPts->SetToolTip(wxEmptyString);
		m_ctrlYPSOpeningPts->Show(true);
		SetReadOnlyFlag(m_ctrlYPSOpeningPts, true);
		m_textClosingPtsTotalFaults->SetLabel(_("IDC_RUNSCORE_TOTAL_FAULTS"));
		m_textClosingPtsTotalFaults->Show(true);
		m_ctrlClosingPtsTotalFaults->SetHelpText(wxEmptyString);
		m_ctrlClosingPtsTotalFaults->SetToolTip(wxEmptyString);
		m_ctrlClosingPtsTotalFaults->Show(true);
		SetReadOnlyFlag(m_ctrlClosingPtsTotalFaults, true);
		break;
	case ARBConfigScoring::eOCScoreThenTime:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByOpenClose, pScoring->DropFractions());
		if (bOnEventChange)
		{
			if (ARBDogRunScoring::eTypeByOpenClose == m_pRealRun->GetScoring().GetType())
			{
				m_Opening = m_Run->GetScoring().GetNeedOpenPts();
				m_Closing = m_Run->GetScoring().GetNeedClosePts();
			}
			else
			{
				m_Opening = pScoring->GetRequiredOpeningPoints();
				m_Closing = pScoring->GetRequiredClosingPoints();
			}
		}
		// Do not push these (above) back into the run.
		// Otherwise this will overwrite valid values during OnInit.
		m_ctrlSCTText->Show(true);
		m_ctrlSCT->Show(true);
		m_textYardsReqOpeningPts->SetLabel(_("IDC_RUNSCORE_OPENING_PTS"));
		m_textYardsReqOpeningPts->Show(true);
		m_ctrlYardsReqOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPENING_PTS"));
		m_ctrlYardsReqOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPENING_PTS"));
		str.Printf(wxT("%hd"), m_Opening);
		m_ctrlYardsReqOpeningPts->ChangeValue(str);
		m_ctrlYardsReqOpeningPts->Show(true);
		m_textMinYPSClosingTime->SetLabel(_("IDC_RUNSCORE_SCT2"));
		m_textMinYPSClosingTime->Show(true);
		m_ctrlMinYPSClosingTime->Show(true);
		m_ctrlMinYPSClosingTime->SetHelpText(_("HIDC_RUNSCORE_SCT2"));
		m_ctrlMinYPSClosingTime->SetToolTip(_("HIDC_RUNSCORE_SCT2"));
		m_ctrlMinYPSClosingTime->ChangeValue(ARBDouble::str(m_SCT2));
		SetReadOnlyFlag(m_ctrlMinYPSClosingTime, false);
		m_ctrlClosingText->Show(true);
		str.Printf(wxT("%hd"), m_Closing);
		m_ctrlClosing->ChangeValue(str);
		m_ctrlClosing->Show(true);
		m_ctrlTimeText->Show(true);
		m_ctrlTime->Show(true);
		m_ctrlFaultsText->Show(true);
		m_ctrlFaults->Show(true);
		m_textYPSOpeningPts->SetLabel(_("IDC_RUNSCORE_OPEN_PTS"));
		m_textYPSOpeningPts->Show(true);
		m_ctrlYPSOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPEN_PTS"));
		m_ctrlYPSOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPEN_PTS"));
		str.Printf(wxT("%hd"), m_Open);
		m_ctrlYPSOpeningPts->ChangeValue(str);
		m_ctrlYPSOpeningPts->Show(true);
		SetReadOnlyFlag(m_ctrlYPSOpeningPts, false);
		m_textClosingPtsTotalFaults->SetLabel(_("IDC_RUNSCORE_CLOSE_PTS"));
		m_textClosingPtsTotalFaults->Show(true);
		m_ctrlClosingPtsTotalFaults->SetHelpText(_("HIDC_RUNSCORE_CLOSE_PTS"));
		m_ctrlClosingPtsTotalFaults->SetToolTip(_("HIDC_RUNSCORE_CLOSE_PTS"));
		str.Printf(wxT("%hd"), m_Close);
		m_ctrlClosingPtsTotalFaults->ChangeValue(str);
		m_ctrlClosingPtsTotalFaults->Show(true);
		SetReadOnlyFlag(m_ctrlClosingPtsTotalFaults, false);
		break;
	case ARBConfigScoring::eScoreThenTime:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByPoints, pScoring->DropFractions());
		if (bOnEventChange)
		{
			if (ARBDogRunScoring::eTypeByPoints == m_pRealRun->GetScoring().GetType())
				m_Opening = m_Run->GetScoring().GetNeedOpenPts();
			else
				m_Opening = pScoring->GetRequiredOpeningPoints();
		}
		// Do not push this back into the run.
		// Otherwise this will overwrite valid values during OnInit.
		m_ctrlSCTText->Show(true);
		m_ctrlSCT->Show(true);
		m_textYardsReqOpeningPts->SetLabel(_("IDS_SCORING_REQUIRED_POINTS"));
		m_textYardsReqOpeningPts->Show(true);
		m_ctrlYardsReqOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPENING_PTS"));
		m_ctrlYardsReqOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPENING_PTS"));
		str.Printf(wxT("%hd"), m_Opening);
		m_ctrlYardsReqOpeningPts->ChangeValue(str);
		m_ctrlYardsReqOpeningPts->Show(true);
		m_ctrlTimeText->Show(true);
		m_ctrlTime->Show(true);
		m_ctrlFaultsText->Show(true);
		m_ctrlFaults->Show(true);
		m_textYPSOpeningPts->SetLabel(_("IDS_POINTS"));
		m_textYPSOpeningPts->Show(true);
		m_ctrlYPSOpeningPts->SetHelpText(_("HIDC_RUNSCORE_OPEN_PTS"));
		m_ctrlYPSOpeningPts->SetToolTip(_("HIDC_RUNSCORE_OPEN_PTS"));
		str.Printf(wxT("%hd"), m_Open);
		m_ctrlYPSOpeningPts->ChangeValue(str);
		m_ctrlYPSOpeningPts->Show(true);
		SetReadOnlyFlag(m_ctrlYPSOpeningPts, false);
		break;
	}

	m_ctrlObstacles->Enable(true);
	m_ctrlObstaclesPS->Enable(true);
	m_ctrlPlace->Enable(true);
	m_ctrlInClass->Enable(true);
	if (0 < pScoring->GetTitlePoints().size() || 0 < pScoring->GetLifetimePoints().size())
	{
		m_ctrlDogsQd->Enable(true);
	}
	m_ctrlQ->Enable(true);
	m_ctrlQ->ResetContent(pScoring);
	if (0 < pScoring->GetTitlePoints().size())
	{
		m_ctrlTitlePointsText->Show(true);
		m_ctrlTitlePoints->Show(true);
	}
	if (pScoring->HasBonusPts())
	{
		m_ctrlBonusPtsText->Show(true);
		m_ctrlBonusPts->Show(true);
	}
	if (pScoring->HasSpeedPts())
	{
		m_ctrlSpeedPtsText->Show(true);
		m_ctrlSpeedPts->Show(true);
	}
	SetTitlePoints();
	SetObstacles();
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		SetMinYPS();
		SetYPS();
		SetTotalFaults();
	}
	FixScoreLayout();
}


void CDlgRun::SetFaultsText()
{
	m_ctrlFaultsList->Clear();
	for (ARBDogFaultList::const_iterator iter = m_Run->GetFaults().begin(); iter != m_Run->GetFaults().end(); ++iter)
	{
		m_ctrlFaultsList->Append(*iter);
	}
}


CDlgDogRefRunDataPtr CDlgRun::GetReferenceDataByData(long index) const
{
	return tr1::dynamic_pointer_cast<CDlgDogRefRunData, CListData>(m_ctrlRefRuns->GetDataByData(index));
}


CDlgDogRefRunDataPtr CDlgRun::GetReferenceData(long index) const
{
	return tr1::dynamic_pointer_cast<CDlgDogRefRunData, CListData>(m_ctrlRefRuns->GetData(index));
}


bool CDlgRun::IsRefRunMe()
{
	if (m_pRefRunMe)
	{
		for (ARBDogReferenceRunList::const_iterator iterRef = m_Run->GetReferenceRuns().begin();
			iterRef != m_Run->GetReferenceRuns().end();
			++iterRef)
		{
			if (*iterRef == m_pRefRunMe)
			{
				return true;
			}
		}
	}
	return false;
}


void CDlgRun::CreateRefRunMe()
{
	// Create the 'me' reference run.
	m_pRefRunMe = ARBDogReferenceRunPtr(ARBDogReferenceRun::New());
	m_pRefRunMe->SetQ(m_Run->GetQ());
	m_pRefRunMe->SetPlace(m_Run->GetPlace());
	m_pRefRunMe->SetName(m_pDoc->GetCurrentDog()->GetCallName());
	m_pRefRunMe->SetHeight(m_Run->GetHeight());
	m_pRefRunMe->SetBreed(m_pDoc->GetCurrentDog()->GetBreed());
	m_pRefRunMe->SetTime(m_Run->GetScoring().GetTime());
	ARBConfigScoringPtr pScoring;
	if (m_pDoc->Book().GetConfig().GetVenues().FindEvent(
		m_pVenue->GetName(),
		m_Run->GetEvent(),
		m_Run->GetDivision(),
		m_Run->GetLevel(),
		m_Run->GetDate(),
		NULL,
		&pScoring))
	{
		m_pRefRunMe->SetScore(ARBDouble::str(m_Run->GetScore(pScoring)));
	}

	// Now see if I'm already in there.
	// Only compare: Q/Place/Name/Time/Score.
	for (ARBDogReferenceRunList::const_iterator iterRef = m_Run->GetReferenceRuns().begin();
		iterRef != m_Run->GetReferenceRuns().end();
		++iterRef)
	{
		ARBDogReferenceRunPtr pRef = *iterRef;
		if (pRef->GetQ() == m_pRefRunMe->GetQ()
		&& pRef->GetPlace() == m_pRefRunMe->GetPlace()
		&& pRef->GetName() == m_pRefRunMe->GetName()
		&& ARBDouble::equal(pRef->GetTime(), m_pRefRunMe->GetTime())
		&& pRef->GetScore() == m_pRefRunMe->GetScore())
		{
			m_pRefRunMe = pRef;
			break;
		}
	}
	// Note: This means if we create 'me', modify it, switch tabs
	// and come back, 'addme' is enabled again.
}


void CDlgRun::UpdateRefRunButtons()
{
	m_ctrlRefAddMe->Enable(!IsRefRunMe());
	bool bSelected = (0 <= m_ctrlRefRuns->GetFirstSelected());
	m_ctrlRefEdit->Enable(bSelected);
	m_ctrlRefDelete->Enable(bSelected);
}


void CDlgRun::SetRefRunColumnHeaders()
{
	for (int i = 0; i < scNumRefRunColumns; ++i)
	{
		wxString str;
		str << wxGetTranslation(scRefRunColumns[i].idText)
			<< wxT(" (") << m_sortRefRuns.FindColumnOrder(i) + 1 << wxT(")");
		wxListItem item;
		item.SetMask(wxLIST_MASK_TEXT);
		item.SetText(str);
		m_ctrlRefRuns->SetColumn(i, item);
		m_ctrlRefRuns->SetColumnSort(i, m_sortRefRuns.IsDescending(i) ? -1 : 1);
	}
}


void CDlgRun::ListRefRuns()
{
	ARBDogReferenceRunPtr pSelected;
	long index = m_ctrlRefRuns->GetFirstSelected();
	if (0 <= index)
	{
		ARBDogReferenceRunPtr pData = GetReferenceData(index)->GetData();
		if (pData)
			pSelected = pData;
	}
	m_ctrlRefRuns->DeleteAllItems();

	index = 0;
	for (ARBDogReferenceRunList::const_iterator iterRef = m_Run->GetReferenceRuns().begin();
	iterRef != m_Run->GetReferenceRuns().end();
	++index, ++iterRef)
	{
		CDlgDogRefRunDataPtr data(new CDlgDogRefRunData(*iterRef));
		m_ctrlRefRuns->InsertItem(data);
	}
	for (index = 0; index < scNumRefRunColumns; ++index)
		m_ctrlRefRuns->SetColumnWidth(index, wxLIST_AUTOSIZE_USEHEADER);
	s_SortInfo.pThis = this;
	s_SortInfo.pCols = &m_sortRefRuns;
	m_ctrlRefRuns->SortItems(CompareRefRuns, 0);

	if (pSelected)
	{
		for (index = 0; index < m_ctrlRefRuns->GetItemCount(); ++index)
		{
			ARBDogReferenceRunPtr pRefRun = GetReferenceData(index)->GetData();
			if (pRefRun == pSelected) // compare by ptr is fine.
			{
				m_ctrlRefRuns->SetSelection(index, true);
				break;
			}
		}
	}
	UpdateRefRunButtons();
}


void CDlgRun::GetAllHeights(std::set<wxString>& outNames)
{
	m_pDoc->Book().GetAllHeights(outNames);
	for (long index = 0; index < m_ctrlRefRuns->GetItemCount(); ++index)
	{
		ARBDogReferenceRunPtr pRefRun = GetReferenceData(index)->GetData();
		if (pRefRun)
		{
			wxString const& ht = pRefRun->GetHeight();
			if (0 < ht.length())
				outNames.insert(ht);
		}
	}
}


void CDlgRun::GetAllCallNames(std::set<wxString>& outNames)
{
	m_pDoc->Book().GetAllCallNames(outNames);
	for (int index = 0; index < m_ctrlRefRuns->GetItemCount(); ++index)
	{
		ARBDogReferenceRunPtr pRefRun = GetReferenceData(index)->GetData();
		if (pRefRun)
		{
			wxString const& ht = pRefRun->GetName();
			if (0 < ht.length())
				outNames.insert(ht);
		}
	}
}


void CDlgRun::GetAllBreeds(std::set<wxString>& outNames)
{
	m_pDoc->Book().GetAllBreeds(outNames);
	for (int index = 0; index < m_ctrlRefRuns->GetItemCount(); ++index)
	{
		ARBDogReferenceRunPtr pRefRun = GetReferenceData(index)->GetData();
		if (pRefRun)
		{
			wxString const& ht = pRefRun->GetBreed();
			if (0 < ht.length())
				outNames.insert(ht);
		}
	}
}


void CDlgRun::EditRefRun()
{
	long nItem = m_ctrlRefRuns->GetFirstSelected();
	if (0 <= nItem)
	{
		std::set<wxString> heights, names, breeds;
		GetAllHeights(heights);
		GetAllCallNames(names);
		GetAllBreeds(breeds);
		ARBDogReferenceRunPtr pRef = GetReferenceData(nItem)->GetData();
		CDlgReferenceRun dlg(m_pDoc, m_Run, heights, names, breeds, pRef, this);
		if (wxID_OK == dlg.ShowModal())
			ListRefRuns();
	}
}


void CDlgRun::UpdateCRCDButtons()
{
	if (m_ctrlCRCDView)
	{
		if (m_CRCDDisplay->ViewText())
			m_ctrlCRCDView->SetLabel(_("IDS_CRCD_BTN_VIEWIMAGE"));
		else
			m_ctrlCRCDView->SetLabel(_("IDS_CRCD_BTN_VIEWTEXT"));
	}
	if (m_CRCDDisplay->HasCourse())
		m_ctrlCRCDCopy->SetLabel(_("IDS_CRCD_BTN_CLEARCOURSE"));
	else
		m_ctrlCRCDCopy->SetLabel(_("IDS_CRCD_BTN_INSERTCOURSE"));

	if (m_ctrlCourse)
		m_ctrlCourse->Enable(m_CRCDDisplay->HasMetafile());
	if (m_ctrlCRCDView)
		m_ctrlCRCDView->Enable(m_CRCDDisplay->HasMetafile());
}


void CDlgRun::UpdateLinksButtons()
{
	bool bEnable = (0 <= m_ctrlLinks->GetFirstSelected());
	m_ctrlLinkEdit->Enable(bEnable);
	m_ctrlLinkDelete->Enable(bEnable);
	m_ctrlLinkOpen->Enable(bEnable);
}


void CDlgRun::ListLinkFiles(wxChar const* pItem)
{
	wxBusyCursor wait;
	m_ctrlLinks->DeleteAllItems();
	std::set<wxString> links;
	m_Run->GetLinks(links);
	long i = 0;
	for (std::set<wxString>::iterator iter = links.begin();
		iter != links.end();
		++iter)
	{
		int idx = m_ctrlLinks->InsertItem(i++, *iter, GetImageIndex(*iter));
		if (pItem && *iter == pItem)
			m_ctrlLinks->Select(idx);
	}
	m_ctrlLinks->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	UpdateLinksButtons();
}


int CDlgRun::GetImageIndex(wxString const& inLink)
{
	wxBusyCursor wait;
	int img = m_imgEmpty;
	if (0 < inLink.length())
	{
		if (CheckLink(inLink, this))
			img = m_imgOk;
		else
			img = m_imgMissing;
	}
	return img;
}


void CDlgRun::EditLink()
{
	int nItem = m_ctrlLinks->GetFirstSelected();
	if (0 <= nItem)
	{
		wxString name = GetListColumnText(m_ctrlLinks, nItem, 0);
		CDlgSelectURL dlg(name, this);
		if (wxID_OK == dlg.ShowModal())
		{
			wxString newName = dlg.GetName();
			if (name != newName)
			{
				m_Run->RemoveLink(name);
				if (0 < newName.length())
					m_Run->AddLink(newName);
				ListLinkFiles(newName);
			}
		}
	}
}


void CDlgRun::OnScoreDateChanged(wxDateEvent& evt)
{
	m_Run->SetDate(ARBDate(evt.GetDate().GetYear(), evt.GetDate().GetMonth() + 1, evt.GetDate().GetDay()));
	FillDivisions();
}


void CDlgRun::OnSelchangeDivision(wxCommandEvent& evt)
{
	int index = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND != index)
		m_Run->SetDivision(m_ctrlDivisions->GetString(index));
	FillLevels();
}


void CDlgRun::OnSelchangeLevel(wxCommandEvent& evt)
{
	int index = m_ctrlLevels->GetSelection();
	if (wxNOT_FOUND != index)
		m_Run->SetLevel(m_ctrlLevels->GetString(index));
	FillEvents();
}


void CDlgRun::OnSelchangeEvent(wxCommandEvent& evt)
{
	int index = m_ctrlEvents->GetSelection();
	if (wxNOT_FOUND != index)
		m_Run->SetEvent(m_ctrlEvents->GetString(index));
	FillSubNames();
	UpdateControls(true);
	ARBConfigEventPtr pEvent;
	GetEvent(&pEvent);
	SetEventDesc(pEvent);
}


void CDlgRun::OnJudgeNotes(wxCommandEvent& evt)
{
	TransferDataFromWindow();
	CDlgInfoNote dlg(m_pDoc, ARBInfo::eJudgeInfo, m_Judge, this);
	if (wxID_OK == dlg.ShowModal())
	{
		m_Judge = dlg.CurrentSelection();
		FillJudges();
	}
}


void CDlgRun::OnPartnersEdit(wxCommandEvent& evt)
{
	CDlgListCtrl dlg(CDlgListCtrl::ePartners, m_pDoc, m_Run, this);
	if (wxID_OK == dlg.ShowModal())
		SetPartnerText();
}


void CDlgRun::OnOtherpoints(wxCommandEvent& evt)
{
	CDlgListCtrl dlg(m_pDoc->Book().GetConfig(), m_Run, this);
	dlg.ShowModal();
}


void CDlgRun::OnBnClickedTableYps(wxCommandEvent& evt)
{
	bool bSetTable = false;
	if (m_ctrlTable->IsShown())
		bSetTable = m_ctrlTable->GetValue();
	m_Run->GetScoring().SetHasTable(bSetTable);
	SetMinYPS();
	SetYPS();
	SetObstacles();
}


void CDlgRun::OnSelchangeQ(wxCommandEvent& evt)
{
	ARB_Q q;
	int index = m_ctrlQ->GetSelection();
	if (wxNOT_FOUND != index)
		q = m_ctrlQ->GetQ(index);
	m_Run->SetQ(q);
	SetTitlePoints();
}


void CDlgRun::OnSCTChange(wxCommandEvent& evt)
{
	m_ctrlSCT->GetValue().ToDouble(&m_SCT);
	m_Run->GetScoring().SetSCT(m_SCT);
	SetMinYPS();
	SetObstacles();
	SetTotalFaults();
	SetTitlePoints();
}


void CDlgRun::OnClosingTimeChange(wxCommandEvent& evt)
{
	m_ctrlMinYPSClosingTime->GetValue().ToDouble(&m_SCT2);
	m_Run->GetScoring().SetSCT2(m_SCT2);
	SetObstacles();
}


void CDlgRun::OnNumObsChange(wxCommandEvent& evt)
{
	m_Obstacles = static_cast<short>(wxAtoi(m_ctrlObstacles->GetValue()));
	m_Run->GetScoring().SetObstacles(m_Obstacles);
	SetObstacles();
}


void CDlgRun::OnReqOpeningYPSChange(wxCommandEvent& evt)
{
	ARBConfigScoringPtr pScoring;
	if (GetScoring(&pScoring))
	{
		short val = static_cast<short>(wxAtoi(m_ctrlYardsReqOpeningPts->GetValue()));
		switch (pScoring->GetScoringStyle())
		{
		case ARBConfigScoring::eScoreThenTime:
		case ARBConfigScoring::eOCScoreThenTime:
			m_Opening = val;
			m_Run->GetScoring().SetNeedOpenPts(m_Opening);
			SetTitlePoints();
			break;
		default:
			m_Yards = val;
			m_Run->GetScoring().SetYards(m_Yards);
			SetMinYPS();
			SetYPS();
			SetTotalFaults();
			break;
		}
	}
}


void CDlgRun::OnReqClosingChange(wxCommandEvent& evt)
{
	m_Closing = static_cast<short>(wxAtoi(m_ctrlClosing->GetValue()));
	m_Run->GetScoring().SetNeedClosePts(m_Closing);
	SetTitlePoints();
}


void CDlgRun::OnTimeChange(wxCommandEvent& evt)
{
	m_ctrlTime->GetValue().ToDouble(&m_Time);
	m_Run->GetScoring().SetTime(m_Time);
	SetYPS();
	SetObstacles();
	SetTotalFaults();
	SetTitlePoints();
}


void CDlgRun::OnFaultsChange(wxCommandEvent& evt)
{
	m_Faults = static_cast<short>(wxAtoi(m_ctrlFaults->GetValue()));
	m_Run->GetScoring().SetCourseFaults(m_Faults);
	SetTotalFaults();
	SetTitlePoints();
}


void CDlgRun::OnOpenChange(wxCommandEvent& evt)
{
	m_Open = static_cast<short>(wxAtoi(m_ctrlYPSOpeningPts->GetValue()));
	m_Run->GetScoring().SetOpenPts(m_Open);
	SetTitlePoints();
}


void CDlgRun::OnCloseChange(wxCommandEvent& evt)
{
	m_Close = static_cast<short>(wxAtoi(m_ctrlClosingPtsTotalFaults->GetValue()));
	m_Run->GetScoring().SetClosePts(m_Close);
	SetTitlePoints();
}


void CDlgRun::OnPlaceChange(wxCommandEvent& evt)
{
	m_Place = static_cast<short>(wxAtoi(m_ctrlPlace->GetValue()));
	m_Run->SetPlace(m_Place);
	SetTitlePoints();
}


void CDlgRun::OnBonusChange(wxCommandEvent& evt)
{
	m_BonusPts = static_cast<short>(wxAtoi(m_ctrlBonusPts->GetValue()));
	m_Run->GetScoring().SetBonusPts(m_BonusPts);
	SetTitlePoints();
}


void CDlgRun::OnCommentsFaults(wxCommandEvent& evt)
{
	CDlgListCtrl dlg(CDlgListCtrl::eFaults, m_pDoc, m_Run, this);
	if (wxID_OK == dlg.ShowModal())
		SetFaultsText();
}


void CDlgRun::OnRefRunColumnClick(wxListEvent& evt)
{
	s_SortInfo.pThis = this;
	m_sortRefRuns.SetColumnOrder(evt.GetColumn());
	SetRefRunColumnHeaders();
	s_SortInfo.pCols = &m_sortRefRuns;
	m_ctrlRefRuns->SortItems(CompareRefRuns, 0);
	m_sortRefRuns.Save();
}


void CDlgRun::OnRefRunItemSelected(wxListEvent& evt)
{
	UpdateRefRunButtons();
}


void CDlgRun::OnRefRunItemActivated(wxListEvent& evt)
{
	EditRefRun();
}


void CDlgRun::OnRefRunKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	default:
		evt.Skip();
		break;
	case WXK_SPACE:
	case WXK_NUMPAD_SPACE:
		EditRefRun();
		break;
	}
}


void CDlgRun::OnRefRunNew(wxCommandEvent& evt)
{
	ARBDogReferenceRunPtr ref(ARBDogReferenceRun::New());
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		ARBConfigScoringPtr pScoring;
		if (m_pDoc->Book().GetConfig().GetVenues().FindEvent(
			m_pVenue->GetName(),
			m_Run->GetEvent(),
			m_Run->GetDivision(),
			m_Run->GetLevel(),
			m_Run->GetDate(),
			NULL,
			&pScoring))
		{
			wxString nScore;
			switch (pScoring->GetScoringStyle())
			{
			default:
				nScore = wxT("0");
				break;
			case ARBConfigScoring::eFaults100ThenTime:
				nScore = wxT("100");
				break;
			case ARBConfigScoring::eFaults200ThenTime:
				nScore = wxT("200");
				break;
			}
			ref->SetScore(nScore);
		}
	}
	ref->SetQ(ARB_Q::eQ_Q);
	std::set<wxString> heights, names, breeds;
	GetAllHeights(heights);
	GetAllCallNames(names);
	GetAllBreeds(breeds);
	CDlgReferenceRun dlg(m_pDoc, m_Run, heights, names, breeds, ref, this);
	if (wxID_OK == dlg.ShowModal())
	{
		if (m_Run->GetReferenceRuns().AddReferenceRun(ref))
		{
			CDlgDogRefRunDataPtr data(new CDlgDogRefRunData(ref));
			long index = m_ctrlRefRuns->InsertItem(data);
			m_ctrlRefRuns->Select(index);
			// Insert item first to set selection.
			// ListRefRuns then 'fixes' order.
			ListRefRuns();
		}
	}
}


void CDlgRun::OnRefRunAddMe(wxCommandEvent& evt)
{
	if (!IsRefRunMe())
	{
		if (m_Run->GetReferenceRuns().AddReferenceRun(m_pRefRunMe))
		{
			CDlgDogRefRunDataPtr data(new CDlgDogRefRunData(m_pRefRunMe));
			long index = m_ctrlRefRuns->InsertItem(data);
			m_ctrlRefRuns->Select(index);
			// Insert item first to set selection.
			// ListRefRuns then 'fixes' order.
			ListRefRuns();
		}
	}
}


void CDlgRun::OnRefRunEdit(wxCommandEvent& evt)
{
	EditRefRun();
}


void CDlgRun::OnRefRunDelete(wxCommandEvent& evt)
{
	long nItem = m_ctrlRefRuns->GetFirstSelected();
	if (0 <= nItem)
	{
		ARBDogReferenceRunPtr pRef = GetReferenceData(nItem)->GetData();
		if (pRef)
		{
			if (m_Run->GetReferenceRuns().DeleteReferenceRun(pRef))
				m_ctrlRefRuns->DeleteItem(nItem);
			if (m_pRefRunMe && m_pRefRunMe == pRef)
				CreateRefRunMe();
		}
		UpdateRefRunButtons();
	}
}


void CDlgRun::OnCRCDEdit(wxCommandEvent& evt)
{
	m_CRCDDisplay->Display();
}


void CDlgRun::OnCRCDView(wxCommandEvent& evt)
{
	m_CRCDDisplay->ToggleView();
	UpdateCRCDButtons();
}


void CDlgRun::OnCRCDCopy(wxCommandEvent& evt)
{
	m_CRCDDisplay->OnCopy();
	UpdateCRCDButtons();
}


void CDlgRun::OnCRCDImage(wxCommandEvent& evt)
{
	m_CRCDDisplay->OnCRCDImage(m_ctrlIncImage->GetValue());
	UpdateCRCDButtons();
}


void CDlgRun::OnLinksItemSelected(wxListEvent& evt)
{
	UpdateLinksButtons();
}


void CDlgRun::OnLinksItemActivated(wxListEvent& evt)
{
	EditLink();
}


void CDlgRun::OnLinksKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	default:
		evt.Skip();
		break;
	case WXK_SPACE:
	case WXK_NUMPAD_SPACE:
		EditLink();
		break;
	}
}


void CDlgRun::OnLinksNew(wxCommandEvent& evt)
{
	CDlgSelectURL dlg(wxT(""), this);
	if (wxID_OK == dlg.ShowModal())
	{
		wxString newName = dlg.GetName();
		if (0 < newName.length())
		{
			m_Run->AddLink(newName);
			ListLinkFiles(newName);
		}
	}
}


void CDlgRun::OnLinksEdit(wxCommandEvent& evt)
{
	EditLink();
}


void CDlgRun::OnLinksDelete(wxCommandEvent& evt)
{
	long nItem = m_ctrlLinks->GetFirstSelected();
	if (0 <= nItem)
	{
		wxString name = GetListColumnText(m_ctrlLinks, nItem, 0);
		m_Run->RemoveLink(name);
		ListLinkFiles(NULL);
	}
}


void CDlgRun::OnLinksOpen(wxCommandEvent& evt)
{
	int nItem = m_ctrlLinks->GetFirstSelected();
	if (0 <= nItem)
	{
		wxString name = GetListColumnText(m_ctrlLinks, nItem, 0);
		wxLaunchDefaultBrowser(name);
	}
}


void CDlgRun::OnPageChanged(wxNotebookEvent& evt)
{
	if (wxNOT_FOUND != evt.GetOldSelection())
		TransferDataFromWindow();
	if (m_idxRefRunPage == evt.GetSelection())
	{
		// Recreate the 'me' reference run on page change. This will push in
		// any changes made in the scoring page.
		CreateRefRunMe();
		UpdateRefRunButtons();
	}
	evt.Skip();
}


void CDlgRun::OnOk(wxCommandEvent& evt)
{
	if (!Validate() || !TransferDataFromWindow())
		return;

	int index = m_ctrlDivisions->GetSelection();
	if (wxNOT_FOUND == index)
	{
		wxMessageBox(_("IDS_SELECT_DIVISION"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlDivisions->SetFocus();
		return;
	}
	wxString curDiv = m_ctrlDivisions->GetString(index);

	index = m_ctrlLevels->GetSelection();
	if (wxNOT_FOUND == index)
	{
		wxMessageBox(_("IDS_SELECT_LEVEL"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlLevels->SetFocus();
		return;
	}
	CDlgDogLevelData* pLevel = GetLevelData(index);
	assert(pLevel);

	index = m_ctrlEvents->GetSelection();
	if (wxNOT_FOUND == index)
	{
		wxMessageBox(_("IDS_SELECT_EVENT"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlEvents->SetFocus();
		return;
	}
	wxString curEvent = m_ctrlEvents->GetString(index);

	ARBConfigEventPtr pEvent;
	m_pVenue->GetEvents().FindEvent(curEvent, &pEvent);
	if (!pEvent)
	{
		wxMessageBox(_("IDS_BAD_EVENT"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlEvents->SetFocus();
		return;
	}

	if (!pEvent->FindEvent(curDiv, pLevel->m_pLevel->GetName(), m_Run->GetDate()))
	{
		wxMessageBox(_("IDS_BAD_SCORINGMETHOD"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlLevels->SetFocus();
		return;
	}

	if (wxNOT_FOUND == m_ctrlQ->GetSelection())
	{
		wxMessageBox(_("IDS_SELECT_Q"), wxMessageBoxCaptionStr, wxCENTRE | wxICON_STOP);
		m_ctrlQ->SetFocus();
		return;
	}

	//@todo: Add integrity checks - things like snooker score >=37? is Q set?

	m_Run->SetDate(m_Date);
	m_Run->SetSubName(m_SubName);
	m_Run->SetHeight(m_Height);
	m_Run->SetJudge(m_Judge);
	m_Run->SetHandler(m_Handler);
	m_Run->SetConditions(m_Conditions);
	m_Run->SetNote(m_Comments);
	m_Run->SetInClass(m_InClass);
	m_Run->SetDogsQd(m_DogsQd);

	//TODO: Remove debugging code
#ifdef _DEBUG
	{
		ARBConfigEventPtr pEvent;
		m_pVenue->GetEvents().FindEvent(m_Run->GetEvent(), &pEvent);
		assert(NULL != pEvent.get());
		if (!pEvent->HasTable())
			if (m_Run->GetScoring().HasTable())
				wxMessageBox(wxT("Poof!"), wxMessageBoxCaptionStr, wxCENTRE);
	}
#endif
	//End TODO

	*m_pRealRun = *m_Run;
	m_pTrial->SetMultiQs(m_pDoc->Book().GetConfig()); // Note, when adding a new run, this is actually too soon to call - the run isn't in the trial yet
	CAgilityBookOptions::SetLastEnteredDivision(m_Run->GetDivision());
	CAgilityBookOptions::SetLastEnteredLevel(m_Run->GetLevel());
	CAgilityBookOptions::SetLastEnteredHeight(m_Run->GetHeight());
	CAgilityBookOptions::SetLastEnteredJudge(m_Run->GetJudge());
	CAgilityBookOptions::SetLastEnteredHandler(m_Run->GetHandler());
	EndDialog(wxID_OK);
}