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
 * @brief interface of the CDlgExistingPoints class
 * @author David Connet
 *
 * Revision History
 * @li 2004-02-03 DRC Created
 */

#include "ARBDate.h"
#include "DlgBaseDialog.h"
class ARBDogExistingPoints;
class ARBDogExistingPointsList;
class CAgilityBookDoc;

class CDlgExistingPoints : public CDlgBaseDialog
{
public:
	CDlgExistingPoints(
			CAgilityBookDoc* pDoc,
			ARBDogExistingPointsList& points,
			ARBDogExistingPoints* pExistingPoints,
			CWnd* pParent = NULL);

private:
// Dialog Data
	//{{AFX_DATA(CDlgExistingPoints)
	enum { IDD = IDD_EXISTING_POINTS };
	CDateTimeCtrl	m_ctrlDate;
	CComboBox	m_ctrlType;
	CStatic	m_ctrlOtherText;
	CComboBox	m_ctrlOther;
	CStatic	m_ctrlMultiQText;
	CComboBox	m_ctrlMultiQ;
	CStatic	m_ctrlVenuesText;
	CComboBox	m_ctrlVenues;
	CStatic	m_ctrlDivisionsText;
	CComboBox	m_ctrlDivisions;
	CStatic	m_ctrlLevelsText;
	CComboBox	m_ctrlLevels;
	CStatic	m_ctrlEventsText;
	CComboBox	m_ctrlEvents;
	CStatic	m_ctrlSubNamesText;
	CComboBox	m_ctrlSubNames;
	CString	m_SubName;
	short	m_Points;
	CString	m_Comments;
	CButton	m_ctrlOk;
	//}}AFX_DATA
	CAgilityBookDoc* m_pDoc;
	ARBDogExistingPointsList& m_PointsList;
	ARBDogExistingPoints* m_pExistingPoints;
	CPoint m_ptControls[2][5];
	ARBDate m_Date;

	//{{AFX_VIRTUAL(CDlgExistingPoints)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GetEnableLists(
			int index,
			BOOL& outOther,
			BOOL& outMQ,
			BOOL& outVenue,
			BOOL& outDiv,
			BOOL& outLevel,
			BOOL& outEvent,
			BOOL& outSubName,
			bool bSet);
	void UpdateControls();
	void ClearLevels();
	void FillVenues();
	void FillFromVenue();
	void FillLevels();
	void FillEvents();
	void FillSubNames();
	//{{AFX_MSG(CDlgExistingPoints)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeType();
	afx_msg void OnSelchangeVenues();
	afx_msg void OnSelchangeDivision();
	afx_msg void OnSelchangeLevel();
	afx_msg void OnSelchangeEvent();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
