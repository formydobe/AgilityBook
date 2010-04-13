/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief implementation of the CDlgConfigPlaceInfo class
 * @author David Connet
 *
 * Revision History
 * @li 2009-02-11 DRC Ported to wxWidgets.
 * @li 2006-11-04 DRC Created.
 */

#include "stdafx.h"
#include "DlgConfigPlaceInfo.h"

#include "AgilityBook.h"
#include "Validators.h"
#include "Widgets.h"
#include <algorithm>
#include <wx/valgen.h>


/////////////////////////////////////////////////////////////////////////////

class CPlaceValidator : public CGenericValidator
{
public:
	CPlaceValidator(
			std::vector<short> const& existingPlaces,
			short* val)
		: CGenericValidator(val)
		, m_ExistingPlaces(existingPlaces)
		, m_origPlace(*val)
	{
	}
	virtual wxObject *Clone() const {return new CPlaceValidator(*this);}

	virtual bool Validate(wxWindow* parent);

private:
	std::vector<short> const& m_ExistingPlaces;
	short m_origPlace;
};


bool CPlaceValidator::Validate(wxWindow* parent)
{
	// We know...
	CTextCtrl* pControl = (CTextCtrl*)m_validatorWindow;
	short val = static_cast<short>(wxAtol(pControl->GetValue()));

	if (val != m_origPlace)
	{
		if (m_ExistingPlaces.end() != std::find(m_ExistingPlaces.begin(), m_ExistingPlaces.end(), val))
		{
			wxMessageBox(_("IDS_CONFIG_PLACE_UNIQUE"), _("Validation conflict"), wxICON_WARNING, parent);
			return false;
		}
	}
	if (0 > val)
	{
		wxMessageBox(_("IDS_CONFIG_PLACE_TOOSMALL"), _("Validation conflict"), wxICON_WARNING, parent);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

class CValueValidator : public CGenericValidator
{
public:
	CValueValidator(double* val)
		: CGenericValidator(val, 0)
		, m_origValue(*val)
	{
	}
	virtual wxObject *Clone() const {return new CValueValidator(*this);}

	virtual bool Validate(wxWindow* parent);

private:
	double m_origValue;
};


bool CValueValidator::Validate(wxWindow* parent)
{
	// We know...
	CTextCtrl* pControl = (CTextCtrl*)m_validatorWindow;
	double val = tstringUtil::ToDouble(pControl->GetValue(), val);

	if (0.0 > val)
	{
		// Note: In Win-version, this was a yesno question. But the text
		// doesn't support that. Not sure why it was yesno...
		wxMessageBox(_("IDS_CONFIG_CHECK_VALUE"), _("Validation conflict"), wxICON_WARNING, parent);
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////

CDlgConfigPlaceInfo::CDlgConfigPlaceInfo(
		std::vector<short> const& inExistingPlaces,
		short inPlace,
		double inValue,
		wxWindow* pParent)
	: wxDialog()
	, m_Place(inPlace)
	, m_Value(inValue)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP);
	if (!pParent)
		pParent = wxGetApp().GetTopWindow();
	Create(pParent, wxID_ANY, _("IDD_CONFIG_PLACEINFO"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

	// Controls (these are done first to control tab order)

	wxStaticText* textNote = new wxStaticText(this, wxID_ANY,
		_("IDD_CONFIG_PLACEINFO_TEXT"),
		wxDefaultPosition, wxDefaultSize, 0);
	textNote->Wrap(200);

	wxStaticText* textPlace = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_PLACEINFO_PLACE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textPlace->Wrap(-1);

	CTextCtrl* ctrlPlace = new CTextCtrl(this, wxID_ANY,
		wxEmptyString,
		wxDefaultPosition, wxDefaultSize, 0,
		CPlaceValidator(inExistingPlaces, &m_Place));
	ctrlPlace->SetHelpText(_("HIDC_CONFIG_PLACEINFO_PLACE"));
	ctrlPlace->SetToolTip(_("HIDC_CONFIG_PLACEINFO_PLACE"));

	wxStaticText* textValue = new wxStaticText(this, wxID_ANY,
		_("IDC_CONFIG_PLACEINFO_VALUE"),
		wxDefaultPosition, wxDefaultSize, 0);
	textValue->Wrap(-1);

	CTextCtrl* ctrlValue = new CTextCtrl(this, wxID_ANY,
		wxEmptyString,
		wxDefaultPosition, wxDefaultSize, 0,
		CValueValidator(&m_Value));
	ctrlValue->SetHelpText(_("HIDC_CONFIG_PLACEINFO_VALUE"));
	ctrlValue->SetToolTip(_("HIDC_CONFIG_PLACEINFO_VALUE"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);
	bSizer->Add(textNote, 1, wxALL|wxEXPAND, 5);

	wxBoxSizer* sizerPlace = new wxBoxSizer(wxHORIZONTAL);
	sizerPlace->Add(textPlace, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerPlace->Add(ctrlPlace, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	bSizer->Add(sizerPlace, 0, wxEXPAND, 5);

	wxBoxSizer* sizerValue = new wxBoxSizer(wxHORIZONTAL);
	sizerValue->Add(textValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizerValue->Add(ctrlValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	bSizer->Add(sizerValue, 0, wxEXPAND, 5);

	wxSizer* sdbSizer = CreateSeparatedButtonSizer(wxOK|wxCANCEL);
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	SetSizeHints(GetSize(), wxDefaultSize);
	CenterOnParent();

	textPlace->SetFocus();
}