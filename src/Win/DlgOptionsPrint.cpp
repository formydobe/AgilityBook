/*
 * Copyright � 2006 David Connet. All Rights Reserved.
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
 * @brief implementation of the CDlgOptionsPrint class
 * @author David Connet
 *
 * Revision History
 * @li 2006-11-22 DRC Moved printing to new page.
 */

#include "stdafx.h"
#include "AgilityBook.h"
#include "DlgOptionsPrint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionsPrint property page

IMPLEMENT_DYNAMIC(CDlgOptionsPrint, CDlgBasePropertyPage)

CDlgOptionsPrint::CDlgOptionsPrint()
	: CDlgBasePropertyPage(CDlgOptionsPrint::IDD)
	, m_Left(0)
{
	//{{AFX_DATA_INIT(CDlgOptionsPrint)
	//}}AFX_DATA_INIT
}

CDlgOptionsPrint::~CDlgOptionsPrint()
{
}

void CDlgOptionsPrint::DoDataExchange(CDataExchange* pDX)
{
	CDlgBasePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionsPrint)
	DDX_Control(pDX, IDC_OPT_PRINT_FONT_TEXT, m_ctrlFontPrint);
	DDX_Text(pDX, IDC_OPT_PRINT_MARGIN_L, m_Left);
	DDX_Text(pDX, IDC_OPT_PRINT_MARGIN_R, m_Right);
	DDX_Text(pDX, IDC_OPT_PRINT_MARGIN_T, m_Top);
	DDX_Text(pDX, IDC_OPT_PRINT_MARGIN_B, m_Bottom);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgOptionsPrint, CDlgBasePropertyPage)
	//{{AFX_MSG_MAP(CDlgOptionsPrint)
	ON_BN_CLICKED(IDC_OPT_PRINT_FONT, OnFontPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionsPrint message handlers

BOOL CDlgOptionsPrint::OnInitDialog() 
{
	CDlgBasePropertyPage::OnInitDialog();
	m_fontPrintInfo.CreateFont(m_fontPrint);
	m_ctrlFontPrint.SetFont(&m_fontPrint);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptionsPrint::OnFontPrint()
{
	UpdateData(TRUE);
	LOGFONT logFont;
	m_fontPrint.GetLogFont(&logFont);
	CFontDialog dlg(&logFont, CF_SCREENFONTS);
	if (IDOK == dlg.DoModal())
	{
		m_fontPrintInfo.CreateFont(dlg, m_fontPrint);
		m_ctrlFontPrint.SetFont(&(m_fontPrint));
	}
}
