#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 *
 * @brief Implementation of IARBLocalization
 * @author David Connet
 *
 * Revision History
 * @li 2009-09-13 DRC Add support for wxWidgets 2.9, deprecate tstring.
 * @li 2009-01-06 DRC Ported to wxWidgets.
 * @li 2007-10-11 DRC Created
 */

#include "ARB/ARBLocalization.h"


class CLocalization : public IARBLocalization
{
public:
	CLocalization();
	bool Load();

	std::wstring UnknownVersion() const				{return m_UnknownVersion;}
	std::wstring WarningNewerDoc() const			{return m_WarningNewerDoc;}
	std::wstring InvalidDocStructure() const		{return m_InvalidDocStructure;}
	std::wstring InvalidFileFormat() const			{return m_InvalidFileFormat;}
	std::wstring InvalidFileMissingAttrib() const	{return m_InvalidFileMissingAttrib;}
	std::wstring InvalidFileBadAttrib() const		{return m_InvalidFileBadAttrib;}
	std::wstring InvalidDate() const				{return m_InvalidDate;}
	std::wstring InvalidDivLevel() const			{return m_InvalidDivLevel;}
	std::wstring InvalidEventName() const			{return m_InvalidEventName;}
	std::wstring InvalidRoot() const				{return m_InvalidRoot;}
	std::wstring MissingConfig() const				{return m_MissingConfig;}
	std::wstring InvalidConfig() const				{return m_InvalidConfig;}
	std::wstring InvalidVenueConfig() const			{return m_InvalidVenueConfig;}
	std::wstring InvalidDivName() const				{return m_InvalidDivName;}
	std::wstring InvalidVenueName() const			{return m_InvalidVenueName;}
	std::wstring InvalidOtherPtsName() const		{return m_InvalidOtherPtsName;}
	std::wstring InvalidMultiqName() const			{return m_InvalidMultiqName;}
	std::wstring InvalidMultiqConversion() const	{return m_InvalidMultiqConversion;}
	std::wstring InvalidEvent() const				{return m_InvalidEvent;}
	std::wstring InvalidTitle() const				{return m_InvalidTitle;}
	std::wstring ValidValues() const				{return m_ValidValues;}
	std::wstring ValidValuesBool() const			{return m_ValidValuesBool;}
	std::wstring CalendarTentative() const			{return m_CalendarTentative;}
	std::wstring CalendarPast() const				{return m_CalendarPast;}
	std::wstring CalendarNotEntered() const			{return m_CalendarNotEntered;}
	std::wstring CalendarEntered() const			{return m_CalendarEntered;}
	std::wstring CalendarPending() const			{return m_CalendarPending;}
	std::wstring CalendarPlanning() const			{return m_CalendarPlanning;}
	std::wstring CalendarStatusN() const			{return m_CalendarStatusN;}
	std::wstring CalendarStatusE() const			{return m_CalendarStatusE;}
	std::wstring CalendarStatusO() const			{return m_CalendarStatusO;}
	std::wstring CalendarStatusP() const			{return m_CalendarStatusP;}
	std::wstring CalendarOpens() const				{return m_CalendarOpens;}
	std::wstring CalendarDraw() const				{return m_CalendarDraw;}
	std::wstring CalendarCloses() const				{return m_CalendarCloses;}

	std::wstring UpdateCalSites(int nNew, int nUpdated, int nSkipped) const;
	std::wstring UpdateFaults(int nNew, int nSkipped) const;
	std::wstring UpdateOtherPts(int nNew, int nUpdated, int nSkipped) const;
	std::wstring UpdateVenues(int nNew, int nUpdated, int nSkipped) const;
	std::wstring UpdateDivisions(int nAdded, int nUpdated, int nSkipped) const;
	std::wstring UpdateDivisionsReordered() const;
	std::wstring UpdateEvents(int nAdded, int nUpdated, int nSkipped) const;
	std::wstring UpdateEventsReordered() const;
	std::wstring UpdateMultiqs(int nAdded, int nDeleted, int nSkipped) const;
	std::wstring UpdateMultiqsReordered() const;
	std::wstring UpdateLevels(int nAdded, int nUpdated, int nSkipped) const;
	std::wstring UpdateLevelsReordered() const;
	std::wstring UpdateTitles(int nAdded, int nUpdated, int nSkipped) const;
	std::wstring UpdateTitlesReordered() const;
	std::wstring UpdateSubLevels(int nAdded, int nUpdated, int nSkipped) const;
	std::wstring UpdateSubLevelsReordered() const;
	std::wstring UpdateRules(int nAdded, int nDeleted, int nUpdated, int nSkipped) const;
	std::wstring WarnDeletedRuns(int nRuns, std::wstring const& inRunsMsg) const;
	std::wstring UpdateTeamRuns(int nRuns, std::wstring const& inRunsMsg) const;
	std::wstring UpdateTableRuns(int nRuns) const;

	std::wstring ActionDeleteCalPlugin(std::wstring const& name) const;
	std::wstring ActionRenameOtherPoints(
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteOtherPoints(
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteOtherPoints(std::wstring const& name) const;
	std::wstring ActionRenameVenue(
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteVenue(
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteVenue(std::wstring const& name) const;
	std::wstring ActionRenameMultiQ(
			std::wstring const& venue,
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteMultiQ(
			std::wstring const& venue,
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteMultiQ(
			std::wstring const& venue,
			std::wstring const& name) const;
	std::wstring ActionRenameDivision(
			std::wstring const& venue,
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteDivision(
			std::wstring const& venue,
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteDivision(
			std::wstring const& venue,
			std::wstring const& name) const;
	std::wstring ActionRenameLevel(
			std::wstring const& venue,
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteLevel(
			std::wstring const& venue,
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteLevel(
			std::wstring const& venue,
			std::wstring const& name) const;
	std::wstring ActionRenameTitle(
			std::wstring const& venue,
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteTitle(
			std::wstring const& venue,
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteTitle(
			std::wstring const& venue,
			std::wstring const& name) const;
	std::wstring ActionRenameEvent(
			std::wstring const& venue,
			std::wstring const& oldName,
			std::wstring const& newName,
			int nChanges) const;
	std::wstring ActionPreDeleteEvent(
			std::wstring const& venue,
			std::wstring const& name,
			int nChanges) const;
	std::wstring ActionDeleteEvent(
			std::wstring const& venue,
			std::wstring const& name) const;

	std::wstring TitlePointsNameFormat(double points, double faults) const;
	std::wstring LifetimePointsNameFormat(double points, double faults) const;
	std::wstring PlacementPointsNameFormat(double points, short place) const;

	std::wstring ScoreStyleUnknown() const			{return m_ScoreStyleUnknown;}
	std::wstring ScoreStyleFaultsTime() const		{return m_ScoreStyleFaultsTime;}
	std::wstring ScoreStyleFaults100Time() const	{return m_ScoreStyleFaults100Time;}
	std::wstring ScoreStyleFaults200Time() const	{return m_ScoreStyleFaults200Time;}
	std::wstring ScoreStyleOCScoreTime() const		{return m_ScoreStyleOCScoreTime;}
	std::wstring ScoreStyleScoreTime() const		{return m_ScoreStyleScoreTime;}
	std::wstring ScoreStyleTimePlusFaults() const	{return m_ScoreStyleTimePlusFaults;}

	std::wstring ExistingPointsOther() const		{return m_ExistingPointsOther;}
	std::wstring ExistingPointsRun() const			{return m_ExistingPointsRun;}
	std::wstring ExistingPointsSpeed() const		{return m_ExistingPointsSpeed;}
	std::wstring ExistingPointsMQ() const			{return m_ExistingPointsMQ;}
	std::wstring ExistingPointsSQ() const			{return m_ExistingPointsSQ;}

	std::wstring ErrorInvalidDocStructure(wchar_t const* const inMsg) const;
	std::wstring ErrorMissingAttribute(
			wchar_t const* const inElement,
			wchar_t const* const inAttrib,
			wchar_t const* const inMsg = NULL) const;
	std::wstring ErrorInvalidAttributeValue(
			wchar_t const* const inElement,
			wchar_t const* const inAttrib,
			wchar_t const* const inMsg = NULL) const;

private:
	std::wstring m_UnknownVersion;
	std::wstring m_WarningNewerDoc;
	std::wstring m_InvalidDocStructure;
	std::wstring m_InvalidFileFormat;
	std::wstring m_InvalidFileMissingAttrib;
	std::wstring m_InvalidFileBadAttrib;
	std::wstring m_InvalidDate;
	std::wstring m_InvalidDivLevel;
	std::wstring m_InvalidEventName;
	std::wstring m_InvalidRoot;
	std::wstring m_MissingConfig;
	std::wstring m_InvalidConfig;
	std::wstring m_InvalidVenueConfig;
	std::wstring m_InvalidDivName;
	std::wstring m_InvalidVenueName;
	std::wstring m_InvalidOtherPtsName;
	std::wstring m_InvalidMultiqName;
	std::wstring m_InvalidMultiqConversion;
	std::wstring m_InvalidEvent;
	std::wstring m_InvalidTitle;
	std::wstring m_ValidValues;
	std::wstring m_ValidValuesBool;
	std::wstring m_CalendarTentative;
	std::wstring m_CalendarPast;
	std::wstring m_CalendarNotEntered;
	std::wstring m_CalendarEntered;
	std::wstring m_CalendarPending;
	std::wstring m_CalendarPlanning;
	std::wstring m_CalendarStatusN;
	std::wstring m_CalendarStatusE;
	std::wstring m_CalendarStatusO;
	std::wstring m_CalendarStatusP;
	std::wstring m_CalendarOpens;
	std::wstring m_CalendarDraw;
	std::wstring m_CalendarCloses;

	std::wstring m_ScoreStyleUnknown;
	std::wstring m_ScoreStyleFaultsTime;
	std::wstring m_ScoreStyleFaults100Time;
	std::wstring m_ScoreStyleFaults200Time;
	std::wstring m_ScoreStyleOCScoreTime;
	std::wstring m_ScoreStyleScoreTime;
	std::wstring m_ScoreStyleTimePlusFaults;

	std::wstring m_QTypeNA;
	std::wstring m_QTypeQ;
	std::wstring m_QTypeNQ;
	std::wstring m_QTypeE;
	std::wstring m_QTypeSQ;

	std::wstring m_ExistingPointsOther;
	std::wstring m_ExistingPointsRun;
	std::wstring m_ExistingPointsSpeed;
	std::wstring m_ExistingPointsMQ;
	std::wstring m_ExistingPointsSQ;
};
