#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Spreadsheet interface
 * @author David Connet
 *
 * Revision History
 * @li 2012-02-18 Added eSpreadSheetNumberNoZero
 * @li 2012-02-05 Added alignment and formatting.
 * @li 2009-07-24 DRC Removed option to export by array.
 * @li 2009-02-11 DRC Ported to wxWidgets.
 * @li 2004-09-30 DRC Created
 */

#if defined(__WXMSW__) && wxUSE_OLE_AUTOMATION
#define HAS_AUTOMATION	1
#else
#define HAS_AUTOMATION	0
#endif

#include "ARBTypes.h"
#include <string>
#include <vector>
class IDlgProgress;
ARB_TYPEDEF(ISpreadSheet)
ARB_TYPEDEF(ISpreadSheetExporter)
ARB_TYPEDEF(ISpreadSheetImporter)


/**
 * Interface for exporting to a spreadsheet.
 */
class ISpreadSheetExporter
{
public:
	virtual ~ISpreadSheetExporter() = 0;

	virtual wchar_t GetSumIfSeparator() const = 0;

	virtual bool AllowAccess(bool bAllow) = 0;

	virtual bool SetTextColor(
			long inRow,
			long inCol,
			wxColour inColor) = 0;
	virtual bool SetBackColor(
			long inRow,
			long inCol,
			wxColour inColor) = 0;
	virtual bool SetItalic(
			long inRow,
			long inCol,
			bool bItalic) = 0;
	virtual bool SetBold(
			long inRow,
			long inCol,
			bool bBold) = 0;

	enum eAlign
	{
		eSpreadSheetNone,
		eSpreadSheetGeneral,
		eSpreadSheetLeft,
		eSpreadSheetRight,
		eSpreadSheetCenter,
	};
	virtual bool SetAlignment(
			long inRow,
			long inCol,
			eAlign align) = 0;
	enum eFormat
	{
		eSpreadSheetText,			///< General text
		eSpreadSheetCurrency,		///< Currency, "$  (redneg)", 0 == "-"
		eSpreadSheetNumber,			///< 0dec number, 1000sep, (redneg)
		eSpreadSheetNumberNoZero,	///< 0dec number, 1000sep, (redneg), 0 == ""
		eSpreadSheetDate,			///< m/d/yyyy
	};
	virtual bool SetFormat(
			long inRow,
			long inCol,
			eFormat format) = 0;
	// Direct formatting
	virtual bool SetFormat(
			long inRow,
			long inCol,
			std::wstring const& format) = 0;

	virtual bool InsertData(
			long inRow,
			long inCol,
			double inData) = 0;
	virtual bool InsertData(
			long inRow,
			long inCol,
			std::wstring const& inData,
			bool bFormula = false) = 0;

	virtual bool AutoFit(
			long inColFrom,
			long inColTo) = 0;
};

/**
 * Interface for importing from a spreadsheet.
 */
class ISpreadSheetImporter
{
public:
	virtual ~ISpreadSheetImporter() = 0;
	virtual bool OpenFile(std::wstring const& inFilename) = 0;
	virtual bool GetData(
			std::vector< std::vector<std::wstring> >& outData,
			IDlgProgress* ioProgress = NULL) = 0;
};

/////////////////////////////////////////////////////////////////////////////

class ISpreadSheet
{
public:
	typedef enum
	{
		eMicrosoftExcel,
		eOpenOfficeCalc
	} eType;
	/**
	 * Create a new spreadsheet manager, must 'delete' the returned pointer.
	 */
	static ISpreadSheetPtr Create(eType inType);

	/// Get the maximum number of rows Excel can handle.
	static long GetMaxRows();
	/// Get the maximum number of columns Excel can handle.
	static long GetMaxCols();
	/// Translate a given row/col into Excel notation: (0,0) -> "A1"
	static bool GetRowCol(
			long inRow,
			long inCol,
			std::wstring& outCell);

	virtual ~ISpreadSheet() = 0;
	virtual ISpreadSheetExporterPtr GetExporter() const = 0;
	virtual ISpreadSheetImporterPtr GetImporter() const = 0;
};