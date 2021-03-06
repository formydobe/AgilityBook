#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief interface of the CDlgTitle class
 * @author David Connet
 *
 * Revision History
 * 2011-08-17 Add support to get newly created title.
 * 2009-02-09 Ported to wxWidgets.
 * 2006-02-16 Cleaned up memory usage with smart pointers.
 */

#include "ARB/ARBTypes2.h"
#include "ARBCommon/ARBDate.h"
class CRichEditCtrl2;
class CVenueComboBox;
class wxDateEvent;
class wxDatePickerCtrl;


class CDlgTitle : public wxDialog
{
public:
	CDlgTitle(
		ARBConfig const& config,
		ARBDogTitleList& titles,
		ARBDogTitlePtr const& inTitle,
		wxWindow* pParent = nullptr);

	// Returns existing title or the newly created one.
	ARBDogTitlePtr GetNewTitle() const
	{
		return m_newTitle;
	}

private:
	ARBConfigTitlePtr GetTitleData(int index) const;
	ARBDate GetDate();
	void FillTitles(bool bIniting = false);
	void FillTitleInfo();
	short GetInstance(ARBConfigTitlePtr const& inTitle, std::vector<short>* outMissing = nullptr) const;

	ARBDogTitleList& m_Titles;
	ARBDogTitlePtr m_pTitle;
	ARBDogTitlePtr m_newTitle;
	wxDatePickerCtrl* m_ctrlDate;
	wxCheckBox* m_ctrlReceived;
	CVenueComboBox* m_ctrlVenues;
	wxCheckBox* m_ctrlHide;
	wxComboBox* m_ctrlTitles;
	CRichEditCtrl2* m_ctrlDesc;
	wxString m_Venue;
	bool m_bEarned;
	bool m_bHidden;
	bool m_bReceived;

	DECLARE_EVENT_TABLE()
	void OnTitleDateChanged(wxDateEvent& evt);
	void OnClickedEarned(wxCommandEvent& evt);
	void OnSelchangeVenues(wxCommandEvent& evt);
	void OnSelchangeTitles(wxCommandEvent& evt);
	void OnOk(wxCommandEvent& evt);
};
