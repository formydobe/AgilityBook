/*
 * Copyright � 2002-2004 David Connet. All Rights Reserved.
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
 * @brief implementation of the CDlgRunScore class
 * @author David Connet
 *
 * Revision History
 * @li 2004-02-09 DRC Update YPS when the time is changed.
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
#include "AgilityBook.h"
#include "DlgRunScore.h"

#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "ARBConfig.h"
#include "ARBConfigOtherPoints.h"
#include "ARBConfigVenue.h"
#include "ARBDogClub.h"
#include "ARBDogRun.h"
#include "ARBDogTrial.h"
#include "DlgListCtrl.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

class CDlgRunDataLevel
{
public:
	CDlgRunDataLevel(const ARBConfigLevel* pLevel)
		: m_pLevel(pLevel)
		, m_pSubLevel(NULL)
	{
	}
	CDlgRunDataLevel(const ARBConfigLevel* pLevel, const ARBConfigSubLevel* pSubLevel)
		: m_pLevel(pLevel)
		, m_pSubLevel(pSubLevel)
	{
	}
	const ARBConfigLevel* m_pLevel;
	const ARBConfigSubLevel* m_pSubLevel;
};

/////////////////////////////////////////////////////////////////////////////
// CDlgRunScore dialog

CDlgRunScore::CDlgRunScore(CAgilityBookDoc* pDoc, const ARBConfigVenue* pVenue,
	const ARBDogTrial* pTrial, const ARBDogRun* pRealRun, ARBDogRun* pRun)
	: CPropertyPage(CDlgRunScore::IDD)
	, m_pDoc(pDoc)
	, m_pVenue(pVenue)
	, m_pTrial(pTrial)
	, m_pRealRun(pRealRun)
	, m_Run(pRun)
{
	ASSERT(NULL != m_pTrial);
	const ARBDogClub* pClub = m_pTrial->GetClubs().GetPrimaryClub();
	ASSERT(NULL != pClub);
	//{{AFX_DATA_INIT(CDlgRunScore)
	m_Venue = m_pVenue->GetName().c_str();
	m_Club = pClub->GetName().c_str();
	m_Location = m_pTrial->GetLocation().c_str();
	m_Height = m_Run->GetHeight().c_str();
	m_Conditions = m_Run->GetConditions().c_str();
	m_Faults = 0;
	m_Time = 0.0;
	m_Yards = 0.0;
	m_SCT = 0.0;
	m_Opening = 0;
	m_Closing = 0;
	m_Open = 0;
	m_Close = 0;
	m_Place = m_Run->GetPlace();
	m_InClass = m_Run->GetInClass();
	m_DogsQd = m_Run->GetDogsQd();
	m_Handler = _T("");
	//}}AFX_DATA_INIT
	m_Date = CTime::GetCurrentTime();
	if (m_Run->GetDate().IsValid())
		m_Date = CTime(m_Run->GetDate().GetDate());
	m_Conditions.Replace("\n", "\r\n");
}

CDlgRunScore::~CDlgRunScore()
{
}

void CDlgRunScore::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRunScore)
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_Date);
	DDX_Text(pDX, IDC_VENUE, m_Venue);
	DDX_Text(pDX, IDC_CLUB, m_Club);
	DDX_Text(pDX, IDC_LOCATION, m_Location);
	DDX_Control(pDX, IDC_DIVISION, m_ctrlDivisions);
	DDX_Control(pDX, IDC_LEVEL, m_ctrlLevels);
	DDX_Control(pDX, IDC_EVENT, m_ctrlEvents);
	DDX_Control(pDX, IDC_HEIGHT, m_ctrlHeight);
	DDX_CBString(pDX, IDC_HEIGHT, m_Height);
	DDX_Control(pDX, IDC_JUDGE, m_ctrlJudge);
	DDX_CBString(pDX, IDC_JUDGE, m_Judge);
	DDX_Control(pDX, IDC_CONDITIONS, m_ctrlConditions);
	DDX_Text(pDX, IDC_CONDITIONS, m_Conditions);
	DDX_Control(pDX, IDC_PARTNERS_EDIT, m_ctrlPartnerEdit);
	DDX_Control(pDX, IDC_PARTNER, m_ctrlPartner);
	DDX_Control(pDX, IDC_HANDLER, m_ctrlHandler);
	DDX_CBString(pDX, IDC_HANDLER, m_Handler);
	DDX_Control(pDX, IDC_FAULTS, m_ctrlFaults);
	DDX_Text(pDX, IDC_FAULTS, m_Faults);
	DDX_Control(pDX, IDC_TIME, m_ctrlTime);
	DDX_Text(pDX, IDC_TIME, m_Time);
	DDX_Control(pDX, IDC_YARDS_TEXT, m_ctrlYardsText);
	DDX_Control(pDX, IDC_YARDS, m_ctrlYards);
	DDX_Text(pDX, IDC_YARDS, m_Yards);
	DDX_Control(pDX, IDC_YPS_TEXT, m_ctrlYPSText);
	DDX_Control(pDX, IDC_YPS, m_ctrlYPS);
	DDX_Control(pDX, IDC_SCT_TEXT, m_ctrlSCTText);
	DDX_Control(pDX, IDC_SCT, m_ctrlSCT);
	DDX_Text(pDX, IDC_SCT, m_SCT);
	DDX_Control(pDX, IDC_TOTAL_FAULTS_TEXT, m_ctrlTotalFaultsText);
	DDX_Control(pDX, IDC_TOTAL_FAULTS, m_ctrlTotalFaults);
	DDX_Control(pDX, IDC_OPENING_PTS_TEXT, m_ctrlOpeningText);
	DDX_Control(pDX, IDC_OPENING_PTS, m_ctrlOpening);
	DDX_Text(pDX, IDC_OPENING_PTS, m_Opening);
	DDX_Control(pDX, IDC_CLOSING_PTS_TEXT, m_ctrlClosingText);
	DDX_Control(pDX, IDC_CLOSING_PTS, m_ctrlClosing);
	DDX_Text(pDX, IDC_CLOSING_PTS, m_Closing);
	DDX_Control(pDX, IDC_OPEN_PTS_TEXT, m_ctrlOpenText);
	DDX_Control(pDX, IDC_OPEN_PTS, m_ctrlOpen);
	DDX_Text(pDX, IDC_OPEN_PTS, m_Open);
	DDX_Control(pDX, IDC_CLOSE_PTS_TEXT, m_ctrlCloseText);
	DDX_Control(pDX, IDC_CLOSE_PTS, m_ctrlClose);
	DDX_Text(pDX, IDC_CLOSE_PTS, m_Close);
	DDX_Control(pDX, IDC_PLACE, m_ctrlPlace);
	DDX_Text(pDX, IDC_PLACE, m_Place);
	DDX_Control(pDX, IDC_IN_CLASS, m_ctrlInClass);
	DDX_Text(pDX, IDC_IN_CLASS, m_InClass);
	DDX_Control(pDX, IDC_DOGS_QD, m_ctrlDogsQd);
	DDX_Text(pDX, IDC_DOGS_QD, m_DogsQd);
	DDX_Control(pDX, IDC_Q, m_ctrlQ);
	DDX_Control(pDX, IDC_SCORE, m_ctrlScore);
	DDX_Control(pDX, IDC_TITLE_POINTS_TEXT, m_ctrlTitlePointsText);
	DDX_Control(pDX, IDC_TITLE_POINTS, m_ctrlTitlePoints);
	DDX_Control(pDX, IDC_MACHPTS_TEXT, m_ctrlMachPtsText);
	DDX_Control(pDX, IDC_MACHPTS, m_ctrlMachPts);
	DDX_Control(pDX, IDC_DOUBLE_Q, m_ctrlDoubleQ);
	DDX_Control(pDX, IDC_OTHERPOINTS, m_ctrlOtherPoints);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_Venue.TrimRight();
		m_Venue.TrimLeft();
		m_Club.TrimRight();
		m_Club.TrimLeft();
		m_Location.TrimRight();
		m_Location.TrimLeft();
		m_Height.TrimRight();
		m_Height.TrimLeft();
		m_Judge.TrimRight();
		m_Judge.TrimLeft();
		m_Conditions.TrimRight();
		m_Conditions.TrimLeft();
		m_Handler.TrimRight();
		m_Handler.TrimLeft();

		CString str;
		std::string div, level, event;

		ARBDate date(m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay());

		pDX->PrepareCtrl(m_ctrlDivisions.GetDlgCtrlID());
		int index = m_ctrlDivisions.GetCurSel();
		if (CB_ERR == index)
		{
			AfxMessageBox(IDS_SELECT_DIVISION);
			pDX->Fail();
			return;
		}
		m_ctrlDivisions.GetLBText(index, str);
		if (str.IsEmpty())
		{
			AfxMessageBox(IDS_SELECT_DIVISION);
			pDX->Fail();
			return;
		}
		div = (LPCSTR)str;

		pDX->PrepareCtrl(m_ctrlLevels.GetDlgCtrlID());
		index = m_ctrlLevels.GetCurSel();
		if (CB_ERR == index)
		{
			AfxMessageBox(IDS_SELECT_LEVEL);
			pDX->Fail();
			return;
		}
		m_ctrlLevels.GetLBText(index, str);
		if (str.IsEmpty())
		{
			AfxMessageBox(IDS_SELECT_LEVEL);
			pDX->Fail();
			return;
		}
		level = (LPCSTR)str;
		CDlgRunDataLevel* pLevel = reinterpret_cast<CDlgRunDataLevel*>(m_ctrlLevels.GetItemDataPtr(index));
		ASSERT(pLevel);

		pDX->PrepareCtrl(m_ctrlEvents.GetDlgCtrlID());
		index = m_ctrlEvents.GetCurSel();
		if (CB_ERR == index)
		{
			AfxMessageBox(IDS_SELECT_EVENT);
			pDX->Fail();
			return;
		}
		m_ctrlEvents.GetLBText(index, str);
		if (str.IsEmpty())
		{
			AfxMessageBox(IDS_SELECT_EVENT);
			pDX->Fail();
			return;
		}
		event = (LPCSTR)str;

		pDX->PrepareCtrl(m_ctrlJudge.GetDlgCtrlID());
		if (m_Judge.IsEmpty())
		{
			AfxMessageBox(IDS_SELECT_JUDGE);
			pDX->Fail();
			return;
		}

		pDX->PrepareCtrl(m_ctrlQ.GetDlgCtrlID());
		index = m_ctrlQ.GetCurSel();
		if (CB_ERR == index)
		{
			AfxMessageBox(IDS_SELECT_Q);
			pDX->Fail();
			return;
		}
		ARB_Q q = ARB_Q::GetValidType(static_cast<int>(m_ctrlQ.GetItemData(index)));

		pDX->PrepareCtrl(m_ctrlEvents.GetDlgCtrlID());
		const ARBConfigEvent* pEvent = m_pVenue->GetEvents().FindEvent(event);
		if (!pEvent)
		{
			AfxMessageBox(IDS_BAD_EVENT, MB_ICONSTOP);
			pDX->Fail();
			return;
		}

		pDX->PrepareCtrl(m_ctrlLevels.GetDlgCtrlID());
		const ARBConfigScoring* pScoring = pEvent->FindEvent(div, pLevel->m_pLevel->GetName(), date);
		if (!pScoring)
		{
			AfxMessageBox(IDS_BAD_SCORINGMETHOD, MB_ICONSTOP);
			pDX->Fail();
			return;
		}

		//@todo: Add integrity checks - things like snooker score >=37? is Q set?

		m_Run->SetDate(date);
		m_Run->SetDivision(div);
		m_Run->SetLevel(level);
		m_Run->SetEvent(event);
		m_Run->SetHeight((LPCSTR)m_Height);
		m_Run->SetJudge((LPCSTR)m_Judge);
		CString tmp(m_Conditions);
		tmp.Replace("\r\n", "\n");
		m_Run->SetConditions((LPCSTR)tmp);
		switch (pScoring->GetScoringStyle())
		{
		default:
			ASSERT(0);
			break;
		case ARBConfigScoring::eFaultsThenTime:
		case ARBConfigScoring::eFaults100ThenTime:
		case ARBConfigScoring::eFaults200ThenTime:
		case ARBConfigScoring::eTimePlusFaults:
			m_Run->GetScoring().SetCourseFaults(m_Faults);
			m_Run->GetScoring().SetTime(m_Time);
			m_Run->GetScoring().SetYards(m_Yards);
			m_Run->GetScoring().SetSCT(m_SCT);
			break;
		case ARBConfigScoring::eOCScoreThenTime:
			m_Run->GetScoring().SetCourseFaults(m_Faults);
			m_Run->GetScoring().SetTime(m_Time);
			m_Run->GetScoring().SetNeedOpenPts(m_Opening);
			m_Run->GetScoring().SetNeedClosePts(m_Closing);
			m_Run->GetScoring().SetOpenPts(m_Open);
			m_Run->GetScoring().SetClosePts(m_Close);
			break;
		case ARBConfigScoring::eScoreThenTime:
			m_Run->GetScoring().SetCourseFaults(m_Faults);
			m_Run->GetScoring().SetTime(m_Time);
			m_Run->GetScoring().SetNeedOpenPts(m_Opening);
			m_Run->GetScoring().SetOpenPts(m_Open);
			break;
		}
		m_Run->SetHandler((LPCSTR)m_Handler);
		m_Run->SetPlace(m_Place);
		m_Run->SetInClass(m_InClass);
		m_Run->SetDogsQd(m_DogsQd);
		m_Run->SetQ(q);
	}
}

BEGIN_MESSAGE_MAP(CDlgRunScore, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgRunScore)
	ON_WM_DESTROY()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE, OnDatetimechangeDate)
	ON_CBN_SELCHANGE(IDC_DIVISION, OnSelchangeDivision)
	ON_CBN_SELCHANGE(IDC_LEVEL, OnSelchangeLevel)
	ON_CBN_SELCHANGE(IDC_EVENT, OnSelchangeEvent)
	ON_BN_CLICKED(IDC_PARTNERS_EDIT, OnPartnersEdit)
	ON_BN_CLICKED(IDC_OTHERPOINTS, OnOtherpoints)
	ON_EN_KILLFOCUS(IDC_FAULTS, OnKillfocusFaults)
	ON_EN_KILLFOCUS(IDC_TIME, OnKillfocusTime)
	ON_EN_KILLFOCUS(IDC_YARDS, OnKillfocusYards)
	ON_EN_KILLFOCUS(IDC_SCT, OnKillfocusSct)
	ON_EN_KILLFOCUS(IDC_OPENING_PTS, OnKillfocusOpening)
	ON_EN_KILLFOCUS(IDC_CLOSING_PTS, OnKillfocusClosing)
	ON_EN_KILLFOCUS(IDC_OPEN_PTS, OnKillfocusOpen)
	ON_EN_KILLFOCUS(IDC_CLOSE_PTS, OnKillfocusClose)
	ON_EN_KILLFOCUS(IDC_PLACE, OnKillfocusPlace)
	ON_CBN_SELCHANGE(IDC_Q, OnSelchangeQ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// Helper functions to avoid UpdateData. Used during KillFocus events.
bool CDlgRunScore::GetText(CEdit* pEdit, short& val) const
{
	if (!pEdit)
		return false;
	CString str;
	pEdit->GetWindowText(str);
	val = static_cast<short>(atoi((LPCTSTR)str));
	return true;
}
bool CDlgRunScore::GetText(CEdit* pEdit, double& val) const
{
	if (!pEdit)
		return false;
	CString str;
	pEdit->GetWindowText(str);
	val = strtod((LPCTSTR)str, NULL);
	return true;
}

const ARBConfigEvent* CDlgRunScore::GetEvent() const
{
	CString str;
	int index = m_ctrlEvents.GetCurSel();
	if (CB_ERR == index)
		return NULL;
	m_ctrlEvents.GetLBText(index, str);
	if (str.IsEmpty())
		return NULL;
	std::string event = (LPCSTR)str;
	return m_pVenue->GetEvents().FindEvent(event);
}

void CDlgRunScore::ClearLevels()
{
	for (int index = m_ctrlLevels.GetCount() - 1; 0 <= index; --index)
	{
		CDlgRunDataLevel* pData = reinterpret_cast<CDlgRunDataLevel*>(m_ctrlLevels.GetItemDataPtr(index));
		delete pData;
		m_ctrlLevels.SetItemDataPtr(index, NULL);
	}
	m_ctrlLevels.ResetContent();
}

void CDlgRunScore::FillLevels()
{
	CString str;
	std::string level;
	int index = m_ctrlLevels.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlLevels.GetLBText(index, str);
		level = (LPCSTR)str;
	}
	if (str.IsEmpty())
		level = m_Run->GetLevel();
	ClearLevels();
	index = m_ctrlDivisions.GetCurSel();
	if (CB_ERR != index)
	{
		const ARBConfigDivision* pDiv = reinterpret_cast<const ARBConfigDivision*>(m_ctrlDivisions.GetItemDataPtr(index));
		for (ARBConfigLevelList::const_iterator iter = pDiv->GetLevels().begin();
			iter != pDiv->GetLevels().end();
			++iter)
		{
			const ARBConfigLevel* pLevel = (*iter);
			if (0 < pLevel->GetSubLevels().size())
			{
				for (ARBConfigSubLevelList::const_iterator iterSub = pLevel->GetSubLevels().begin();
					iterSub != pLevel->GetSubLevels().end();
					++iterSub)
				{
					const ARBConfigSubLevel* pSubLevel = (*iterSub);
					CDlgRunDataLevel* pData = new CDlgRunDataLevel(pLevel, pSubLevel);
					int idx = m_ctrlLevels.AddString(pSubLevel->GetName().c_str());
					m_ctrlLevels.SetItemDataPtr(idx, pData);
					if (level == pSubLevel->GetName())
						m_ctrlLevels.SetCurSel(idx);
				}
			}
			else
			{
				CDlgRunDataLevel* pData = new CDlgRunDataLevel(pLevel);
				int idx = m_ctrlLevels.AddString(pLevel->GetName().c_str());
				m_ctrlLevels.SetItemDataPtr(idx, pData);
				if (level == pLevel->GetName())
					m_ctrlLevels.SetCurSel(idx);
			}
		}
		if (CB_ERR == m_ctrlLevels.GetCurSel())
		{
			CString last = CAgilityBookOptions::GetLastEnteredLevel();
			if (0 < last.GetLength())
			{
				int idx = m_ctrlLevels.FindStringExact(-1, last);
				if (0 <= idx)
					m_ctrlLevels.SetCurSel(idx);
			}
		}
	}
	FillEvents();
	SetDoubleQ();
	SetTitlePoints();
}

void CDlgRunScore::FillEvents()
{
	CString str;
	std::string event;
	int index = m_ctrlEvents.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlEvents.GetLBText(index, str);
		event = (LPCSTR)str;
	}
	if (str.IsEmpty())
		event = m_Run->GetEvent();
	m_ctrlEvents.ResetContent();
	int idxDiv = m_ctrlDivisions.GetCurSel();
	if (CB_ERR != idxDiv)
	{
		const ARBConfigDivision* pDiv = reinterpret_cast<const ARBConfigDivision*>(m_ctrlDivisions.GetItemDataPtr(idxDiv));
		int idxLevel = m_ctrlLevels.GetCurSel();
		if (CB_ERR != idxLevel)
		{
			CDlgRunDataLevel* pData = reinterpret_cast<CDlgRunDataLevel*>(m_ctrlLevels.GetItemDataPtr(idxLevel));
			for (ARBConfigEventList::const_iterator iter = m_pVenue->GetEvents().begin();
				iter != m_pVenue->GetEvents().end();
				++iter)
			{
				const ARBConfigEvent* pEvent = (*iter);
				if (pEvent->FindEvent(pDiv->GetName(), pData->m_pLevel->GetName(), m_Run->GetDate()))
				{
					int idx = m_ctrlEvents.AddString(pEvent->GetName().c_str());
					if (event == pEvent->GetName())
						m_ctrlEvents.SetCurSel(idx);
				}
			}
		}
	}
	UpdateControls();
}

void CDlgRunScore::SetPartnerText()
{
	CString partners;
	const ARBConfigEvent* pEvent = GetEvent();
	if (pEvent && pEvent->HasPartner())
	{
		for (ARBDogRunPartnerList::const_iterator iter = m_Run->GetPartners().begin(); iter != m_Run->GetPartners().end(); ++iter)
		{
			if (!partners.IsEmpty())
				partners += ", ";
			partners += (*iter)->GetHandler().c_str();
			partners += "/";
			partners += (*iter)->GetDog().c_str();
		}
	}
	m_ctrlPartner.SetWindowText(partners);
}

void CDlgRunScore::SetYPS()
{
	CString yps;
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType()
	&& 0 < m_Yards && 0.0 < m_Time)
	{
		yps.Format("%.3f", m_Yards / m_Time);
	}
	m_ctrlYPS.SetWindowText(yps);
}

void CDlgRunScore::SetTotalFaults()
{
	CString total;
	if (ARBDogRunScoring::eTypeByTime == m_Run->GetScoring().GetType())
	{
		double faults = m_Run->GetScoring().GetCourseFaults() + m_Run->GetScoring().GetTimeFaults();
		total.Format("%.3f", faults);
	}
	m_ctrlTotalFaults.SetWindowText(total);
}

void CDlgRunScore::FillQ(bool bHasSuperQ)
{
	m_ctrlQ.ResetContent();
	int nQs = ARB_Q::GetNumValidTypes();
	for (int index = 0; index < nQs; ++index)
	{
		ARB_Q q = ARB_Q::GetValidType(index);
		if (ARB_Q::eQ_SuperQ == q && !bHasSuperQ)
			continue;
		int idx = m_ctrlQ.AddString(q.str().c_str());
		m_ctrlQ.SetItemData(idx, index);
		if (m_Run->GetQ() == q)
			m_ctrlQ.SetCurSel(idx);
	}
}

void CDlgRunScore::SetDoubleQ()
{
	std::string div, level;
	CString str;
	int index = m_ctrlDivisions.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlDivisions.GetLBText(index, str);
		if (!str.IsEmpty())
			div = (LPCSTR)str;
	}
	index = m_ctrlLevels.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlLevels.GetLBText(index, str);
		if (!str.IsEmpty())
			level = (LPCSTR)str;
	}
	int nQs = 0;
	if (0 < div.length() && 0 < level.length())
	{
		ARBDate date(m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay());
		// Get all Qs for the trial date.
		if (m_pTrial->HasQQ(date, m_pDoc->GetConfig(), div, level))
			nQs = 2;
		// Subtract us.
		if (m_pRealRun->GetQ().Qualified()
		&& m_pRealRun->GetDivision() == div
		&& m_pRealRun->GetLevel() == level)
			--nQs;
		// Now re-add us based on UI.
		if (m_Run->GetQ().Qualified())
			++nQs;
	}
	// And set it.
	if (2 == nQs)
		m_ctrlDoubleQ.SetCheck(1);
	else
		m_ctrlDoubleQ.SetCheck(0);
}

void CDlgRunScore::SetTitlePoints()
{
	int index = m_ctrlQ.GetCurSel();
	if (CB_ERR == index)
	{
		GotoDlgCtrl(&m_ctrlQ);
		return;
	}
	ARB_Q q = ARB_Q::GetValidType(static_cast<int>(m_ctrlQ.GetItemData(index)));

	CString strMach("0");
	CString strTitle("0");
	CString strScore("");
	const ARBConfigEvent* pEvent = GetEvent();
	std::string div, level;
	CString str;
	index = m_ctrlDivisions.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlDivisions.GetLBText(index, str);
		if (!str.IsEmpty())
			div = (LPCSTR)str;
	}
	index = m_ctrlLevels.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlLevels.GetLBText(index, str);
		if (!str.IsEmpty())
			level = (LPCSTR)str;
	}
	CDlgRunDataLevel* pLevel = reinterpret_cast<CDlgRunDataLevel*>(m_ctrlLevels.GetItemDataPtr(index));
	ASSERT(pLevel);
	const ARBConfigScoring* pScoring = NULL;
	if (0 < div.length() && 0 < level.length() && pEvent)
		pScoring = pEvent->FindEvent(div, pLevel->m_pLevel->GetName(), m_Run->GetDate());
	if (pScoring)
	{
		// 8/17/03: Only compute title points on Q runs.
		if (q.Qualified())
		{
			if (pScoring->HasMachPts())
			{
				strMach.Format("%hd", m_Run->GetMachPoints(pScoring));
			}
			strTitle.Format("%hd", m_Run->GetTitlePoints(pScoring));
		}
		// 8/17/03: Only compute score on Q and NQ runs.
		if (q.Qualified()
		|| ARB_Q::eQ_NQ == q)
			strScore = m_Run->GetScore(pScoring).str().c_str();
	}
	// Doesn't matter if they're hidden,..
	m_ctrlMachPts.SetWindowText(strMach);
	m_ctrlTitlePoints.SetWindowText(strTitle);
	m_ctrlScore.SetWindowText(strScore);
}

void CDlgRunScore::UpdateControls()
{
	const ARBConfigEvent* pEvent = GetEvent();
	std::string div, level;
	CString str;
	int index = m_ctrlDivisions.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlDivisions.GetLBText(index, str);
		if (!str.IsEmpty())
			div = (LPCSTR)str;
	}
	index = m_ctrlLevels.GetCurSel();
	if (CB_ERR != index)
	{
		m_ctrlLevels.GetLBText(index, str);
		if (!str.IsEmpty())
			level = (LPCSTR)str;
	}
	CDlgRunDataLevel* pLevel = reinterpret_cast<CDlgRunDataLevel*>(m_ctrlLevels.GetItemDataPtr(index));
	ASSERT(pLevel);
	const ARBConfigScoring* pScoring = NULL;
	if (0 < div.length() && 0 < level.length() && pEvent)
		pScoring = pEvent->FindEvent(div, pLevel->m_pLevel->GetName(), m_Run->GetDate());
	if (!pScoring)
	{
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeUnknown, false);
		m_ctrlHeight.EnableWindow(FALSE);
		m_ctrlJudge.EnableWindow(FALSE);
		m_ctrlConditions.EnableWindow(FALSE);
		m_ctrlPartnerEdit.ShowWindow(SW_HIDE);
		m_ctrlPartner.ShowWindow(SW_HIDE);
		m_ctrlHandler.EnableWindow(FALSE);
		m_ctrlTime.EnableWindow(FALSE);
		m_ctrlFaults.EnableWindow(FALSE);
		m_ctrlYards.EnableWindow(FALSE);
		m_ctrlSCT.EnableWindow(FALSE);
		m_ctrlOpening.EnableWindow(FALSE);
		m_ctrlClosing.EnableWindow(FALSE);
		m_ctrlOpen.EnableWindow(FALSE);
		m_ctrlClose.EnableWindow(FALSE);
		m_ctrlPlace.EnableWindow(FALSE);
		m_ctrlInClass.EnableWindow(FALSE);
		m_ctrlDogsQd.EnableWindow(FALSE);
		m_ctrlQ.EnableWindow(FALSE);
		m_ctrlOtherPoints.EnableWindow(FALSE);
		m_ctrlTitlePointsText.ShowWindow(SW_HIDE);
		m_ctrlTitlePoints.ShowWindow(SW_HIDE);
		m_ctrlMachPtsText.ShowWindow(SW_HIDE);
		m_ctrlMachPts.ShowWindow(SW_HIDE);
		m_ctrlDoubleQ.ShowWindow(SW_HIDE);
		return;
	}

	m_ctrlHeight.GetWindowText(str);
	if (str.IsEmpty())
	{
		CString last = CAgilityBookOptions::GetLastEnteredHeight();
		if (!last.IsEmpty())
			m_ctrlHeight.SetWindowText(last);
	}
	m_ctrlHeight.EnableWindow(TRUE);

	m_ctrlJudge.GetWindowText(str);
	if (str.IsEmpty())
	{
		CString last = CAgilityBookOptions::GetLastEnteredJudge();
		if (!last.IsEmpty())
			m_ctrlJudge.SetWindowText(last);
	}
	m_ctrlJudge.EnableWindow(TRUE);

	m_ctrlConditions.EnableWindow(TRUE);

	if (pEvent->HasPartner())
	{
		m_ctrlPartnerEdit.ShowWindow(SW_SHOW);
		m_ctrlPartner.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ctrlPartnerEdit.ShowWindow(SW_HIDE);
		m_ctrlPartner.ShowWindow(SW_HIDE);
	}

	m_ctrlHandler.GetWindowText(str);
	if (str.IsEmpty())
	{
		CString last = CAgilityBookOptions::GetLastEnteredHandler();
		if (!last.IsEmpty())
			m_ctrlHandler.SetWindowText(last);
	}
	m_ctrlHandler.EnableWindow(TRUE);

	switch (pScoring->GetScoringStyle())
	{
	default:
		ASSERT(0);
		break;
	case ARBConfigScoring::eFaultsThenTime:
	case ARBConfigScoring::eFaults100ThenTime:
	case ARBConfigScoring::eFaults200ThenTime:
	case ARBConfigScoring::eTimePlusFaults:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByTime, pScoring->DropFractions());
		m_ctrlTime.EnableWindow(TRUE);
		m_ctrlFaults.EnableWindow(TRUE);
		m_ctrlYards.EnableWindow(TRUE);
		m_ctrlSCT.EnableWindow(TRUE);
		m_ctrlOpening.EnableWindow(FALSE);
		m_ctrlClosing.EnableWindow(FALSE);
		m_ctrlOpen.EnableWindow(FALSE);
		m_ctrlClose.EnableWindow(FALSE);
		break;
	case ARBConfigScoring::eOCScoreThenTime:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByOpenClose, pScoring->DropFractions());
		m_Opening = pScoring->GetRequiredOpeningPoints();
		m_Closing = pScoring->GetRequiredClosingPoints();
		// Do not push these back into the run.
		// Otherwise this will overwrite valid values during OnInit.
		m_ctrlTime.EnableWindow(TRUE);
		m_ctrlFaults.EnableWindow(TRUE);
		m_ctrlYards.EnableWindow(FALSE);
		m_ctrlSCT.EnableWindow(FALSE);
		m_ctrlOpeningText.SetWindowText(m_strOpening[0]);
		m_ctrlOpening.EnableWindow(TRUE);
		m_ctrlClosingText.ShowWindow(SW_SHOW);
		m_ctrlClosing.ShowWindow(SW_SHOW);
		m_ctrlClosing.EnableWindow(TRUE);
		m_ctrlOpenText.SetWindowText(m_strOpen[0]);
		m_ctrlOpen.EnableWindow(TRUE);
		m_ctrlCloseText.ShowWindow(SW_SHOW);
		m_ctrlClose.ShowWindow(SW_SHOW);
		m_ctrlClose.EnableWindow(TRUE);
		// I don't want to call UpdateData here. It could cause a loss of data.
		str.Format("%hd", m_Opening);
		m_ctrlOpening.SetWindowText(str);
		str.Format("%hd", m_Closing);
		m_ctrlClosing.SetWindowText(str);
		break;
	case ARBConfigScoring::eScoreThenTime:
		m_Run->GetScoring().SetType(ARBDogRunScoring::eTypeByPoints, pScoring->DropFractions());
		m_Opening = pScoring->GetRequiredOpeningPoints();
		// Do not push this back into the run.
		// Otherwise this will overwrite valid values during OnInit.
		m_ctrlTime.EnableWindow(TRUE);
		m_ctrlFaults.EnableWindow(TRUE);
		m_ctrlYards.EnableWindow(FALSE);
		m_ctrlSCT.EnableWindow(FALSE);
		m_ctrlOpeningText.SetWindowText(m_strOpening[1]);
		m_ctrlOpening.EnableWindow(TRUE);
		m_ctrlClosingText.ShowWindow(SW_HIDE);
		m_ctrlClosing.ShowWindow(SW_HIDE);
		m_ctrlOpenText.SetWindowText(m_strOpen[1]);
		m_ctrlOpen.EnableWindow(TRUE);
		m_ctrlCloseText.ShowWindow(SW_HIDE);
		m_ctrlClose.ShowWindow(SW_HIDE);
		// I don't want to call UpdateData here. It could cause a loss of data.
		str.Format("%hd", m_Opening);
		m_ctrlOpening.SetWindowText(str);
		break;
	}

	m_ctrlPlace.EnableWindow(TRUE);
	m_ctrlInClass.EnableWindow(TRUE);
	if (0 < pScoring->GetTitlePoints().size())
	{
		m_ctrlQ.EnableWindow(TRUE);
		m_ctrlDogsQd.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlQ.EnableWindow(FALSE);
		m_ctrlDogsQd.EnableWindow(FALSE);
	}
	FillQ(pScoring->HasSuperQ());
	if (0 < pScoring->GetTitlePoints().size())
	{
		m_ctrlTitlePointsText.ShowWindow(SW_SHOW);
		m_ctrlTitlePoints.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ctrlTitlePointsText.ShowWindow(SW_HIDE);
		m_ctrlTitlePoints.ShowWindow(SW_HIDE);
	}
	if (pScoring->HasMachPts())
	{
		m_ctrlMachPtsText.ShowWindow(SW_SHOW);
		m_ctrlMachPts.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ctrlMachPtsText.ShowWindow(SW_HIDE);
		m_ctrlMachPts.ShowWindow(SW_HIDE);
	}
	if (pScoring->HasDoubleQ())
	{
		m_ctrlDoubleQ.ShowWindow(SW_SHOW);
		SetDoubleQ();
	}
	else
		m_ctrlDoubleQ.ShowWindow(SW_HIDE);
	SetTitlePoints();
}

/////////////////////////////////////////////////////////////////////////////
// CDlgRunScore message handlers

BOOL CDlgRunScore::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_ctrlOpeningText.GetWindowText(m_strOpening[0]);
	m_ctrlOpenText.GetWindowText(m_strOpen[0]);
	m_strOpening[1].LoadString(IDS_SCORING_REQUIRED_POINTS);
	m_strOpen[1].LoadString(IDS_SCORING_POINTS);

	int index;
	for (ARBConfigDivisionList::const_iterator iterDiv = m_pVenue->GetDivisions().begin();
		iterDiv != m_pVenue->GetDivisions().end();
		++iterDiv)
	{
		ARBConfigDivision* pDiv = (*iterDiv);
		index = m_ctrlDivisions.AddString(pDiv->GetName().c_str());
		m_ctrlDivisions.SetItemDataPtr(index, pDiv);
		if (m_Run->GetDivision() == pDiv->GetName())
			m_ctrlDivisions.SetCurSel(index);
	}
	if (CB_ERR == m_ctrlDivisions.GetCurSel())
	{
		CString last = CAgilityBookOptions::GetLastEnteredDivision();
		if (0 < last.GetLength())
		{
			index = m_ctrlDivisions.FindStringExact(-1, last);
			if (0 <= index)
				m_ctrlDivisions.SetCurSel(index);
		}
	}

	FillLevels(); // This will call	UpdateControls();

	set<string> names;
	m_pDoc->GetAllHeights(names);
	set<string>::const_iterator iter;
	for (iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlHeight.AddString((*iter).c_str());
	}
	m_Judge = m_Run->GetJudge().c_str();

	m_pDoc->GetAllJudges(names);
	for (iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlJudge.AddString((*iter).c_str());
	}
	m_Judge = m_Run->GetJudge().c_str();

	m_pDoc->GetAllHandlers(names);
	for (iter = names.begin(); iter != names.end(); ++iter)
	{
		m_ctrlHandler.AddString((*iter).c_str());
	}
	m_Handler = m_Run->GetHandler().c_str();

	SetPartnerText();
	switch (m_Run->GetScoring().GetType())
	{
	default:
		break;
	case ARBDogRunScoring::eTypeByTime:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Yards = m_Run->GetScoring().GetYards();
		m_SCT = m_Run->GetScoring().GetSCT();
		SetYPS();
		SetTotalFaults();
		break;
	case ARBDogRunScoring::eTypeByOpenClose:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Opening = m_Run->GetScoring().GetNeedOpenPts();
		m_Closing = m_Run->GetScoring().GetNeedClosePts();
		m_Open = m_Run->GetScoring().GetOpenPts();
		m_Close = m_Run->GetScoring().GetClosePts();
		break;
	case ARBDogRunScoring::eTypeByPoints:
		m_Faults = m_Run->GetScoring().GetCourseFaults();
		m_Time = m_Run->GetScoring().GetTime();
		m_Opening = m_Run->GetScoring().GetNeedOpenPts();
		m_Open = m_Run->GetScoring().GetOpenPts();
		break;
	}

	if (0 < m_pDoc->GetConfig().GetOtherPoints().size())
		m_ctrlOtherPoints.EnableWindow(TRUE);
	else
		m_ctrlOtherPoints.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRunScore::OnDestroy() 
{
	ClearLevels();
	CPropertyPage::OnDestroy();
}

void CDlgRunScore::OnDatetimechangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Do not call UpdateData, it causes too much validation
	UpdateControls();
	*pResult = 0;
}

void CDlgRunScore::OnSelchangeDivision()
{
	FillLevels();
}

void CDlgRunScore::OnSelchangeLevel()
{
	FillEvents();
}

void CDlgRunScore::OnSelchangeEvent()
{
	UpdateControls();
}

void CDlgRunScore::OnPartnersEdit() 
{
	CDlgListCtrl dlg(m_Run, this);
	if (IDOK == dlg.DoModal())
		SetPartnerText();
}

void CDlgRunScore::OnOtherpoints() 
{
	CDlgListCtrl dlg(&m_pDoc->GetConfig(), m_Run, this);
	dlg.DoModal();
}

// In all of the killfocus routines, do not call updatedata.
// Clicking on the cancel button causes killfocus, which if we're
// canceling, we obviously don't want to validate!

void CDlgRunScore::OnKillfocusFaults()
{
	GetText(&m_ctrlFaults, m_Faults);
	m_Run->GetScoring().SetCourseFaults(m_Faults);
	SetTotalFaults();
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusTime() 
{
	GetText(&m_ctrlTime, m_Time);
	m_Run->GetScoring().SetTime(m_Time);
	SetYPS();
	SetTotalFaults();
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusYards()
{
	GetText(&m_ctrlYards, m_Yards);
	m_Run->GetScoring().SetYards(m_Yards);
	SetYPS();
	SetTotalFaults();
}

void CDlgRunScore::OnKillfocusSct() 
{
	GetText(&m_ctrlSCT, m_SCT);
	m_Run->GetScoring().SetSCT(m_SCT);
	SetTotalFaults();
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusOpening()
{
	GetText(&m_ctrlOpening, m_Opening);
	m_Run->GetScoring().SetNeedOpenPts(m_Opening);
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusClosing()
{
	GetText(&m_ctrlClosing, m_Closing);
	m_Run->GetScoring().SetNeedClosePts(m_Closing);
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusOpen()
{
	GetText(&m_ctrlOpen, m_Open);
	m_Run->GetScoring().SetOpenPts(m_Open);
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusClose()
{
	GetText(&m_ctrlClose, m_Close);
	m_Run->GetScoring().SetClosePts(m_Close);
	SetTitlePoints();
}

void CDlgRunScore::OnKillfocusPlace()
{
	GetText(&m_ctrlPlace, m_Place);
	m_Run->SetPlace(m_Place);
	SetTitlePoints();
}

void CDlgRunScore::OnSelchangeQ()
{
	ARB_Q q;
	int index = m_ctrlQ.GetCurSel();
	if (CB_ERR != index)
		q = ARB_Q::GetValidType(static_cast<int>(m_ctrlQ.GetItemData(index)));
	m_Run->SetQ(q);
	SetDoubleQ();
	SetTitlePoints();
}
