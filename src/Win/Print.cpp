/*
 * Copyright � 2007-2009 David Connet. All Rights Reserved.
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
 * @brief Support for direct printing of runs.
 * @author David Connet
 *
 * Revision History
 * @li 2009-01-21 DRC Leave gray separator line in when text in run is empty.
 * @li 2007-07-13 DRC Created
 */

#include "stdafx.h"
#include "AgilityBook.h"

#include "ARBConfig.h"
#include "ARBDog.h"
#include "ARBDogRun.h"
#include "ARBDogRunOtherPoints.h"
#include "ARBDogTrial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// Added '2' to avoid dependencies on class defined (locally) in viewprnt.cpp.
// In fact, I suspect if I compiled using MFC DLLs, it wouldn't work otherwise.
// Since I'm statically linking, the OnCancel/... actually resolve when linking

static bool s_bUserAbort = false;


class CPrintingDialog2 : public CDialog
{
public:
	//{{AFX_DATA(CPrintingDialog2)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA
	CPrintingDialog2::CPrintingDialog2(CWnd* pParent)
	{
		Create(CPrintingDialog2::IDD, pParent);      // modeless !
		s_bUserAbort = false;
	}
	virtual ~CPrintingDialog2() { }

	virtual BOOL OnInitDialog();
	virtual void OnCancel();
};


BOOL CPrintingDialog2::OnInitDialog()
{
	SetWindowText(AfxGetAppName());
	CenterWindow();
	return CDialog::OnInitDialog();
}


void CPrintingDialog2::OnCancel()
{
	s_bUserAbort = true;  // flag that user aborted print
	CDialog::OnCancel();
}


BOOL CALLBACK AbortProc(HDC, int)
{
	MSG msg;
	while (!s_bUserAbort && ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
#if _MSC_VER >= 1300
		if (!AfxPumpMessage())
#else
		if (!theApp.PumpMessage())
#endif
			return FALSE;   // terminate if WM_QUIT received
	}
	return !s_bUserAbort;
}

/////////////////////////////////////////////////////////////////////////////

class CPrintRuns
{
public:
	CPrintRuns(
			ARBConfig const* inConfig,
			ARBDogPtr inDog,
			std::vector<RunInfo> const& inRuns)
		: m_config(inConfig)
		, m_dog(inDog)
		, m_runs(inRuns)
		, m_cur(0)
	{
	}

	BOOL DoPreparePrinting(CPrintInfo* pInfo);
	BOOL OnPreparePrinting(CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrint(CDC* pDC, CPrintInfo* pInfo);

private:
	tstring GetFieldText(ARBDogTrialPtr trial, ARBDogRunPtr run, int code);
	void PrintPage(UINT nCurPage, CDC* pDC, CRect inRect);

	ARBConfig const* m_config;
	ARBDogPtr m_dog;
	std::vector<RunInfo> const& m_runs;
	size_t m_cur;
};


BOOL CPrintRuns::DoPreparePrinting(CPrintInfo* pInfo)
{
	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;
	pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
	pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	if (theApp.DoPrintDialog(pInfo->m_pPD) != IDOK)
		return FALSE;
	assert(pInfo->m_pPD != NULL);
	assert(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;
	pInfo->m_nNumPreviewPages = theApp.m_nNumPreviewPages;
	pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC);
	return TRUE;
}


BOOL CPrintRuns::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMinPage(1);
	if (0 == m_runs.size())
		pInfo->SetMaxPage(2);
	// Don't bother computing max page - we won't know
	// the paper orientation until DoPrepare returns.
	// And that's where we need it!
	return DoPreparePrinting(pInfo);
}


void CPrintRuns::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
}


void CPrintRuns::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	UINT nPages = static_cast<UINT>(m_runs.size());
	if (nPages > 0)
	{
		// Correct the max page based on paper orientation.
		UINT nRuns = static_cast<UINT>(m_runs.size());
		nPages = 1;
		// pInfo->m_rectDraw is not set yet.
		CRect r(0, 0,
			pDC->GetDeviceCaps(HORZRES),
			pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&r);
		if (abs(r.Width()) > abs(r.Height()))
		{
			nPages = nRuns / 4;
			if (0 != nRuns % 4)
				++nPages;
		}
		else
		{
			nPages = nRuns / 2;
			if (0 != nRuns % 2)
				++nPages;
		}
		if (nPages < pInfo->GetMaxPage())
			pInfo->SetMaxPage(nPages);
	}
}


void CPrintRuns::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}


#define CODE_DOG		0
#define CODE_DATE		1
#define CODE_VENUE		2
#define CODE_CLUB		3
#define CODE_DIV		4
#define CODE_LOCATION	5
#define CODE_HEIGHT		6
#define CODE_JUDGE		7
#define CODE_HANDLER	8
#define CODE_CONDITIONS	9
#define CODE_Q			10
#define CODE_SCT		11
#define CODE_YARDS		12
#define CODE_OPEN		13
#define CODE_TIME		14
#define CODE_OBSTACLES	15
#define CODE_FAULTS		16
#define CODE_SCORE		17
#define CODE_PLACE		18
#define CODE_INCLASS	19
#define CODE_QD			20
#define CODE_OTHER		21
#define CODE_COMMENTS	22
#define CODE_REFPLACE1	23
#define CODE_REFQ1		24
#define CODE_REFTIME1	25
#define CODE_REFSCORE1	26
#define CODE_REFHT1		27
#define CODE_REF1		28
#define CODE_REFPLACE2	29
#define CODE_REFQ2		30
#define CODE_REFTIME2	31
#define CODE_REFSCORE2	32
#define CODE_REFHT2		33
#define CODE_REF2		34
#define CODE_REFPLACE3	35
#define CODE_REFQ3		36
#define CODE_REFTIME3	37
#define CODE_REFSCORE3	38
#define CODE_REFHT3		39
#define CODE_REF3		40
#define CODE_REFPLACE4	41
#define CODE_REFQ4		42
#define CODE_REFTIME4	43
#define CODE_REFSCORE4	44
#define CODE_REFHT4		45
#define CODE_REF4		46

// Make sure these are ordered by the aboves codes
static const UINT sc_codes[] =
{
	IDS_COL_DOG,
	IDS_COL_DATE,
	IDS_COL_VENUE,
	IDS_COL_CLUB,
	IDS_COL_DIV_LVL_EVT,
	IDS_COL_LOCATION,
	IDS_COL_HEIGHT,
	IDS_COL_JUDGE,
	IDS_COL_HANDLER,
	IDS_COL_CONDITIONS,
	IDS_COL_Q,
	IDS_COL_SCT,
	IDS_COL_YARDS,
	IDS_COL_OPEN_CLOSE,
	IDS_COL_TIME,
	IDS_COL_OBSTACLES,
	IDS_COL_FAULTS,
	IDS_COL_SCORE,
	IDS_COL_PLACE,
	IDS_COL_IN_CLASS,
	IDS_COL_Q_D,
	IDS_COL_OTHERPOINTS,
	IDS_COL_COMMENTS,
	IDS_COL_PLACE,
	IDS_COL_Q,
	IDS_COL_TIME,
	IDS_COL_SCORE,
	IDS_COL_HEIGHT,
	IDS_COL_NAME_BREED_NOTE,
	IDS_COL_PLACE,
	IDS_COL_Q,
	IDS_COL_TIME,
	IDS_COL_SCORE,
	IDS_COL_HEIGHT,
	IDS_COL_NAME_BREED_NOTE,
	IDS_COL_PLACE,
	IDS_COL_Q,
	IDS_COL_TIME,
	IDS_COL_SCORE,
	IDS_COL_HEIGHT,
	IDS_COL_NAME_BREED_NOTE,
	IDS_COL_PLACE,
	IDS_COL_Q,
	IDS_COL_TIME,
	IDS_COL_SCORE,
	IDS_COL_HEIGHT,
	IDS_COL_NAME_BREED_NOTE,
};
#define FOR_TIME	0x1
#define FOR_PTS		0x2
#define FOR_BOTH	0x3
static const struct
{
	unsigned char type;
	int line;
	int box;
	int colspan; // number of boxes to span
	int rowspan;
	bool bContinuation;
	int code;
	UINT fmt;
} sc_lines[] =
{
	{FOR_BOTH,  0, 0, 2, 1, false, CODE_DOG,        DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  0, 2, 2, 1, false, CODE_DATE,       DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  0, 4, 2, 1, false, CODE_VENUE,      DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  0, 6, 2, 1, false, CODE_LOCATION,   DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  1, 0, 3, 1, false, CODE_DIV,        DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  1, 3, 1, 1, false, CODE_HEIGHT,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  1, 4, 4, 2, false, CODE_CLUB,       DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH,  2, 4, 4, 1, true,  CODE_CLUB, 0},
	{FOR_BOTH,  2, 0, 2, 1, false, CODE_JUDGE,      DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  2, 2, 2, 1, false, CODE_HANDLER,    DT_LEFT | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  3, 0, 7, 1, false, CODE_CONDITIONS, DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH,  3, 7, 1, 1, false, CODE_Q,          DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 0, 1, 1, false, CODE_SCT,        DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_TIME,  4, 1, 1, 1, false, CODE_YARDS,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_PTS,   4, 1, 1, 1, false, CODE_OPEN,       DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 2, 1, 1, false, CODE_OBSTACLES,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 3, 1, 1, false, CODE_TIME,       DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_TIME,  4, 4, 1, 1, false, CODE_FAULTS,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_PTS,   4, 4, 1, 1, false, CODE_SCORE,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 5, 1, 1, false, CODE_PLACE,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 6, 1, 1, false, CODE_INCLASS,    DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  4, 7, 1, 1, false, CODE_QD,         DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH,  5, 0, 8, 5, false, CODE_COMMENTS,   DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH,  6, 0, 8, 4, true,  CODE_COMMENTS, 0},
	{FOR_BOTH,  7, 0, 8, 3, true,  CODE_COMMENTS, 0},
	{FOR_BOTH,  8, 0, 8, 2, true,  CODE_COMMENTS, 0},
	{FOR_BOTH,  9, 0, 6, 1, true,  CODE_COMMENTS, 0},
	{FOR_BOTH,  9, 6, 2, 1, false, CODE_OTHER,      DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH, 10, 0, 1, 1, false, CODE_REFPLACE1,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 10, 1, 1, 1, false, CODE_REFQ1,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 10, 2, 1, 1, false, CODE_REFTIME1,   DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 10, 3, 1, 1, false, CODE_REFSCORE1,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 10, 4, 1, 1, false, CODE_REFHT1,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 10, 5, 3, 1, false, CODE_REF1,       DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH, 11, 0, 1, 1, false, CODE_REFPLACE2,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 11, 1, 1, 1, false, CODE_REFQ2,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 11, 2, 1, 1, false, CODE_REFTIME2,   DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 11, 3, 1, 1, false, CODE_REFSCORE2,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 11, 4, 1, 1, false, CODE_REFHT2,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 11, 5, 3, 1, false, CODE_REF2,       DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH, 12, 0, 1, 1, false, CODE_REFPLACE3,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 12, 1, 1, 1, false, CODE_REFQ3,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 12, 2, 1, 1, false, CODE_REFTIME3,   DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 12, 3, 1, 1, false, CODE_REFSCORE3,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 12, 4, 1, 1, false, CODE_REFHT3,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 12, 5, 3, 1, false, CODE_REF3,       DT_LEFT | DT_WORDBREAK},
	{FOR_BOTH, 13, 0, 1, 1, false, CODE_REFPLACE4,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 13, 1, 1, 1, false, CODE_REFQ4,      DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 13, 2, 1, 1, false, CODE_REFTIME4,   DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 13, 3, 1, 1, false, CODE_REFSCORE4,  DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 13, 4, 1, 1, false, CODE_REFHT4,     DT_CENTER | DT_SINGLELINE | DT_BOTTOM},
	{FOR_BOTH, 13, 5, 3, 1, false, CODE_REF4,       DT_LEFT | DT_WORDBREAK},
};
static const int sc_nLines = sizeof(sc_lines) / sizeof(sc_lines[0]);


static void RefRunHelper(otstringstream& text, ARBDogReferenceRunPtr ref, int code)
{
	switch (code)
	{
	case CODE_REFPLACE1:
	case CODE_REFPLACE2:
	case CODE_REFPLACE3:
	case CODE_REFPLACE4:
		if (0 < ref->GetPlace())
			text << ref->GetPlace();
		break;
	case CODE_REFQ1:
	case CODE_REFQ2:
	case CODE_REFQ3:
	case CODE_REFQ4:
		if ((ARB_Q::eQ)ref->GetQ() != ARB_Q::eQ_NA)
			text << ref->GetQ().str();
		break;
	case CODE_REFTIME1:
	case CODE_REFTIME2:
	case CODE_REFTIME3:
	case CODE_REFTIME4:
		{
			double val = ref->GetTime();
			if (0.0 < val)
				text << ARBDouble::str(val);
		}
		break;
	case CODE_REFSCORE1:
	case CODE_REFSCORE2:
	case CODE_REFSCORE3:
	case CODE_REFSCORE4:
		text << ref->GetScore();
		break;
	case CODE_REFHT1:
	case CODE_REFHT2:
	case CODE_REFHT3:
	case CODE_REFHT4:
		text << ref->GetHeight();
		break;
	case CODE_REF1:
	case CODE_REF2:
	case CODE_REF3:
	case CODE_REF4:
		text << ref->GetName();
		if (!ref->GetBreed().empty())
			text << "/" << ref->GetBreed();
		if (!ref->GetNote().empty())
			text << "/" << ref->GetNote();
		break;
	}
}


tstring CPrintRuns::GetFieldText(ARBDogTrialPtr trial, ARBDogRunPtr run, int code)
{
	otstringstream text;
	switch (code)
	{
	default:
		break;
	case CODE_DOG:
		if (m_dog)
			text << m_dog->GetCallName();
		break;
	case CODE_DATE:
		if (run)
			text << run->GetDate().GetString(ARBDate::eDashYYYYMMDD);
		break;
	case CODE_VENUE:
		if (trial)
		{
			int i = 0;
			for (ARBDogClubList::iterator iter = trial->GetClubs().begin();
				iter != trial->GetClubs().end();
				++iter, ++i)
			{
				if (0 < i)
					text << "/";
				text << (*iter)->GetVenue();
			}
			break;
		}
	case CODE_CLUB:
		if (trial)
		{
			int i = 0;
			for (ARBDogClubList::iterator iter = trial->GetClubs().begin();
				iter != trial->GetClubs().end();
				++iter, ++i)
			{
				if (0 < i)
					text << "/";
				text << (*iter)->GetName();
			}
			break;
		}
	case CODE_DIV:
		if (run)
		{
			text << run->GetDivision()
				<< "/" << run->GetLevel()
				<< "/" << run->GetEvent();
		}
		break;
	case CODE_LOCATION:
		if (trial)
			text << trial->GetLocation();
		break;
	case CODE_HEIGHT:
		if (run)
			text << run->GetHeight();
		break;
	case CODE_JUDGE:
		if (run)
			text << run->GetJudge();
		break;
	case CODE_HANDLER:
		if (run)
			text << run->GetHandler();
		break;
	case CODE_CONDITIONS:
		if (run)
			text << run->GetConditions();
		break;
	case CODE_Q:
		if (run)
		{
			if ((ARB_Q::eQ)run->GetQ() != ARB_Q::eQ_NA)
				text << run->GetQ().str();
		}
		break;
	case CODE_SCT:
		if (run)
		{
			double val = run->GetScoring().GetSCT();
			if (0.0 < val)
				text << ARBDouble::str(val);
		}
		break;
	case CODE_YARDS:
		if (run)
		{
			double val = run->GetScoring().GetYards();
			if (0.0 < val)
				text << ARBDouble::str(val, 0);
		}
		break;
	case CODE_OPEN:
		if (run)
		{
			switch (run->GetScoring().GetType())
			{
			case ARBDogRunScoring::eTypeByOpenClose:
				if (0 < run->GetScoring().GetNeedOpenPts())
					text << run->GetScoring().GetNeedOpenPts();
				text << " / ";
				if (0 < run->GetScoring().GetNeedClosePts())
					text << run->GetScoring().GetNeedClosePts();
				break;
			case ARBDogRunScoring::eTypeByPoints:
				if (0 < run->GetScoring().GetNeedOpenPts())
					text << run->GetScoring().GetNeedOpenPts();
				break;
			}
		}
		break;
	case CODE_TIME:
		if (run)
		{
			double val = run->GetScoring().GetTime();
			if (0.0 < val)
				text << ARBDouble::str(val);
		}
		break;
	case CODE_FAULTS:
		if (trial && run)
		{
			ARBConfigScoringPtr pScoring;
			if (m_config && trial->GetClubs().GetPrimaryClub())
				m_config->GetVenues().FindEvent(
					trial->GetClubs().GetPrimaryClubVenue(),
					run->GetEvent(),
					run->GetDivision(),
					run->GetLevel(),
					run->GetDate(),
					NULL,
					&pScoring);
			double timeFaults = run->GetScoring().GetTimeFaults(pScoring);
			if ((ARB_Q::eQ)run->GetQ() != ARB_Q::eQ_NA
			|| (0 < run->GetScoring().GetCourseFaults() || 0.0 < timeFaults))
			{
				text << run->GetScoring().GetCourseFaults();
				if (0.0 < timeFaults)
					text << " + " << ARBDouble::str(timeFaults, 0);
			}
		}
		break;
	case CODE_SCORE:
		if (run)
		{
			switch (run->GetScoring().GetType())
			{
			case ARBDogRunScoring::eTypeByOpenClose:
				if (0 < run->GetScoring().GetOpenPts())
					text << run->GetScoring().GetOpenPts();
				text << " / ";
				if (0 < run->GetScoring().GetClosePts())
					text << run->GetScoring().GetClosePts();
				break;
			case ARBDogRunScoring::eTypeByPoints:
				if (0 < run->GetScoring().GetOpenPts())
					text << run->GetScoring().GetOpenPts();
				break;
			}
		}
		break;
	case CODE_PLACE:
		if (run && 0 < run->GetPlace())
			text << run->GetPlace();
		break;
	case CODE_INCLASS:
		if (run && 0 < run->GetInClass())
			text << run->GetInClass();
		break;
	case CODE_QD:
		if (run && 0 <= run->GetDogsQd())
			text << run->GetDogsQd();
		break;
	case CODE_COMMENTS:
		if (run)
			text << run->GetNote();
		break;
	case CODE_OTHER:
		if (run && 0 < run->GetOtherPoints().size())
		{
			int i = 0;
			for (ARBDogRunOtherPointsList::iterator iter = run->GetOtherPoints().begin();
				iter != run->GetOtherPoints().end();
				++iter, ++i)
			{
				if (0 < i)
					text << " ";
				text << (*iter)->GetName() << ":" << (*iter)->GetPoints();
			}
		}
		break;
	case CODE_REFPLACE1:
	case CODE_REFQ1:
	case CODE_REFTIME1:
	case CODE_REFSCORE1:
	case CODE_REFHT1:
	case CODE_REF1:
		if (run && 0 < run->GetReferenceRuns().size())
			RefRunHelper(text, run->GetReferenceRuns()[0], code);
		break;
	case CODE_REFPLACE2:
	case CODE_REFQ2:
	case CODE_REFTIME2:
	case CODE_REFSCORE2:
	case CODE_REFHT2:
	case CODE_REF2:
		if (run && 1 < run->GetReferenceRuns().size())
			RefRunHelper(text, run->GetReferenceRuns()[1], code);
		break;
	case CODE_REFPLACE3:
	case CODE_REFQ3:
	case CODE_REFTIME3:
	case CODE_REFSCORE3:
	case CODE_REFHT3:
	case CODE_REF3:
		if (run && 2 < run->GetReferenceRuns().size())
			RefRunHelper(text, run->GetReferenceRuns()[2], code);
		break;
	case CODE_REFPLACE4:
	case CODE_REFQ4:
	case CODE_REFTIME4:
	case CODE_REFSCORE4:
	case CODE_REFHT4:
	case CODE_REF4:
		if (run && 3 < run->GetReferenceRuns().size())
			RefRunHelper(text, run->GetReferenceRuns()[3], code);
		break;
	}
	return text.str();
}


void CPrintRuns::PrintPage(UINT nCurPage, CDC* pDC, CRect inRect)
{
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetBkMode(TRANSPARENT);
	CFont fontText, fontData;
	fontText.CreatePointFont(70, _T("MS Sans Serif"), pDC);
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 90;
	lf.lfWeight = FW_BOLD;
#if _MSC_VER >= 1400
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"), 5);
#else
	_tcsncpy(lf.lfFaceName, _T("Arial"), 5);
#endif
	fontData.CreatePointFontIndirect(&lf, pDC);
	CFont* pOldFont = pDC->SelectObject(&fontText);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int h = inRect.Height() / 29;
	int w = inRect.Width() / 8;
	CRect r[2];
	r[0] = inRect;
	r[1] = inRect;
	r[0].bottom = r[0].top + h * 14;
	r[1].top = r[1].bottom - h * 14;

	for (int iItem = 0; iItem < 2; ++iItem, ++m_cur)
	{
		ARBDogTrialPtr trial;
		ARBDogRunPtr run;
		bool bPoints = false;
		if (m_runs.size() == 0)
		{
			bPoints = (0 == nCurPage % 2);
		}
		else if (m_cur >= m_runs.size())
		{
			break;
		}
		else
		{
			trial = m_runs[m_cur].first;
			run = m_runs[m_cur].second;
			if (!run)
				continue;
			switch (run->GetScoring().GetType())
			{
			default:
			case ARBDogRunScoring::eTypeByTime:
				break;
			case ARBDogRunScoring::eTypeByOpenClose:
			case ARBDogRunScoring::eTypeByPoints:
				bPoints = true;
				break;
			}
		}
		// Frame the whole thing
		pDC->Rectangle(r[iItem]);

		// Now go thru each box.
		for (int j = 0; j < sc_nLines; ++j)
		{
			if ((bPoints && (sc_lines[j].type & FOR_PTS))
			|| (!bPoints && (sc_lines[j].type & FOR_TIME)))
			{
				CRect rect;
				rect.left = r[iItem].left + w * sc_lines[j].box;
				rect.right = rect.left + w * sc_lines[j].colspan + 1;
				if (8 == sc_lines[j].box + sc_lines[j].colspan)
					rect.right = r[iItem].right;
				rect.top = r[iItem].top + h * sc_lines[j].line;
				rect.bottom = rect.top + h;
				// Draw vertical separator lines (on left)
				if (0 < sc_lines[j].box)
				{
					pDC->MoveTo(rect.left, rect.top);
					pDC->LineTo(rect.left, rect.bottom);
				}

				tstring str = GetFieldText(trial, run, sc_lines[j].code);

				// Draw horizontal separator lines (on top)
				if (0 < sc_lines[j].line && (str.empty() || !sc_lines[j].bContinuation))
				{
					CPen penGray;
					CPen* oldPen = NULL;
					if (sc_lines[j].bContinuation)
					{
						penGray.CreatePen(PS_SOLID, 1, RGB(192,192,192));
						oldPen = pDC->SelectObject(&penGray);
					}
					pDC->MoveTo(rect.left, rect.top);
					pDC->LineTo(rect.right, rect.top);
					if (oldPen)
						pDC->SelectObject(oldPen);
				}
				if (sc_lines[j].bContinuation)
					str.erase();

				rect.InflateRect(-1, 1);
				CRect rText(rect);
				if (!sc_lines[j].bContinuation)
				{
					CString caption;
					caption.LoadString(sc_codes[sc_lines[j].code]);
					pDC->DrawText(caption, rect, DT_LEFT | DT_TOP| DT_NOPREFIX);
					pDC->DrawText(caption, rText, DT_LEFT | DT_TOP | DT_NOPREFIX | DT_CALCRECT);
				}
				if (!str.empty())
				{
					if (1 < sc_lines[j].rowspan)
						rect.bottom -= rect.Width() * sc_lines[j].rowspan;
					pDC->SelectObject(&fontData);
					UINT flags = DT_NOPREFIX | sc_lines[j].fmt;
					if (!(flags & DT_VCENTER))
						rect.top = rText.bottom + 1;
					pDC->DrawText(str.c_str(), -1, rect, flags);
					pDC->SelectObject(&fontText);
				}
			}
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


static enum RingBinder
{
	eSmall3Ring,
	eLarge3Ring,
	eLarge4Ring
};

static void PrintMark(CDC* pDC, int x, int y)
{
	pDC->MoveTo(x - 5, y - 5);
	pDC->LineTo(x + 5, y + 5);
	pDC->MoveTo(x + 5, y - 5);
	pDC->LineTo(x - 5, y + 5);
}

// Binder rings (based on US 8.5x11 paper)
// Rings are inset 3/8 inch
// sm 3ring: 2.75
// lg 3ring: 4.25
// lg 4ring: 1.375/4.25/1.375
static void PrintBinderMarkings(
		RingBinder style,
		CDC* pDC,
		CRect rPrinted,
		int margin)
{
	int x = rPrinted.left - margin + 38;
	int y = rPrinted.top + rPrinted.Height() / 2; // centered
	switch (style)
	{
	default:
	case eSmall3Ring:
		PrintMark(pDC, x, y);
		PrintMark(pDC, x, y - 275);
		PrintMark(pDC, x, y + 275);
		break;
	case eLarge3Ring:
		PrintMark(pDC, x, y);
		PrintMark(pDC, x, y - 425);
		PrintMark(pDC, x, y + 425);
		break;
	case eLarge4Ring:
		PrintMark(pDC, x, y - 325); // 1 1/8 + 2 1/8
		PrintMark(pDC, x, y - 238);
		PrintMark(pDC, x, y + 238);
		PrintMark(pDC, x, y + 325);
		break;
	}
}


void CPrintRuns::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	int saveDC = pDC->SaveDC();

	// Units are .01 inches.
	pDC->SetMapMode(MM_LOENGLISH);

	// Drawing area. Note, located at 0,0.
	CRect r(pInfo->m_rectDraw);
	pDC->DPtoLP(r);
	// Note, we could get the physical size of the page and try and get our
	// margin at exactly 1/2 inch. But observation has revealed that what the
	// printer reports as the offset, isn't what the printer prints as the
	// offset. So pretent the drawing area and physical area are the same.
	// (if printing to a pdf "printer", they are!)

	// Indent 1/2 inch
	int margin = 50;

	// 'r' is full print area
	//pDC->Rectangle(r);
	CPoint pt = r.TopLeft();
	// Landscape
	if (abs(r.Width()) > abs(r.Height()))
	{
		//510x817
		CRect r1(r);
		r1.left += margin;
		r1.top -= margin;
		r1.right = r.left + r.Width() / 2 - margin;
		r1.bottom += margin;
		PrintPage(pInfo->m_nCurPage, pDC, r1);
		// 3ring small
		PrintBinderMarkings(eSmall3Ring, pDC, r1, margin);

		r1.left = r1.right + margin;
		pDC->MoveTo(r1.TopLeft());
		pDC->LineTo(r1.left, r1.bottom);
		r1.left += margin;
		r1.right = r.right - margin;
		PrintPage(pInfo->m_nCurPage, pDC, r1);
		PrintBinderMarkings(eSmall3Ring, pDC, r1, margin);
	}
	// Portrait
	else
	{
		CRect r1(r);
		r1.left += margin;
		r1.top -= margin;
		r1.right -= margin;
		r1.bottom += margin;
		PrintPage(pInfo->m_nCurPage, pDC, r1);
		PrintBinderMarkings(eLarge3Ring, pDC, r1, margin);
		PrintBinderMarkings(eLarge4Ring, pDC, r1, margin);
	}

	pDC->RestoreDC(saveDC);
}

/////////////////////////////////////////////////////////////////////////////

bool PrintRuns(ARBConfig const* inConfig, ARBDogPtr inDog, std::vector<RunInfo> const& inRuns)
{
	CPrintRuns runs(inConfig, inDog, inRuns);

	// Style the printing after CView (code is based on CView::OnFilePrint)

	CPrintInfo printInfo;
	printInfo.m_rectDraw.SetRectEmpty();
	assert(printInfo.m_pPD != NULL);    // must be set
	if (!runs.OnPreparePrinting(&printInfo))
		return false;
	// hDC must be set (did you remember to call DoPreparePrinting?)
	assert(printInfo.m_pPD->m_pd.hDC != NULL);

	CString strTitle;
	strTitle.LoadString(IDS_RUNS);

	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = strTitle;
	docInfo.lpszOutput = NULL;
	CString strPortName = printInfo.m_pPD->GetPortName();

	CDC dcPrint;
	dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
	dcPrint.m_bPrinting = TRUE;

	runs.OnBeginPrinting(&dcPrint, &printInfo);

	dcPrint.SetAbortProc(AbortProc);

	CWnd* hwndTemp = AfxGetMainWnd();
	assert(hwndTemp);
	hwndTemp->EnableWindow(FALSE);
	CPrintingDialog2 dlgPrintStatus(AfxGetMainWnd());

	CString strTemp;
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTitle);
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME, printInfo.m_pPD->GetDeviceName());
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strPortName);
	dlgPrintStatus.ShowWindow(SW_SHOW);
	dlgPrintStatus.UpdateWindow();

	// Initialise print document details
	// Begin a new print job
	int jobNumber = dcPrint.StartDoc(&docInfo);
#if _MSC_VER >= 1300
	printInfo.m_nJobNumber = jobNumber;
#endif
	if (jobNumber == SP_ERROR)
	{
		// enable main window before proceeding
		hwndTemp->EnableWindow(TRUE);
		// cleanup and show error message
		runs.OnEndPrinting(&dcPrint, &printInfo);
		dlgPrintStatus.DestroyWindow();
		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
		AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
		return false;
	}

	// Guarantee values are in the valid range
	UINT nEndPage = printInfo.GetToPage();
	UINT nStartPage = printInfo.GetFromPage();

	if (nEndPage < printInfo.GetMinPage())
		nEndPage = printInfo.GetMinPage();
	if (nEndPage > printInfo.GetMaxPage())
		nEndPage = printInfo.GetMaxPage();

	if (nStartPage < printInfo.GetMinPage())
		nStartPage = printInfo.GetMinPage();
	if (nStartPage > printInfo.GetMaxPage())
		nStartPage = printInfo.GetMaxPage();

	int nStep = (nEndPage >= nStartPage) ? 1 : -1;
	nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

	VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

	bool bError = false;

	for (printInfo.m_nCurPage = nStartPage;
		printInfo.m_nCurPage != nEndPage;
		printInfo.m_nCurPage += nStep)
	{
		runs.OnPrepareDC(&dcPrint, &printInfo);
		// check for end of print
		if (!printInfo.m_bContinuePrinting)
			break;
		// write current page
		TCHAR szBuf[80];
#if _MSC_VER >= 1400
		ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuf, _countof(szBuf), _countof(szBuf) - 1, strTemp, printInfo.m_nCurPage));
#else
		sprintf(szBuf, strTemp, printInfo.m_nCurPage);
#endif
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

		// set up drawing rect to entire page (in logical coordinates)
		printInfo.m_rectDraw.SetRect(0, 0,
			dcPrint.GetDeviceCaps(HORZRES),
			dcPrint.GetDeviceCaps(VERTRES));
		dcPrint.DPtoLP(&printInfo.m_rectDraw);

		// attempt to start the current page
		if (dcPrint.StartPage() < 0)
		{
			bError = true;
			break;
		}

		// must call OnPrepareDC on newer versions of Windows because
		// StartPage now resets the device attributes.
		runs.OnPrepareDC(&dcPrint, &printInfo);

		assert(printInfo.m_bContinuePrinting);

		// page successfully started, so now render the page
		runs.OnPrint(&dcPrint, &printInfo);
		if (nStep > 0) // pages are printed in ascending order
		{
			// OnPrint may have set the last page
			// because the end of the document was reached.
			// The loop must not continue with the next iteration.
		// Is this a bug in CView? Leaving this here causes it to print all pages.
		// If there are 5 pages available, and I specified 1-2, this changes it
		// to print all 5.
			//nEndPage = printInfo.GetMaxPage() + nStep; 
		}

		// If the user restarts the job when it's spooling, all 
		// subsequent calls to EndPage returns < 0. The first time
		// GetLastError returns ERROR_PRINT_CANCELLED
		if (dcPrint.EndPage() < 0 && (GetLastError()!= ERROR_SUCCESS))
		{
			// Don't bother. CView does a bunch of stuff. We die.
			bError = true;
			break;
		}
		if (!AbortProc(dcPrint.m_hDC, 0))
		{		
			bError = true;
			break;
		}
	}
	if (!bError)
		dcPrint.EndDoc();
	else
		dcPrint.AbortDoc();

	hwndTemp->EnableWindow();    // enable main window

	runs.OnEndPrinting(&dcPrint, &printInfo);              // Call your "Clean up" function

	dlgPrintStatus.DestroyWindow();
	dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor

	return bError;
}
