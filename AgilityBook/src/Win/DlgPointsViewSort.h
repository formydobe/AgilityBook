#pragma once

/*
 * Copyright (c) 2006-2009 David Connet. All Rights Reserved.
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
 * @brief interface of the CDlgPointsViewSort class
 * @author David Connet
 *
 * Revision History
 * @li 2009-02-09 DRC Ported to wxWidgets.
 * @li 2006-07-16 DRC Created
 */

#include "AgilityBookOptions.h"


class CDlgPointsViewSort : public wxDialog
{
public:
	CDlgPointsViewSort(wxWindow* pParent = NULL);

private:
	wxComboBox* m_ctrlPrimary;
	wxComboBox* m_ctrlSecondary;
	wxComboBox* m_ctrlTertiary;
	wxButton* m_ctrlOk;

	CAgilityBookOptions::PointsViewSort m_Primary;
	CAgilityBookOptions::PointsViewSort m_Secondary;
	CAgilityBookOptions::PointsViewSort m_Tertiary;

protected:
	void UpdateControls();
	void FillPrimary();
	void FillSecondary();
	void FillTertiary();

	DECLARE_EVENT_TABLE()
	void OnSelchangePrimary(wxCommandEvent& evt);
	void OnSelchangeSecondary(wxCommandEvent& evt);
	void OnSelchangeTertiary(wxCommandEvent& evt);
	void OnOk(wxCommandEvent& evt);
};