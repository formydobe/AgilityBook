/*
 * Copyright � 2002-2005 David Connet. All Rights Reserved.
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
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2003-10-13 DRC Make ref run dlg default to perfect score.
 */

#include "stdafx.h"
#include "AgilityBook.h"
#include "DlgRun.h"

#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "ARBDogRun.h"
#include "DlgRunComments.h"
#include "DlgRunCRCD.h"
#include "DlgRunLink.h"
#include "DlgRunReference.h"
#include "DlgRunScore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRun

IMPLEMENT_DYNAMIC(CDlgRun, CDlgBaseSheet)

CDlgRun::CDlgRun(
		CAgilityBookDoc* pDoc,
		ARBDogTrial* pTrial,
		ARBDogRun* pRun,
		CWnd* pParent,
		UINT iSelectPage)
	: CDlgBaseSheet(IDS_RUN_PROPERTIES, pParent, iSelectPage)
	, m_pDoc(pDoc)
	, m_pTrial(pTrial)
	, m_pRealRun(pRun)
	, m_Run(new ARBDogRun(*pRun))
	, m_pageScore(NULL)
	, m_pageComments(NULL)
	, m_pageReference(NULL)
	, m_pageCRCD(NULL)
	, m_pageLink(NULL)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	ARBDogClub* pClub;
	pTrial->GetClubs().GetPrimaryClub(&pClub);
	ASSERT(NULL != pClub);
	ARBConfigVenue* pVenue;
	pDoc->GetConfig().GetVenues().FindVenue(pClub->GetVenue(), &pVenue);
	pClub->Release();
	ASSERT(NULL != pVenue);

	m_pageScore = new CDlgRunScore(pDoc, pVenue, pTrial, m_pRealRun, m_Run);
	m_pageComments = new CDlgRunComments(pDoc, m_Run);
	m_pageReference = new CDlgRunReference(pDoc, pVenue, m_Run);
	m_pageCRCD = new CDlgRunCRCD(m_Run);
	m_pageLink = new CDlgRunLink(pDoc, m_Run);
	AddPage(m_pageScore);
	AddPage(m_pageComments);
	AddPage(m_pageReference);
	AddPage(m_pageCRCD);
	AddPage(m_pageLink);
	pVenue->Release();
}

CDlgRun::~CDlgRun()
{
	m_Run->Release();
	m_Run = NULL;
	delete m_pageScore;
	delete m_pageComments;
	delete m_pageReference;
	delete m_pageCRCD;
	delete m_pageLink;
}

BEGIN_MESSAGE_MAP(CDlgRun, CDlgBaseSheet)
	//{{AFX_MSG_MAP(CDlgRun)
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRun message handlers

void CDlgRun::OnOK()
{
	if (GetActivePage()->UpdateData(TRUE))
	{
		*m_pRealRun = *m_Run;
		m_pTrial->SetMultiQs(m_pDoc->GetConfig());
		CAgilityBookOptions::SetLastEnteredDivision(m_Run->GetDivision().c_str());
		CAgilityBookOptions::SetLastEnteredLevel(m_Run->GetLevel().c_str());
		CAgilityBookOptions::SetLastEnteredHeight(m_Run->GetHeight().c_str());
		CAgilityBookOptions::SetLastEnteredJudge(m_Run->GetJudge().c_str());
		CAgilityBookOptions::SetLastEnteredHandler(m_Run->GetHandler().c_str());
		EndDialog(IDOK);
	}
}
