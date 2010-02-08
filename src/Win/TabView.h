#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 *
 * @brief Tab view
 * @author David Connet
 *
 * Revision History
 * @li 2009-01-06 DRC Ported to wxWidgets.
 */

#include "CommonView.h"
#include <wx/choicebk.h>
#include <wx/docview.h>
#include <wx/imaglist.h>
#include <wx/listbook.h>
#include <wx/notebook.h>
#include <wx/toolbook.h>
#include <wx/treebook.h>
class CMainFrame;

#define IDX_PANE_RUNS		0
#define IDX_PANE_POINTS		1
#define IDX_PANE_CALENDAR	2
#define IDX_PANE_LOG		3


class CTabView : public CAgilityBookBaseView
{
	DECLARE_DYNAMIC_CLASS(CTabView)

public:
	CTabView();
	~CTabView();
	bool ShowPointsAsHtml(bool bHtml);
	int GetCurTab() const;
	int SetCurTab(int index);
	virtual void OnChangeFilename();
	virtual bool OnCreate(wxDocument* doc, long flags);
	virtual void OnDraw(wxDC* dc);
	virtual void OnUpdate(wxView* sender, wxObject* inHint = NULL);

	void OnType(int id);
	void OnOrient(int id);

private:
	void RecreateBook(wxDocument* doc, long flags, bool bOnCreate);

	CMainFrame* m_frame;
	long m_type;
	long m_orient;
	wxBoxSizer* m_sizerFrame;
	wxBookCtrlBase* m_ctrlBook;
	wxImageList m_imageList;
	bool m_bIgnoreEvents;

	void OnBookCtrlChanged(wxBookCtrlBaseEvent& evt);
#if wxUSE_NOTEBOOK
	void OnNotebookChanged(wxNotebookEvent& evt)		{OnBookCtrlChanged(evt);}
#endif
#if wxUSE_CHOICEBOOK
	void OnChoicebookChanged(wxChoicebookEvent& evt)	{OnBookCtrlChanged(evt);}
#endif
#if wxUSE_LISTBOOK
	void OnListbookChanged(wxListbookEvent& evt)		{OnBookCtrlChanged(evt);}
#endif
#if wxUSE_TREEBOOK
	void OnTreebookChanged(wxTreebookEvent& evt)		{OnBookCtrlChanged(evt);}
#endif
#if wxUSE_TOOLBOOK
	void OnToolbookChanged(wxToolbookEvent& evt)		{OnBookCtrlChanged(evt);}
#endif
};