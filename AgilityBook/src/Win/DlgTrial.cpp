/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief implementation of the CDlgTrial class
 * @author David Connet
 *
 * Revision History
 * @li 2012-05-22 DRC Change KillFocus handler to text change handler.
 * @li 2012-05-07 DRC Added autocompletion to combo boxes.
 * @li 2011-12-22 DRC Switch to using Bind on wx2.9+.
 * @li 2011-08-16 DRC Make trial notes multi-line.
 * @li 2009-10-14 DRC Add dog's name to dialog caption.
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-08-12 DRC Fix killfocus handling.
 * @li 2009-02-09 DRC Ported to wxWidgets.
 * @li 2008-02-01 DRC Make 'Notes' button change selection.
 * @li 2007-12-03 DRC Refresh location list after invoking 'notes' button.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 * @li 2005-12-13 DRC Added direct access to Notes dialog.
 * @li 2005-06-25 DRC Cleaned up reference counting when returning a pointer.
 * @li 2004-12-19 DRC Added Location/Club note information.
 * @li 2003-12-27 DRC Changed FindEvent to take a date.
 */

#include "stdafx.h"
#include "DlgTrial.h"

#include "AgilityBook.h"
#include "AgilityBookDoc.h"
#include "ARBDogTrial.h"
#include "ComboBoxes.h"
#include "DlgClub.h"
#include "DlgInfoNote.h"
#include "ListCtrl.h"
#include "ListData.h"
#include "NoteButton.h"
#include "RichEditCtrl2.h"
#include "Validators.h"
#include <set>
#include <wx/valgen.h>

#ifdef __WXMSW__
#include <wx/msw/msvcrt.h>
#endif

/////////////////////////////////////////////////////////////////////////////

class CListTrialData : public CListData
{
public:
	CListTrialData(ARBDogClubPtr club) : m_Club(club) {}
	virtual std::wstring OnNeedText(long iCol) const;
	ARBDogClubPtr GetClub() const	{return m_Club;}
private:
	ARBDogClubPtr	m_Club;
};
typedef std::tr1::shared_ptr<CListTrialData> CListTrialDataPtr;


std::wstring CListTrialData::OnNeedText(long iCol) const
{
	switch (iCol)
	{
	default:
		assert(0);
		return std::wstring();
	case 0:
		return m_Club->GetName();
	case 1:
		return m_Club->GetVenue();
	}
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CDlgTrial, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgTrial::OnOk)
END_EVENT_TABLE()


CDlgTrial::CDlgTrial(
		CAgilityBookDoc* pDoc,
		ARBDogTrialPtr pTrial,
		wxWindow* pParent)
	: wxDialog()
	, m_Location(StringUtil::stringWX(pTrial->GetLocation()))
	, m_Verified(pTrial->IsVerified())
	, m_Notes(StringUtil::stringWX(pTrial->GetNote()))
	, m_ctrlLocationInfo(NULL)
	, m_ctrlEdit(NULL)
	, m_ctrlDelete(NULL)
	, m_ctrlClubNotes(NULL)
	, m_pDoc(pDoc)
	, m_pTrial(pTrial)
	, m_Clubs()
	, m_bRunsDeleted(false)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP | GetExtraStyle());
	if (!pParent)
		pParent = wxGetApp().GetTopWindow();
	Create(pParent, wxID_ANY,
			StringUtil::stringWX(pDoc->AddDogToCaption(StringUtil::stringW(_("IDD_TRIAL")))),
			wxDefaultPosition, wxDefaultSize,
			wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

	pTrial->GetClubs().Clone(m_Clubs);

	// Controls (these are done first to control tab order)

	wxStaticText* textLocation = new wxStaticText(this, wxID_ANY,
		_("IDC_TRIAL_LOCATION"),
		wxDefaultPosition, wxDefaultSize, 0);
	textLocation->Wrap(-1);

	m_ctrlLocation = new CAutoFillComboBox(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize,
		0, NULL,
		wxCB_DROPDOWN|wxCB_SORT,
		CTrimValidator(&m_Location), _("IDS_ENTER_NAME"));
	BIND_OR_CONNECT_CTRL(m_ctrlLocation, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler, CDlgTrial::OnEnChangeLocation);
	BIND_OR_CONNECT_CTRL(m_ctrlLocation, wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler, CDlgTrial::OnSelchangeLocation);
	m_ctrlLocation->SetHelpText(_("HIDC_TRIAL_LOCATION"));
	m_ctrlLocation->SetToolTip(_("HIDC_TRIAL_LOCATION"));

	wxCheckBox* checkVerified = new wxCheckBox(this, wxID_ANY,
		_("IDC_TRIAL_VERIFIED"),
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_Verified));
	checkVerified->SetHelpText(_("HIDC_TRIAL_VERIFIED"));
	checkVerified->SetToolTip(_("HIDC_TRIAL_VERIFIED"));

	wxStaticText* textNotes = new wxStaticText(this, wxID_ANY,
		_("IDC_TRIAL_NOTES"),
		wxDefaultPosition, wxDefaultSize, 0);
	textNotes->Wrap(-1);

	CTextCtrl* ctrlTrialNotes = new CTextCtrl(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxSize(260, 65), wxTE_MULTILINE,
		CTrimValidator(&m_Notes, TRIMVALIDATOR_TRIM_BOTH));
	ctrlTrialNotes->SetHelpText(_("HIDC_TRIAL_NOTES"));
	ctrlTrialNotes->SetToolTip(_("HIDC_TRIAL_NOTES"));

	m_ctrlLocationNotes = new CNoteButton(this);
	BIND_OR_CONNECT_CTRL(m_ctrlLocationNotes, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgTrial::OnLocationNotes);
	m_ctrlLocationNotes->SetHelpText(_("HIDC_TRIAL_LOCATION_NOTES"));
	m_ctrlLocationNotes->SetToolTip(_("HIDC_TRIAL_LOCATION_NOTES"));

	wxStaticText* noteLocationNotes = new wxStaticText(this, wxID_ANY,
		_("IDC_TRIAL_LOCATION_INFO"),
		wxDefaultPosition, wxDefaultSize, 0);
	noteLocationNotes->Wrap(-1);

	m_ctrlLocationInfo = new CRichEditCtrl2(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, true);
	m_ctrlLocationInfo->SetHelpText(_("HIDC_TRIAL_LOCATION_INFO"));
	m_ctrlLocationInfo->SetToolTip(_("HIDC_TRIAL_LOCATION_INFO"));

	wxStaticText* textClub = new wxStaticText(this, wxID_ANY,
		_("IDC_TRIAL_CLUBS"),
		wxDefaultPosition, wxDefaultSize, 0);
	textClub->Wrap(-1);

	wxButton* btnNew = new wxButton(this, wxID_ANY,
		_("IDC_TRIAL_CLUB_NEW"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	BIND_OR_CONNECT_CTRL(btnNew, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgTrial::OnClubNew);
	btnNew->SetHelpText(_("HIDC_TRIAL_CLUB_NEW"));
	btnNew->SetToolTip(_("HIDC_TRIAL_CLUB_NEW"));

	m_ctrlEdit = new wxButton(this, wxID_ANY,
		_("IDC_TRIAL_CLUB_EDIT"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	BIND_OR_CONNECT_CTRL(m_ctrlEdit, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgTrial::OnClubEdit);
	m_ctrlEdit->SetHelpText(_("HIDC_TRIAL_CLUB_EDIT"));
	m_ctrlEdit->SetToolTip(_("HIDC_TRIAL_CLUB_EDIT"));

	m_ctrlDelete = new wxButton(this, wxID_ANY,
		_("IDC_TRIAL_CLUB_DELETE"),
		wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	BIND_OR_CONNECT_CTRL(m_ctrlDelete, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgTrial::OnClubDelete);
	m_ctrlDelete->SetHelpText(_("HIDC_TRIAL_CLUB_DELETE"));
	m_ctrlDelete->SetToolTip(_("HIDC_TRIAL_CLUB_DELETE"));

	m_ctrlClubs = new CReportListCtrl(this,
		wxDefaultPosition, wxDefaultSize,
		true, CReportListCtrl::eNoSortHeader, true);
	BIND_OR_CONNECT_CTRL(m_ctrlClubs, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler, CDlgTrial::OnItemSelectedClubs);
	BIND_OR_CONNECT_CTRL(m_ctrlClubs, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler, CDlgTrial::OnItemActivatedClubs);
	BIND_OR_CONNECT_CTRL(m_ctrlClubs, wxEVT_KEY_DOWN, wxKeyEventHandler, CDlgTrial::OnKeydownClubs);
	m_ctrlClubs->SetHelpText(_("HIDC_TRIAL_CLUBS"));
	m_ctrlClubs->SetToolTip(_("HIDC_TRIAL_CLUBS"));

	m_ctrlClubNotes = new CNoteButton(this);
	BIND_OR_CONNECT_CTRL(m_ctrlClubNotes, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler, CDlgTrial::OnClubNotes);
	m_ctrlClubNotes->SetHelpText(_("HIDC_TRIAL_CLUB_NOTES"));
	m_ctrlClubNotes->SetToolTip(_("HIDC_TRIAL_CLUB_NOTES"));

	wxStaticText* textClubNotes = new wxStaticText(this, wxID_ANY,
		_("IDC_TRIAL_CLUB_INFO"),
		wxDefaultPosition, wxDefaultSize, 0);
	textClubNotes->Wrap(-1);

	m_ctrlClubInfo = new CRichEditCtrl2(this, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, true);
	m_ctrlClubInfo->SetHelpText(_("HIDC_TRIAL_CLUB_INFO"));
	m_ctrlClubInfo->SetToolTip(_("HIDC_TRIAL_CLUB_INFO"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizerTop = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizerTop2 = new wxBoxSizer(wxHORIZONTAL);
	sizerTop2->Add(textLocation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerTop2->Add(m_ctrlLocation, 1, wxALL|wxEXPAND, 5);

	sizerTop->Add(sizerTop2, 3, wxEXPAND, 5);
	sizerTop->Add(checkVerified, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	bSizer->Add(sizerTop, 0, wxEXPAND, 5);

	wxBoxSizer* sizerNoteText = new wxBoxSizer(wxHORIZONTAL);
	sizerNoteText->Add(textNotes, 3, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* sizerLocationNotes = new wxBoxSizer(wxHORIZONTAL);
	sizerLocationNotes->Add(m_ctrlLocationNotes, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	sizerLocationNotes->Add(noteLocationNotes, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerNoteText->Add(sizerLocationNotes, 2, wxEXPAND, 5);

	bSizer->Add(sizerNoteText, 0, wxEXPAND, 5);

	wxBoxSizer* sizerNote = new wxBoxSizer(wxHORIZONTAL);
	sizerNote->Add(ctrlTrialNotes, 3, wxALL|wxEXPAND, 5);
	sizerNote->Add(m_ctrlLocationInfo, 2, wxALL|wxEXPAND, 5);

	bSizer->Add(sizerNote, 1, wxEXPAND, 5);

	wxBoxSizer* sizerClubText = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizerClub = new wxBoxSizer(wxHORIZONTAL);
	sizerClub->Add(textClub, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerClub->Add(0, 0, 1, wxEXPAND, 5);
	sizerClub->Add(btnNew, 0, wxLEFT|wxRIGHT, 5);
	sizerClub->Add(m_ctrlEdit, 0, wxLEFT|wxRIGHT, 5);
	sizerClub->Add(m_ctrlDelete, 0, wxLEFT|wxRIGHT, 5);

	sizerClubText->Add(sizerClub, 3, wxEXPAND, 5);

	wxBoxSizer* sizerClubNotes = new wxBoxSizer(wxHORIZONTAL);
	sizerClubNotes->Add(m_ctrlClubNotes, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	sizerClubNotes->Add(textClubNotes, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	sizerClubText->Add(sizerClubNotes, 2, wxEXPAND, 5);

	bSizer->Add(sizerClubText, 0, wxEXPAND, 5);

	wxBoxSizer* sizerClubs = new wxBoxSizer(wxHORIZONTAL);
	sizerClubs->Add(m_ctrlClubs, 3, wxALL|wxEXPAND, 5);
	sizerClubs->Add(m_ctrlClubInfo, 2, wxALL|wxEXPAND, 5);

	bSizer->Add(sizerClubs, 1, wxEXPAND, 5);

	wxSizer* sdbSizer = CreateSeparatedButtonSizer(wxOK|wxCANCEL);
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnParent();

	m_ctrlClubs->InsertColumn(0, _("IDS_COL_CLUB"));
	m_ctrlClubs->InsertColumn(1, _("IDS_COL_VENUE"));
	m_ctrlEdit->Enable(false);
	m_ctrlClubNotes->Enable(false);
	m_ctrlDelete->Enable(false);
	ListLocations();
	ListClubs();
	UpdateNotes(m_Location, true, true);

	IMPLEMENT_ON_INIT(CDlgTrial, m_ctrlLocation)
}


DEFINE_ON_INIT(CDlgTrial)


ARBDogClubPtr CDlgTrial::GetClubData(long index) const
{
	CListTrialDataPtr pData = std::tr1::dynamic_pointer_cast<CListTrialData, CListData>(m_ctrlClubs->GetData(index));
	if (pData)
		return pData->GetClub();
	return ARBDogClubPtr();
}


void CDlgTrial::UpdateNotes(
		wxString const& location,
		bool bLocation,
		bool bClub)
{
	if (bLocation)
	{
		std::wstring str;
		ARBInfoItemPtr pItem;
		if (m_pDoc->Book().GetInfo().GetInfo(ARBInfo::eLocationInfo).FindItem(StringUtil::stringW(location), &pItem))
		{
			str = pItem->GetComment();
		}
		m_ctrlLocationInfo->SetValue(StringUtil::stringWX(str));
	}
	if (bClub)
	{
		std::wstring str;
		ARBDogClubPtr pClub = GetClubData(m_ctrlClubs->GetFirstSelected());
		if (pClub)
		{
			ARBInfoItemPtr pItem;
			if (m_pDoc->Book().GetInfo().GetInfo(ARBInfo::eClubInfo).FindItem(pClub->GetName(), &pItem))
			{
				str = pItem->GetComment();
			}
		}
		m_ctrlClubInfo->SetValue(StringUtil::stringWX(str));
	}
}


void CDlgTrial::ListLocations()
{
	std::set<std::wstring> locations;
	m_pDoc->Book().GetAllTrialLocations(locations, true, true);
	if (!m_pTrial->GetLocation().empty())
		locations.insert(m_pTrial->GetLocation());
	std::wstring loc(m_Location);
	if (m_Location.empty())
		loc = m_pTrial->GetLocation();
	m_ctrlLocation->Clear();
	wxArrayString choices;
	for (std::set<std::wstring>::const_iterator iter = locations.begin(); iter != locations.end(); ++iter)
	{
		wxString wxName(StringUtil::stringWX(*iter));
		int index = m_ctrlLocation->Append(wxName);
		choices.Add(wxName);
		if ((*iter) == loc)
			m_ctrlLocation->SetSelection(index);
	}
	m_ctrlLocation->AutoComplete(choices);
}


void CDlgTrial::ListClubs(ARBDogClubPtr* inClub)
{
	m_ctrlClubs->DeleteAllItems();
	for (ARBDogClubList::const_iterator iter = m_Clubs.begin(); iter != m_Clubs.end(); ++iter)
	{
		ARBDogClubPtr pClub = *iter;
		CListTrialDataPtr data(new CListTrialData(pClub));
		long idx = m_ctrlClubs->InsertItem(data);
		if (inClub && *(*inClub) == *pClub)
			m_ctrlClubs->Select(idx);
	}
	if (1 == m_ctrlClubs->GetItemCount())
		m_ctrlClubs->Select(0);
	int nColumns = m_ctrlClubs->GetColumnCount();
	for (int i = 0; i < nColumns ; ++i)
		m_ctrlClubs->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
}


void CDlgTrial::EditClub()
{
	long index = m_ctrlClubs->GetFirstSelected();
	if (0 <= index)
	{
		ARBDogClubPtr pClub = GetClubData(index);
		CDlgClub dlg(m_pDoc, m_Clubs, pClub, this);
		if (wxID_OK == dlg.ShowModal())
			ListClubs(&pClub);
	}
}


void CDlgTrial::OnEnChangeLocation(wxCommandEvent& evt)
{
	wxString s = m_ctrlLocation->GetValue();
	UpdateNotes(s, true, false);
	evt.Skip();
}


void CDlgTrial::OnSelchangeLocation(wxCommandEvent& evt)
{
	TransferDataFromWindow();
	UpdateNotes(m_Location, true, false);
}


void CDlgTrial::OnItemSelectedClubs(wxListEvent& evt)
{
	TransferDataFromWindow();
	if (0 == m_ctrlClubs->GetSelectedItemCount())
	{
		m_ctrlEdit->Enable(false);
		m_ctrlClubNotes->Enable(false);
		m_ctrlDelete->Enable(false);
	}
	else
	{
		m_ctrlEdit->Enable(true);
		m_ctrlClubNotes->Enable(true);
		m_ctrlDelete->Enable(true);
	}
	UpdateNotes(m_Location, false, true);
}


void CDlgTrial::OnItemActivatedClubs(wxListEvent& evt)
{
	EditClub();
}


void CDlgTrial::OnKeydownClubs(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
	default:
		evt.Skip();
		break;
	case WXK_SPACE:
	case WXK_NUMPAD_SPACE:
		EditClub();
		break;
	}
}


void CDlgTrial::OnLocationNotes(wxCommandEvent& evt)
{
	if (TransferDataFromWindow())
	{
		CDlgInfoNote dlg(m_pDoc, ARBInfo::eLocationInfo, StringUtil::stringW(m_Location), this);
		if (wxID_OK == dlg.ShowModal())
		{
			m_Location = StringUtil::stringWX(dlg.CurrentSelection());
			TransferDataToWindow();
			ListLocations();
			UpdateNotes(m_Location, true, false);
		}
	}
}


void CDlgTrial::OnClubNotes(wxCommandEvent& evt)
{
	long index = m_ctrlClubs->GetFirstSelected();
	if (0 <= index)
	{
		TransferDataFromWindow();
		ARBDogClubPtr pClub = GetClubData(index);
		CDlgInfoNote dlg(m_pDoc, ARBInfo::eClubInfo, pClub->GetName(), this);
		if (wxID_OK == dlg.ShowModal())
		{
			if (pClub->GetName() != dlg.CurrentSelection())
			{
				pClub->SetName(dlg.CurrentSelection());
				ListClubs(&pClub);
			}
			else
				UpdateNotes(m_Location, false, true);
		}
	}
}


void CDlgTrial::OnClubNew(wxCommandEvent& evt)
{
	CDlgClub dlg(m_pDoc, m_Clubs, ARBDogClubPtr(), this);
	if (wxID_OK == dlg.ShowModal())
	{
		ARBDogClubPtr club;
		if (m_Clubs.AddClub(dlg.Club(), dlg.Venue(), &club))
			ListClubs(&club);
	}
}


void CDlgTrial::OnClubEdit(wxCommandEvent& evt)
{
	EditClub();
}


void CDlgTrial::OnClubDelete(wxCommandEvent& evt)
{
	long index = m_ctrlClubs->GetFirstSelected();
	if (0 <= index)
	{
		ARBDogClubPtr pClub = GetClubData(index);
		m_Clubs.DeleteClub(pClub->GetName(), pClub->GetVenue());
		m_ctrlClubs->DeleteItem(index);
	}
}


void CDlgTrial::OnOk(wxCommandEvent& evt)
{
	if (!Validate() || !TransferDataFromWindow())
		return;

	// Before the final commit, verify the trial still makes sense!
	// A user could have changed the venue of a club, this will make
	// any existing runs possibly invalid.
	m_bRunsDeleted = false;
	if (0 < m_pTrial->GetRuns().size())
	{
		std::set<std::wstring> oldVenues;
		std::set<std::wstring> newVenues;
		ARBDogClubList::iterator iterClub;
		for (iterClub = m_pTrial->GetClubs().begin(); iterClub != m_pTrial->GetClubs().end(); ++iterClub)
		{
			ARBDogClubPtr pClub = *iterClub;
			oldVenues.insert(pClub->GetVenue());
		}
		for (iterClub = m_Clubs.begin(); iterClub != m_Clubs.end(); ++iterClub)
		{
			ARBDogClubPtr pClub = *iterClub;
			newVenues.insert(pClub->GetVenue());
		}
		bool bAllThere = true;
		for (std::set<std::wstring>::iterator iterVenues = oldVenues.begin(); iterVenues != oldVenues.end(); ++iterVenues)
		{
			if (newVenues.end() == newVenues.find((*iterVenues)))
			{
				bAllThere = false;
				break;
			}
		}
		if (!bAllThere)
		{
			int nDelete = 0;
			for (ARBDogRunList::iterator iterRun = m_pTrial->GetRuns().begin();
			iterRun != m_pTrial->GetRuns().end();
			++iterRun)
			{
				ARBDogRunPtr pRun = *iterRun;
				bool bFound = false;
				for (std::set<std::wstring>::iterator iterVenues = newVenues.begin(); iterVenues != newVenues.end(); ++iterVenues)
				{
					if (m_pDoc->Book().GetConfig().GetVenues().FindEvent(
						(*iterVenues),
						pRun->GetEvent(),
						pRun->GetDivision(),
						pRun->GetLevel(),
						pRun->GetDate()))
					{
						bFound = true;
						break;
					}
				}
				if (!bFound)
				{
					++nDelete;
				}
			}
			if (0 < nDelete)
			{
				wxString msg = wxString::Format(_("IDS_CONFIG_DELETE_RUNS"),
					static_cast<int>(m_pTrial->GetRuns().size()));
				if (wxYES != wxMessageBox(msg, wxMessageBoxCaptionStr, wxYES_NO | wxNO_DEFAULT | wxCENTRE | wxICON_WARNING))
					return;
				m_pTrial->GetRuns().clear();
				m_bRunsDeleted = true;
			}
		}
	}

	m_pTrial->SetLocation(StringUtil::stringW(m_Location));
	m_pTrial->SetNote(StringUtil::stringW(m_Notes));
	m_pTrial->SetVerified(m_Verified);
	m_pTrial->GetClubs() = m_Clubs;

	m_pDoc->Modify(true);

	EndDialog(wxID_OK);
}
