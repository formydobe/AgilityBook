#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief interface of the CLanguageManager class
 * @author David Connet
 *
 * Revision History
 * @li 2013-08-18 DRC Reworked for easier reuse.
 * @li 2009-01-01 DRC Ported to wxWidgets.
 * @li 2007-09-22 DRC Created
 */

class wxLocale;


class ILanguageCallback
{
public:
	virtual int OnGetLanguage() const = 0;
	virtual wxString OnGetCatalogName() const = 0;
	virtual wxString OnGetLangConfigName() const = 0;
	virtual wxString OnGetLanguageDir() const = 0;
	virtual void OnErrorMessage(wxString const& msg) const = 0;
};


class CLanguageManager
{
public:
	// Must use callback to enable catalogs.
	CLanguageManager(ILanguageCallback* pCallback);
	~CLanguageManager();

	bool InitLocale();

	std::wstring CurrentLanguage() const		{return m_dirLoadedLang;}
	int CurrentLanguageId() const				{return m_CurLang;}
	int GetDefaultLanguage() const;
	wxString GetDefaultCatalogName() const;
	wxString GetDefaultLanguageDir() const;

	bool SelectLanguage(wxWindow* parent = NULL);
	int SelectLang(wxWindow* parent = NULL);
	bool SetLang(int langId);

private:
	ILanguageCallback* m_pCallback;
	int m_CurLang;
	wxString m_dirLang; /// Where the en/fr/etc directories are located
	std::wstring m_dirLoadedLang; /// 'en'/'fr' etc
	wxLocale* m_locale;
};
