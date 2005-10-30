#pragma once

/*
 * Copyright � 2004-2005 David Connet. All Rights Reserved.
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
 * @brief interface of the CDlgFindLinks class
 * @author David Connet
 *
 * Revision History
 * @li 2004-06-02 DRC Added 'Open' button.
 * @li 2004-03-31 DRC Created.
 */

#include <afxinet.h>
#include <vector>
#include "ColumnOrder.h"
#include "DlgBaseDialog.h"
#include "ListCtrl.h"

class ARBDog;
class ARBDogList;
class ARBDogRun;
class ARBDogTrial;

class CDlgFindLinks : public CDlgBaseDialog
{
	friend int CALLBACK CompareLinks(LPARAM, LPARAM, LPARAM);
public:
	CDlgFindLinks(
			ARBDogList& inDogs,
			CWnd* pParent = NULL);
	virtual ~CDlgFindLinks();
	size_t GetNumLinks() const		{return m_Data.size();}

private:
// Dialog Data
	//{{AFX_DATA(CDlgFindLinks)
	enum { IDD = IDD_FIND_LINKS };
	CListCtrl2	m_ctrlLinks;
	CButton	m_ctrlEdit;
	CButton	m_ctrlOpen;
	//}}AFX_DATA
	CColumnOrder m_sortLinks;
	CImageList m_ImageList;
	int m_imgEmpty;
	int m_imgOk;
	int m_imgMissing;
	CInternetSession m_Session;

	class CDlgFindLinksData
	{
	public:
		CDlgFindLinksData(
				ARBDog* pDog,
				ARBDogTrial* pTrial,
				ARBDogRun* pRun,
				ARBString const& inLink);
		~CDlgFindLinksData();
		ARBDog* m_pDog;
		ARBDogTrial* m_pTrial;
		ARBDogRun* m_pRun;
		ARBString m_OldLink;
		ARBString m_Link;
	};
	std::vector<CDlgFindLinksData> m_Data;

	//{{AFX_VIRTUAL(CDlgFindLinks)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	int GetImageIndex(ARBString const& inLink);
	void SetColumnHeaders();
	void UpdateButtons();

	//{{AFX_MSG(CDlgFindLinks)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnEdit();
	afx_msg void OnOpen();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
