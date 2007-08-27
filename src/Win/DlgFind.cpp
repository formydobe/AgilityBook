/*
 * Copyright � 2003-2007 David Connet. All Rights Reserved.
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
 * @brief implementation of the CDlgFind class
 * @author David Connet
 *
 * Revision History
 * @li 2003-12-27 DRC Created
 */

#include "stdafx.h"
#include "AgilityBook.h"
#include "DlgFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFind dialog

CDlgFind::CDlgFind(
		IFindCallback& callback,
		CWnd* pParent)
	: CDlgBaseDialog(CDlgFind::IDD, pParent)
	, m_Callback(callback)
{
	//{{AFX_DATA_INIT(CDlgFind)
	m_strName = m_Callback.Text();
	m_bCase = m_Callback.MatchCase() ? TRUE : FALSE;
	m_Search = m_Callback.SearchAll() ? 0 : 1;
	m_Direction = m_Callback.SearchDown() ? 1 : 0;
	//}}AFX_DATA_INIT
}


void CDlgFind::DoDataExchange(CDataExchange* pDX)
{
	CDlgBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFind)
	DDX_Text(pDX, IDC_FIND_NAME, m_strName);
	DDX_Check(pDX, IDC_FIND_CASE, m_bCase);
	DDX_Radio(pDX, IDC_FIND_ALL, m_Search);
	DDX_Radio(pDX, IDC_FIND_UP, m_Direction);
	DDX_Control(pDX, IDOK, m_ctrlFind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFind, CDlgBaseDialog)
	//{{AFX_MSG_MAP(CDlgFind)
	ON_EN_CHANGE(IDC_FIND_NAME, OnChangeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFind message handlers

BOOL CDlgFind::OnInitDialog() 
{
	CDlgBaseDialog::OnInitDialog();
	CString caption = m_Callback.GetCaption();
	if (!caption.IsEmpty())
		SetWindowText(caption);
	if (0 < m_strName.GetLength())
		m_ctrlFind.EnableWindow(TRUE);
	else
		m_ctrlFind.EnableWindow(FALSE);
	if (!m_Callback.EnableSearch())
	{
    	GetDlgItem(IDC_FIND_ALL)->EnableWindow(FALSE);
    	GetDlgItem(IDC_FIND_VISIBLE)->EnableWindow(FALSE);
	}
	if (!m_Callback.EnableDirection())
	{
    	GetDlgItem(IDC_FIND_UP)->EnableWindow(FALSE);
    	GetDlgItem(IDC_FIND_DOWN)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgFind::OnChangeName() 
{
	UpdateData(TRUE);
	if (0 < m_strName.GetLength())
		m_ctrlFind.EnableWindow(TRUE);
	else
		m_ctrlFind.EnableWindow(FALSE);
}


void CDlgFind::OnOK() 
{
	UpdateData(TRUE);
	m_Callback.Text(m_strName);
	m_Callback.MatchCase(m_bCase ? true : false);
	m_Callback.SearchAll(0 == m_Search);
	m_Callback.SearchDown(1 == m_Direction);
	m_Callback.Search(this);
}
