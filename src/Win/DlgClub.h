#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief interface of the CDlgClub class
 * @author David Connet
 *
 * Revision History
 * 2012-05-07 Added autocompletion to combo boxes.
 * 2009-02-11 Ported to wxWidgets.
 * 2008-02-01 Only modify existing club, don't add a new one (caller does).
 * 2006-02-16 Cleaned up memory usage with smart pointers.
 */

#include "ARB/ARBTypes2.h"
class CAgilityBookDoc;
class CDlgClubData;
class CVenueComboBox;


class CDlgClub : public wxDialog
{
public:
	CDlgClub(
		CAgilityBookDoc* pDoc,
		ARBDogClubList& inClubs,
		ARBDogClubPtr const& inClub,
		bool bShowCoSanction,
		wxWindow* pParent = nullptr);

	ARBDogClubPtr AddClub(ARBDogClubList& clubs) const;

private:
	CDlgClubData* GetClubData(int index) const;
	void SetPrimaryClub(ARBDogClubPtr pClub) const;

	CAgilityBookDoc* m_pDoc;
	ARBDogClubPtr m_pClub;
	wxComboBox* m_ctrlClubs;
	CVenueComboBox* m_ctrlVenues;
	wxComboBox* m_ctrlPrimary;
	wxString m_Club;
	wxString m_Venue;
	bool m_bCoSanction;

	DECLARE_ON_INIT()
	DECLARE_EVENT_TABLE()
	void OnClickedPrimary(wxCommandEvent& evt);
	void OnOk(wxCommandEvent& evt);
};
