/*
 * Copyright � 2002-2009 David Connet. All Rights Reserved.
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
 * @brief implementation of the CDlgPartner class
 * @author David Connet
 *
 * Revision History
 * @li 2009-02-11 DRC Ported to wxWidgets.
 * @li 2006-02-16 DRC Cleaned up memory usage with smart pointers.
 */

#include "stdafx.h"
#include "DlgPartner.h"

#include "ARBDogRunPartner.h"
#include "Validators.h"


BEGIN_EVENT_TABLE(CDlgPartner, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgPartner::OnOk)
END_EVENT_TABLE()


CDlgPartner::CDlgPartner(
		ARBDogRunPartnerPtr partner,
		std::set<tstring> const& inHandlers,
		std::set<tstring> const& inDogs,
		wxWindow* pParent)
	: wxDialog(pParent, wxID_ANY, _("IDD_PARTNER"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
	, m_Handler(partner->GetHandler().c_str())
	, m_Dog(partner->GetDog().c_str())
	, m_RegNum(partner->GetRegNum().c_str())
	, m_Partner(partner)
{
	SetExtraStyle(wxDIALOG_EX_CONTEXTHELP);

	wxArrayString handlers;
	std::set<tstring>::const_iterator iter;
	for (iter = inHandlers.begin(); iter != inHandlers.end(); ++iter)
	{
		handlers.Add((*iter).c_str());
	}
	handlers.Sort();

	wxArrayString dogs;
	for (iter = inDogs.begin(); iter != inDogs.end(); ++iter)
	{
		dogs.Add((*iter).c_str());
	}
	dogs.Sort();

	// Controls (these are done first to control tab order)

	wxStaticText* textHandler = new wxStaticText(this, wxID_ANY,
		_("IDC_PARTNER_NAME"),
		wxDefaultPosition, wxDefaultSize, 0);
	textHandler->Wrap(-1);

	wxComboBox* ctrlHandler = new wxComboBox(this, wxID_ANY, m_Handler,
		wxDefaultPosition, wxDefaultSize,
		handlers, wxCB_DROPDOWN|wxCB_SORT,
		CTrimValidator(&m_Handler, TRIMVALIDATOR_TRIM_BOTH));
	ctrlHandler->SetHelpText(_("HIDC_PARTNER_NAME"));
	ctrlHandler->SetToolTip(_("HIDC_PARTNER_NAME"));

	wxStaticText* textDog = new wxStaticText(this, wxID_ANY,
		_("IDC_PARTNER_CALLNAME"),
		wxDefaultPosition, wxDefaultSize, 0);
	textDog->Wrap(-1);

	wxComboBox* ctrlDog = new wxComboBox(this, wxID_ANY, m_Dog,
		wxDefaultPosition, wxDefaultSize,
		dogs, wxCB_DROPDOWN|wxCB_SORT,
		CTrimValidator(&m_Dog, TRIMVALIDATOR_TRIM_BOTH));
	ctrlDog->SetHelpText(_("HIDC_PARTNER_CALLNAME"));
	ctrlDog->SetToolTip(_("HIDC_PARTNER_CALLNAME"));

	wxStaticText* textNumber = new wxStaticText(this, wxID_ANY,
		_("IDC_PARTNER_REG_NUM"),
		wxDefaultPosition, wxDefaultSize, 0);
	textNumber->Wrap(-1);

	wxTextCtrl* ctrlNumber = new wxTextCtrl(this, wxID_ANY, m_RegNum,
		wxDefaultPosition, wxDefaultSize, 0,
		wxGenericValidator(&m_RegNum));
	ctrlNumber->SetHelpText(_("HIDC_PARTNER_REG_NUM"));
	ctrlNumber->SetToolTip(_("HIDC_PARTNER_REG_NUM"));

	// Sizers (sizer creation is in same order as wxFormBuilder)

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
	sizer1->Add(textHandler, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizer1->Add(ctrlHandler, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);

	bSizer->Add(sizer1, 0, wxEXPAND, 5);

	wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
	sizer2->Add(textDog, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizer2->Add(ctrlDog, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);

	bSizer->Add(sizer2, 0, wxEXPAND, 5);

	wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
	sizer3->Add(textNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sizer3->Add(ctrlNumber, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);

	bSizer->Add(sizer3, 1, wxEXPAND, 5);

	wxSizer* sdbSizer = CreateSeparatedButtonSizer(wxOK|wxCANCEL);
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	wxSize sz(GetSize());
	SetSizeHints(sz, wxSize(-1, sz.y));
	CenterOnParent();

	ctrlHandler->SetFocus();
}


void CDlgPartner::OnOk(wxCommandEvent& evt)
{
	if (!Validate() || !TransferDataFromWindow())
		return;

	m_Partner->SetHandler(m_Handler.c_str());
	m_Partner->SetDog(m_Dog.c_str());
	m_Partner->SetRegNum(m_RegNum.c_str());
	EndDialog(wxID_OK);
}
