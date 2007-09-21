/*
 * Copyright � 2002-2007 David Connet. All Rights Reserved.
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
 * @brief implementation of the CMainFrame class
 * @author David Connet
 *
 * Revision History
 * @li 2006-09-01 DRC Added multi-monitor support.
 * @li 2003-05-18 DRC Obsoleted registry settings "Calendar"/"List" (bool)
 *                    "Settings"/"View" (int) changed (see TabView.cpp)
 */

#include "stdafx.h"
#include <afxpriv.h>
#include "AgilityBook.h"
#include "MainFrm.h"

#include "ARBConfigVenue.h"
#include "AgilityBookDoc.h"
#include "AgilityBookOptions.h"
#include "AgilityBookTree.h"
#include "AgilityBookViewCalendar.h"
#include "AgilityBookViewCalendarList.h"
#include "AgilityBookViewPoints.h"
#include "AgilityBookViewRuns.h"
#include "Splash.h"
#include "TabView.h"

#if WINVER < 0x0500
#if _MSC_VER < 1400
#define COMPILE_MULTIMON_STUBS
#endif
#include <MultiMon.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS, OnUpdatePane)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FILTERED, OnUpdatePane)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DOG, OnUpdatePane)
	ON_COMMAND(ID_NEXT_TAB, OnNextTab)
	ON_COMMAND(ID_PREV_TAB, OnPrevTab)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_DOG,
	ID_INDICATOR_STATUS,
	ID_INDICATOR_FILTERED,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};


CMainFrame::CMainFrame()
	: m_pView(NULL)
{
}


CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS);
	m_wndStatusBar.SetPaneText(index, _T(""));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CSplashWnd::ShowSplashScreen(this);
	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	int x = AfxGetApp()->GetProfileInt(_T("Settings"), _T("lastXpos"), cs.x);
	int y = AfxGetApp()->GetProfileInt(_T("Settings"), _T("lastYpos"), cs.y);
	int cx = AfxGetApp()->GetProfileInt(_T("Settings"), _T("lastCX"), cs.cx);
	int cy = AfxGetApp()->GetProfileInt(_T("Settings"), _T("lastCY"), cs.cy);

	bool bCompute = false;
	CPoint curPt;
	GetCursorPos(&curPt);
	if (CW_USEDEFAULT != x)
	{
		bCompute = true;
		curPt.x = x;
	}
	if (CW_USEDEFAULT != y)
	{
		bCompute = true;
		curPt.y = y;
	}
	CSize curSize(0, 0);
	if (CW_USEDEFAULT != cx)
	{
		bCompute = true;
		curSize.cx = cx;
	}
	if (CW_USEDEFAULT != cy)
	{
		bCompute = true;
		curSize.cy = cy;
	}

	if (bCompute)
	{
		HMONITOR hMon = MonitorFromPoint(curPt, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi;
		mi.cbSize = sizeof(mi);
		GetMonitorInfo(hMon, &mi);
		CRect rWorkSpace(mi.rcWork);
		CRect rect(curPt, curSize);
		// Make sure window is not bigger.
		if (rect.Width() > rWorkSpace.Width())
			rect.right = rect.left + rWorkSpace.Width();
		if (rect.Height() > rWorkSpace.Height())
			rect.bottom = rect.top + rWorkSpace.Height();
		// Make sure the window is fully visible in the screen.
		if (!rWorkSpace.PtInRect(rect.TopLeft()))
		{
			if (rect.left < rWorkSpace.left)
				rect.OffsetRect(rWorkSpace.left - rect.left, 0);
			if (rect.top < rWorkSpace.top)
				rect.OffsetRect(0, rWorkSpace.top - rect.top);
		}
		// Only check the bottom-right if the rect has size.
		if (!rect.IsRectEmpty() && !rWorkSpace.PtInRect(rect.BottomRight()))
		{
			if (rect.right > rWorkSpace.right)
				rect.OffsetRect(rWorkSpace.right - rect.right, 0);
			if (rect.bottom > rWorkSpace.bottom)
				rect.OffsetRect(0, rWorkSpace.bottom - rect.bottom);
		}
		if (CW_USEDEFAULT != x)
			cs.x = rect.left;
		if (CW_USEDEFAULT != y)
			cs.y = rect.top;
		if (CW_USEDEFAULT != cx)
			cs.cx = rect.Width();
		if (CW_USEDEFAULT != cy)
			cs.cy = rect.Height();
	}

	return CFrameWnd::PreCreateWindow(cs);
}


#ifdef _DEBUG
// CMainFrame diagnostics
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}


void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


void CMainFrame::SetStatusText(
		CString const& msg,
		bool bFiltered)
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS);
	UINT nId, nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo(index, nId, nStyle, cxWidth);
	CWindowDC dc(&m_wndStatusBar);
	CFont* pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize sz = dc.GetTextExtent(msg);
	dc.SelectObject(pOldFont);
	m_wndStatusBar.SetPaneInfo(index, nId, nStyle, sz.cx);
	// Note, a cmdui handler is required to get text to display.
	m_wndStatusBar.SetPaneText(index, msg);

	CString filtered;
	if (bFiltered)
		filtered.LoadString(ID_INDICATOR_FILTERED);
	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_FILTERED);
	m_wndStatusBar.SetPaneText(index, filtered);
}


void CMainFrame::SetStatusText2(CString const& msg)
{
	int index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_DOG);
	UINT nId, nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo(index, nId, nStyle, cxWidth);
	CWindowDC dc(&m_wndStatusBar);
	CFont* pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
	CSize sz = dc.GetTextExtent(msg);
	dc.SelectObject(pOldFont);
	m_wndStatusBar.SetPaneInfo(index, nId, nStyle, sz.cx);
	m_wndStatusBar.SetPaneText(index, msg);
}


void CMainFrame::SetCurTab(int tab)
{
	m_pView->SetCurSel(tab);
}


int CMainFrame::GetCurTab() const
{
	return m_pView->GetCurSel();
}


bool CMainFrame::ShowPointsAs(bool bHtml)
{
	return m_pView->ShowPointsAs(bHtml);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	WINDOWPLACEMENT win;
	GetWindowPlacement(&win);
	int state;
	switch (win.showCmd)
	{
	default:
		state = 0;
		{
			CRect r;
			GetWindowRect(&r);
			AfxGetApp()->WriteProfileInt(_T("Settings"), _T("lastXpos"), r.left);
			AfxGetApp()->WriteProfileInt(_T("Settings"), _T("lastYpos"), r.top);
			AfxGetApp()->WriteProfileInt(_T("Settings"), _T("lastCX"), r.Width());
			AfxGetApp()->WriteProfileInt(_T("Settings"), _T("lastCY"), r.Height());
		}
		break;
	case SW_SHOWMINIMIZED:
		state = -1;
		break;
	case SW_SHOWMAXIMIZED:
		state = 1;
		break;
	}
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("lastState"), state);

	CFrameWnd::OnClose();
}


void CMainFrame::OnUpdatePane(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}


void CMainFrame::OnNextTab()
{
	if (m_pView)
	{
		int idx = m_pView->GetCurSel();
		++idx;
		if (idx == m_pView->GetItemCount())
			idx = 0;
		m_pView->SetCurSel(idx);
	}
}


void CMainFrame::OnPrevTab()
{
	if (m_pView)
	{
		int idx = m_pView->GetCurSel();
		--idx;
		if (idx == -1)
			idx = m_pView->GetItemCount() - 1;
		m_pView->SetCurSel(idx);
	}
}
