#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Various validators.
 * @author David Connet
 *
 * Revision History
 * 2018-10-11 Moved to Win LibARBWin
 * 2012-08-13 Moved CQualifyingValidator to separate file.
 * 2012-01-02 Change validator to support default value on empty field.
 * 2009-02-15 Created
 */

#include "ARBCommon/ARBTypes.h"
#include <wx/valgen.h>
#include <wx/valtext.h>
class ARBDate;
class wxDateTime;


/// wxGenericValidator doesn't do shorts and doubles
// Would have preferred to inherit from wxGenericValidator, but there's
// no ctor I can use (it has no default one).
class CGenericValidator : public wxValidator
{
DECLARE_CLASS(CGenericValidator)
public:
	CGenericValidator(
			unsigned short* val,
			unsigned short defVal = 0,
			bool bUseDefOnEmpty = true,
			wxChar const* errMsg = nullptr);
	CGenericValidator(
			short* val,
			short defVal = 0,
			bool bUseDefOnEmpty = true,
			wxChar const* errMsg = nullptr);
	CGenericValidator(
			long* val,
			long defVal = 0,
			bool bUseDefOnEmpty = true,
			wxChar const* errMsg = nullptr);
	CGenericValidator(
			double* val,
			int inPrec = 2,
			double defVal = 0.0,
			bool bUseDefOnEmpty = true,
			wxChar const* errMsg = nullptr);
	CGenericValidator(
			ARBDate* val,
			wxChar const* errMsg = nullptr);
	CGenericValidator(
			wxDateTime* val,
			wxChar const* errMsg = nullptr);
	CGenericValidator(CGenericValidator const& rhs);

	virtual wxObject *Clone() const {return new CGenericValidator(*this);}
	bool Copy(CGenericValidator const& val);

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate(wxWindow* parent);

private:
	unsigned short* m_pUShort;
	short* m_pShort;
	long* m_pLong;
	double* m_pDouble;
	int m_Prec;
	union
	{
		unsigned short us;
		short s;
		long l;
		double dbl;
	} m_Default;
	bool m_bUseDefOnEmpty;
	ARBDate* m_pDate;
	wxDateTime* m_pTime;
	wxString m_ErrMsg;
};


#define TRIMVALIDATOR_NONE			0x0000
#define TRIMVALIDATOR_TRIM_LEFT		0x0001
#define TRIMVALIDATOR_TRIM_RIGHT	0x0002
#define TRIMVALIDATOR_TRIM_BOTH		(TRIMVALIDATOR_TRIM_LEFT | TRIMVALIDATOR_TRIM_RIGHT)
#define TRIMVALIDATOR_NONEMPTY		0x0004
#define TRIMVALIDATOR_DEFAULT		(TRIMVALIDATOR_TRIM_BOTH | TRIMVALIDATOR_NONEMPTY)


class CTrimValidator : public wxGenericValidator
{
DECLARE_CLASS(CTrimValidator)
public:
	CTrimValidator(
			wxString* valPtr = nullptr,
			long trimStyle = TRIMVALIDATOR_DEFAULT,
			wxChar const* errMsg = nullptr); // Message to use when validation fails
	CTrimValidator(CTrimValidator const& rhs);

	virtual wxObject *Clone() const {return new CTrimValidator(*this);}
	bool Copy(CTrimValidator const& val);

	virtual bool TransferFromWindow();
	virtual bool Validate(wxWindow* parent);

protected:
	long m_TrimStyle;
	wxString m_ErrMsg;
};