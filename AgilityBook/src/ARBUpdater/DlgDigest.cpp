/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Generate a checksum for the given file.
 * @author David Connet
 *
 * Revision History
 * @li 2010-02-09 DRC Created
 */

#include "stdafx.h"
#include "DlgDigest.h"

#include "ARBMsgDigest.h"
#include "../Win/Globals.h"
#include <wx/filedlg.h>
#include <wx/valgen.h>
#include <wx/valtext.h>

/////////////////////////////////////////////////////////////////////////////

class CLongValidator : public wxValidator
{
DECLARE_CLASS(CLongValidator)
public:
	CLongValidator(long* val)
		: m_pLong(val)
	{
	}
	CLongValidator(CLongValidator const& rhs)
		: m_pLong(rhs.m_pLong)
	{
		Copy(rhs);
	}
	virtual wxObject *Clone() const {return new CLongValidator(*this);}
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate(wxWindow* parent)	{return true;}
private:
	long* m_pLong;
};


IMPLEMENT_CLASS(CLongValidator, wxValidator)


bool CLongValidator::TransferFromWindow()
{
	// Following the example of wxGenericValidator
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)))
	{
		wxTextCtrl* pControl = (wxTextCtrl*)m_validatorWindow;
		if (m_pLong)
		{
			if (!tstringUtil::ToLong(pControl->GetValue(), *m_pLong))
				return false;
			return true;
		}
	}
	return false;
}


bool CLongValidator::TransferToWindow()
{
	if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)))
	{
		wxTextCtrl* pControl = (wxTextCtrl*)m_validatorWindow;
		if (m_pLong)
		{
			wxString str;
			str.Printf(wxT("%ld"), *m_pLong);
			pControl->SetValue(str);
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CDlgDigest, wxDialog)
	EVT_BUTTON(wxID_OK, CDlgDigest::OnOk)
END_EVENT_TABLE()


CDlgDigest::CDlgDigest(wxString const& inFile)
	: m_File(inFile)
	, m_MD5()
	, m_Size(0)
{
	Create(NULL, wxID_ANY, wxT("MD5 Checksum"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	if (!m_File.empty())
	{
		wxBusyCursor wait;
		size_t size;
		m_MD5 = ARBMsgDigest::ComputeFile(m_File, &size);
		if (m_MD5.empty())
			m_File.erase();
		else
			m_Size = static_cast<long>(size);
	}

	wxTextCtrl* ctrlFile = new wxTextCtrl(this, wxID_ANY,
		wxEmptyString,
		wxDefaultPosition, wxSize(300, -1), 0,
		wxTextValidator(wxFILTER_NONE, &m_File));

	wxTextCtrl* ctrlMD5 = new wxTextCtrl(this, wxID_ANY,
		wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_READONLY,
		wxTextValidator(wxFILTER_NONE, &m_MD5));
	ctrlMD5->SetBackgroundColour(GetBackgroundColour());

	wxTextCtrl* ctrlSize = new wxTextCtrl(this, wxID_ANY,
		wxEmptyString,
		wxDefaultPosition, wxSize(100, -1), wxTE_READONLY,
		CLongValidator(&m_Size));
	ctrlSize->SetBackgroundColour(GetBackgroundColour());

	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);
	bSizer->Add(ctrlFile, 0, wxALL|wxEXPAND, 5);
	bSizer->Add(ctrlMD5, 0, wxALL|wxEXPAND, 5);
	bSizer->Add(ctrlSize, 0, wxALL, 5);

	wxSizer* sdbSizer = CreateSeparatedButtonSizer(wxOK|wxCANCEL);
	wxButton* ok = wxDynamicCast(FindWindowInSizer(sdbSizer, wxID_OK), wxButton);
	ok->SetLabel(wxT("Find..."));
	wxButton* cancel = wxDynamicCast(FindWindowInSizer(sdbSizer, wxID_CANCEL), wxButton);
	cancel->SetLabel(wxT("Close"));
	bSizer->Add(sdbSizer, 0, wxALL|wxEXPAND, 5);

	SetSizer(bSizer);
	Layout();
	GetSizer()->Fit(this);
	wxSize sz = GetSize();
	SetSizeHints(sz, wxSize(-1, sz.y));
	ok->SetFocus();
}


void CDlgDigest::OnOk(wxCommandEvent& evt)
{
	wxFileDialog dlg(NULL, wxFileSelectorPromptStr, wxEmptyString, m_File);
	if (wxID_OK == dlg.ShowModal())
	{
		wxBusyCursor wait;
		m_File = dlg.GetPath();
		size_t size;
		m_MD5 = ARBMsgDigest::ComputeFile(m_File, &size);
		m_Size = static_cast<long>(size);
		TransferDataToWindow();
	}
}