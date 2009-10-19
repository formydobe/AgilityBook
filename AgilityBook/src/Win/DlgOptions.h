#pragma once

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
 * @brief interface of the CDlgOptions class
 * @author David Connet
 *
 * Revision History
 * @li 2009-02-09 DRC Ported to wxWidgets.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-08-18 DRC Separated options and filters into two dialogs.
 */

class CAgilityBookDoc;
class CDlgOptionsCalendar;
class CDlgOptionsFilter;
class CDlgOptionsPrint;
class CDlgOptionsProgram;


class CDlgOptions : public wxDialog
{
public:
	static int GetProgramPage()		{return 0;}
	static int GetFilterPage()		{return 1;}
	static int GetCalendarPage()	{return 2;}
	static int GetPrintPage()		{return 3;}

	CDlgOptions(
			CAgilityBookDoc* pDoc,
			wxWindow* pParentWnd = NULL,
			size_t iSelectPage = 0);

protected:
	CAgilityBookDoc* m_pDoc;
	CDlgOptionsProgram* m_pageProgram;
	CDlgOptionsFilter* m_pageFilter;
	CDlgOptionsCalendar* m_pageCal;
	CDlgOptionsPrint* m_pagePrint;

	DECLARE_EVENT_TABLE()
	void OnOk(wxCommandEvent& evt);
};