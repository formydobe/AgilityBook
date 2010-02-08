#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See Licence.txt
 */

/**
 * @file
 *
 * @brief Configuration updater
 * @author David Connet
 *
 * Revision History
 * @li 2009-02-10 DRC Ported to wxWidgets.
 */

#include "ARBAgilityRecordBook.h"
class CTextCtrl;


class CDlgConfigUpdate : public wxDialog
{
public:
	CDlgConfigUpdate(wxWindow* parent = NULL);

	ARBConfig& GetConfig()		{return m_Book.GetConfig();}
	// This should not be called if the dialog was flown. It is intended
	// to allow loading config data without flying the dialog (for instance,
	// from the import menu item).
	// NULL will load the default configuration.
	bool LoadConfig(wxChar const* pFile);

private:
	void EnableControls();

	wxRadioButton* m_radioDefault;
	wxRadioButton* m_radioExisting;
	wxButton* m_btnPick;
	CTextCtrl* m_FileName;
	//CEdit	m_ctrlFileNameEdit;
	//CString	m_FileName;
	//CButton	m_ctrlFileName;
	ARBAgilityRecordBook m_Book;

	DECLARE_EVENT_TABLE()
	void OnUpdateDefault(wxCommandEvent& evt);
	void OnUpdateExisting(wxCommandEvent& evt);
	void OnName(wxCommandEvent& evt);
	void OnOk(wxCommandEvent& evt);
};