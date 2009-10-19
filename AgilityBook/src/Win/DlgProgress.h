#pragma once

/*
 * Copyright (c) 2004-2009 David Connet. All Rights Reserved.
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
 * @brief Progress dialog
 * @author David Connet
 *
 * This is simply an interface in case I want to push it down into the ARB
 * code at some point. If that happens, this interface declaration will move
 * and the factory method will remain behind.
 *
 * Revision History
 * @li 2009-02-11 DRC Ported to wxWidgets.
 * @li 2004-10-01 DRC Created
 */

class IDlgProgress
{
public:
	static IDlgProgress* CreateProgress(short nBars = 1, wxWindow* parent = NULL);

	// Setup dialog.
	/// Set the caption of the dialog.
	virtual void SetCaption(wxString const& inCaption) = 0;
	/// Set a visible message.
	virtual void SetMessage(wxString const& inMessage) = 0;

	// Progress bar interface (these are thin wrappers on the progress bar)
	virtual void SetRange(
			short inBar,
			int inRange) = 0;
	virtual void SetStep(
			short inBar,
			int inStep) = 0;
	virtual void StepIt(short inBar) = 0;
	virtual void OffsetPos(
			short inBar,
			int inDelta) = 0;
	virtual void SetPos(
			short inBar,
			int inPos) = 0;
	virtual int GetPos(short inBar) = 0;
	virtual void EnableCancel(bool bEnable = true) = 0;
	virtual bool HasCanceled() const = 0;

	/// Show/hide the dialog.
	virtual void ShowProgress(bool bShow = true) = 0;
	/// Force dialog to have focus
	virtual void SetForegroundWindow() = 0;
	/// Shut down (delete) the dialog.
	virtual void Dismiss() = 0;

protected:
	virtual ~IDlgProgress();
};