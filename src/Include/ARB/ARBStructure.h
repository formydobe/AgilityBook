#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief Defines for elements and attributes from the DTD
 * @author David Connet
 *
 * Revision History
 * @li 2013-09-03 DRC Added short name to div/level/event.
 * @li 2013-01-13 DRC Added new recurring title suffix style.
 * @li 2012-09-09 DRC Added 'titlePts','speedPts' to 'Placement'.
 * @li 2007-08-14 DRC Separated from ARBAgilityRecordBook.h
 */

/*
 * These defines make up the structure of the xml.
 * Make sure these agree with the .dtd.
 *
 * Defines that are indented are the valid values for the attribute that
 * was just defined.
 */
#define TREE_BOOK							L"AgilityBook"
#define TREE_CALENDAR						L"Calendar"
#define TREE_TRAINING						L"Training"
#define TREE_CONFIG							L"Configuration"
#define TREE_ACTION							L"Action"
#define TREE_VENUE							L"Venue"
#define TREE_VENUE_DESC						L"Desc"
#define TREE_TITLES							L"Titles"
#define TREE_MULTIQ							L"MultiQ"
#define TREE_MULTIQ_ITEM					L"MultiQItem"
#define TREE_DIVISION						L"Division"
#define TREE_LEVEL							L"Level"
#define TREE_SUBLEVEL						L"SubLevel"
#define TREE_EVENT							L"Event"
#define TREE_EVENT_DESC						L"Desc"
#define TREE_EVENT_SUBNAME					L"SubName"
#define TREE_SCORING						L"Scoring"
#define TREE_PLACE_INFO						L"PlaceInfo"
#define TREE_TITLE_POINTS					L"TitlePoints"
#define TREE_LIFETIME_POINTS				L"LifeTime"
#define TREE_PLACEMENTS						L"Placements"
#define TREE_CALSITE						L"CalSite"
#define TREE_CALSITE_DESC					L"Desc"
#define TREE_LOCCODE						L"LocCode"
#define TREE_VENUECODE						L"VenueCode"
#define TREE_FAULTTYPE						L"FaultType"
#define TREE_OTHERPTS						L"OtherPts"
#define TREE_INFO							L"Info"
#define TREE_CLUBINFO						L"ClubInfo"
#define TREE_JUDGEINFO						L"JudgeInfo"
#define TREE_LOCATIONINFO					L"LocationInfo"
#define TREE_DOG							L"Dog"
#define TREE_REGNAME						L"RegisteredName"
#define TREE_BREED							L"Breed"
#define TREE_NOTE							L"Note"
#define TREE_EXISTING_PTS					L"ExistingPoints"
#define TREE_REG_NUM						L"RegNum"
#define TREE_TITLE							L"Title"
#define TREE_TRIAL							L"Trial"
#define TREE_LOCATION						L"Location"
#define TREE_CLUB							L"Club"
#define TREE_RUN							L"Run"
#define TREE_CONDITIONS						L"Conditions"
#define TREE_JUDGE							L"Judge"
#define TREE_HANDLER						L"Handler"
#define TREE_PARTNER						L"Partner"
#define TREE_BY_TIME						L"ByTime"
#define TREE_BY_OPENCLOSE					L"ByOpenClose"
#define TREE_BY_POINTS						L"ByPoints"
#define TREE_PLACEMENT						L"Placement"
#define TREE_PLACEMENT_OTHERPOINTS			L"OtherPoints"
#define TREE_NOTES							L"Notes"
#define TREE_FAULTS							L"Faults"
#define TREE_CRCD							L"CRCD"
#define TREE_CRCD_META						L"CRCDMeta"
#define TREE_CRCD_META2						L"CRCDMeta2"
#define TREE_OTHER							L"Other"
#define TREE_RUN_LINK						L"Link"
#define TREE_REF_RUN						L"ReferenceRun"
#define TREE_REF_NAME						L"Name"
#define TREE_REF_BREED						L"Breed"
#define TREE_REF_SCORE						L"ScoreOrFaults"
#define TREE_REF_NOTE						L"Note"
#define ATTRIB_BOOK_VERSION					L"Book"
#define ATTRIB_BOOK_PGM_VERSION				L"ver"
#define ATTRIB_BOOK_PGM_PLATFORM			L"platform"
#define ATTRIB_BOOK_PGM_OS					L"os"
#define ATTRIB_BOOK_TIMESTAMP				L"timestamp"
#define ATTRIB_CONFIG_VERSION				L"version"
#define ATTRIB_CONFIG_UPDATE				L"update"
#define ATTRIB_CAL_START					L"DateStart"
#define ATTRIB_CAL_END						L"DateEnd"
#define ATTRIB_CAL_OPENING					L"DateOpening"
#define ATTRIB_CAL_DRAW						L"DateDraw"
#define ATTRIB_CAL_CLOSING					L"DateClosing"
#define ATTRIB_CAL_MAYBE					L"isTentative"
#define ATTRIB_CAL_LOCATION					L"Location"
#define ATTRIB_CAL_CLUB						L"Club"
#define ATTRIB_CAL_VENUE					L"Venue"
#define ATTRIB_CAL_ENTERED					L"Entered"
#define ATTRIB_CAL_ACCOMMODATION			L"Acc"
#define ATTRIB_CAL_CONFIRMATION				L"Confirm"
#define ATTRIB_CAL_SECEMAIL					L"SecEmail"
#define ATTRIB_CAL_PREMIUMURL				L"PremiumURL"
#define ATTRIB_CAL_ONLINEURL				L"OnlineURL"
#define ATTRIB_CAL_NOTE						L"Note"
#define ATTRIB_TRAINING_DATE				L"Date"
#define ATTRIB_TRAINING_NAME				L"Name"
#define ATTRIB_TRAINING_SUBNAME				L"SubName"
#define ATTRIB_ACTION_VERB					L"Verb"
#define ATTRIB_ACTION_VENUE					L"Venue"
#define ATTRIB_ACTION_DIVISION				L"Div"
#define ATTRIB_ACTION_OLDNAME				L"OldName"
#define ATTRIB_ACTION_NEWNAME				L"NewName"
#define ATTRIB_ACTION_CONFIG				L"Config"
#define ATTRIB_VENUE_NAME					L"Name"
#define ATTRIB_VENUE_LONGNAME				L"LongName"
#define ATTRIB_VENUE_URL					L"URL"
#define ATTRIB_VENUE_ICON					L"icon"
#define ATTRIB_VENUE_LIFETIME_NAME			L"LifetimeName"
#define ATTRIB_MULTIQ_NAME					L"Name"
#define ATTRIB_MULTIQ_SHORTNAME				L"SName"
#define ATTRIB_MULTIQ_VALID_FROM			L"ValidFrom"
#define ATTRIB_MULTIQ_VALID_TO				L"ValidTo"
#define ATTRIB_MULTIQ_ITEM_DIV				L"Div"
#define ATTRIB_MULTIQ_ITEM_LEVEL			L"Level"
#define ATTRIB_MULTIQ_ITEM_EVENT			L"Event"
#define ATTRIB_CALSITE_NAME					L"name"
#define ATTRIB_CALSITE_SEARCH				L"search"
#define ATTRIB_CALSITE_HELP					L"help"
#define ATTRIB_LOCCODE_CODE					L"code"
#define ATTRIB_LOCCODE_NAME					L"name"
#define ATTRIB_VENUECODE_CODE				L"code"
#define ATTRIB_VENUECODE_VENUE				L"venue"
#define ATTRIB_OTHERPTS_NAME				L"Name"
#define ATTRIB_OTHERPTS_COUNT				L"Count"
#define ATTRIB_OTHERPTS_DEFAULT				L"defValue"
#define ATTRIB_DIVISION_NAME				L"Name"
#define ATTRIB_DIVISION_SHORTNAME			L"ShortName"
#define ATTRIB_LEVEL_NAME					L"Name"
#define ATTRIB_LEVEL_SHORTNAME				L"ShortName"
#define ATTRIB_TITLES_NAME					L"Name"
#define ATTRIB_TITLES_LONGNAME				L"LongName"
#define ATTRIB_TITLES_PREFIX				L"Prefix"
#define ATTRIB_TITLES_VALIDFROM				L"ValidFrom"
#define ATTRIB_TITLES_VALIDTO				L"ValidTo"
#define ATTRIB_TITLES_MULTIPLE_STARTAT		L"StartAt"
#define ATTRIB_TITLES_MULTIPLE_INC			L"Inc"
#define ATTRIB_TITLES_MULTIPLE_ONFIRST		L"OnFirst"
#define ATTRIB_TITLES_MULTIPLE_STYLE		L"Style"
#define ATTRIB_TITLES_MULTIPLE_SEP			L"Sep"
#define ATTRIB_SUBLEVEL_NAME				L"Name"
#define ATTRIB_SUBLEVEL_SHORTNAME			L"ShortName"
#define ATTRIB_EVENT_NAME					L"Name"
#define ATTRIB_EVENT_SHORTNAME				L"ShortName"
#define ATTRIB_EVENT_HAS_TABLE				L"hasTable"
#define ATTRIB_EVENT_HASPARTNER				L"hasPartner"
#define ATTRIB_EVENT_HASSUBNAMES			L"hasSubNames"
#define ATTRIB_SCORING_VALIDFROM			L"ValidFrom"
#define ATTRIB_SCORING_VALIDTO				L"ValidTo"
#define ATTRIB_SCORING_DIVISION				L"Division"
#define ATTRIB_SCORING_LEVEL				L"Level"
#define ATTRIB_SCORING_TYPE					L"type"
#define ATTRIB_SCORING_DROPFRACTIONS		L"dropFractions"
#define ATTRIB_SCORING_TIMEFAULTS_CLEAN_Q	L"cleanQ"
#define ATTRIB_SCORING_TIMEFAULTS_UNDER		L"underTF"
#define ATTRIB_SCORING_TIMEFAULTS_OVER		L"overTF"
#define ATTRIB_SCORING_SUBTRACT_TIMEFAULTS	L"subtractTF"
#define ATTRIB_SCORING_OPENINGPTS			L"OpeningPts"
#define ATTRIB_SCORING_CLOSINGPTS			L"ClosingPts"
#define ATTRIB_SCORING_SUPERQ				L"superQ"
#define ATTRIB_SCORING_SPEEDPTS				L"speedPts"
#define ATTRIB_SCORING_BONUSPTS				L"bonusPts"
#define ATTRIB_SCORING_OBSTACLES			L"obstacles"
#define ATTRIB_PLACE_INFO_PLACE				L"Place"
#define ATTRIB_PLACE_INFO_VALUE				L"Value"
#define ATTRIB_PLACE_INFO_MUSTQ				L"MustQ"
#define ATTRIB_TITLE_POINTS_POINTS			L"Points"
#define ATTRIB_TITLE_POINTS_FAULTS			L"Faults"
#define ATTRIB_TITLE_POINTS_TYPE			L"Type"
#define ATTRIB_TITLE_POINTS_TYPE_NORMAL			L"Norm"
#define ATTRIB_TITLE_POINTS_TYPE_T2B			L"T2B"
#define ATTRIB_TITLE_POINTS_TYPE_UKI			L"UKI"
#define ATTRIB_LIFETIME_POINTS_POINTS		L"Points"
#define ATTRIB_LIFETIME_POINTS_FAULTS		L"Faults"
#define ATTRIB_INFO_NAME					L"Name"
#define ATTRIB_INFO_VISIBLE					L"Visible"
#define ATTRIB_DOG_CALLNAME					L"CallName"
#define ATTRIB_DOG_DOB						L"DOB"
#define ATTRIB_DOG_DECEASED					L"Deceased"
#define ATTRIB_EXISTING_PTS_DATE			L"Date"
#define ATTRIB_EXISTING_PTS_TYPE			L"Type"
#define ATTRIB_EXISTING_PTS_OTHER			L"Other"
#define ATTRIB_EXISTING_PTS_VENUE			L"Venue"
#define ATTRIB_EXISTING_PTS_MULTIQ			L"MultiQ"
#define ATTRIB_EXISTING_PTS_DIV				L"Div"
#define ATTRIB_EXISTING_PTS_LEVEL			L"Level"
#define ATTRIB_EXISTING_PTS_EVENT			L"Event"
#define ATTRIB_EXISTING_PTS_SUBNAME			L"SubName"
#define ATTRIB_EXISTING_PTS_POINTS			L"Pts"
#define ATTRIB_REG_NUM_VENUE				L"Venue"
#define ATTRIB_REG_NUM_NUMBER				L"Number"
#define ATTRIB_REG_NUM_HEIGHT				L"Height"
#define ATTRIB_REG_NUM_RECEIVED				L"isReceived"
#define ATTRIB_TITLE_VENUE					L"Venue"
#define ATTRIB_TITLE_NAME					L"Name"
#define ATTRIB_TITLE_DATE					L"Date"
#define ATTRIB_TITLE_INSTANCE				L"instance"
#define ATTRIB_TITLE_INSTANCE_STARTAT		L"startat"
#define ATTRIB_TITLE_INSTANCE_INC			L"inc"
#define ATTRIB_TITLE_INSTANCE_SHOW			L"show"
#define ATTRIB_TITLE_INSTANCE_STYLE			L"style"
#define ATTRIB_TITLE_INSTANCE_SEP			L"sep"
#define ATTRIB_TITLE_RECEIVED				L"isReceived"
#define ATTRIB_TITLE_HIDDEN					L"isHidden"
#define ATTRIB_TRIAL_DEFAULT_DATE			L"Date"
#define ATTRIB_TRIAL_VERIFIED				L"Verified"
#define ATTRIB_CLUB_VENUE					L"Venue"
#define ATTRIB_RUN_DATE						L"Date"
#define ATTRIB_RUN_DIVISION					L"Division"
#define ATTRIB_RUN_LEVEL					L"Level"
#define ATTRIB_RUN_HEIGHT					L"Height"
#define ATTRIB_RUN_EVENT					L"Event"
#define ATTRIB_RUN_SUBNAME					L"SubName"
#define ATTRIB_PARTNER_HANDLER				L"Handler"
#define ATTRIB_PARTNER_DOG					L"Dog"
#define ATTRIB_PARTNER_REGNUM				L"RegNum"
#define ATTRIB_SCORING_FAULTS				L"CourseFaults"
#define ATTRIB_SCORING_TIME					L"Time"
#define ATTRIB_SCORING_HAS_TABLE			L"hasTable"
#define ATTRIB_SCORING_TF_MULTIPLIER		L"timeFault"
#define ATTRIB_SCORING_SCT					L"SCT"
#define ATTRIB_SCORING_SCT2					L"SCT2"
#define ATTRIB_BY_TIME_YARDS				L"Yards"
#define ATTRIB_BY_OPENCLOSE_NEEDOPEN		L"NeedOpenPts"
#define ATTRIB_BY_OPENCLOSE_NEEDCLOSE		L"NeedClosePts"
#define ATTRIB_BY_OPENCLOSE_GOTOPEN			L"OpenPts"
#define ATTRIB_BY_OPENCLOSE_GOTCLOSE		L"ClosePts"
#define ATTRIB_BY_POINTS_NEED				L"NeedPts"
#define ATTRIB_BY_POINTS_GOT				L"Points"
#define ATTRIB_PLACEMENT_Q					L"Q"
#define ATTRIB_QTYPE_NA							L"NA"
#define ATTRIB_QTYPE_Q							L"Q"
#define ATTRIB_QTYPE_NQ							L"NQ"
#define ATTRIB_QTYPE_E							L"E"
#define ATTRIB_QTYPE_DNR						L"DNR"
#define ATTRIB_QTYPE_SQ							L"SQ"
#define ATTRIB_PLACEMENT_PLACE				L"Place"
#define ATTRIB_PLACEMENT_INCLASS			L"InClass"
#define ATTRIB_PLACEMENT_DOGSQD				L"DogsQd"
#define ATTRIB_PLACEMENT_SPEED_POINTS		L"speedPts"
#define ATTRIB_PLACEMENT_TITLE_POINTS		L"titlePts"
#define ATTRIB_PLACEMENT_OTHERPOINTS_NAME	L"Name"
#define ATTRIB_PLACEMENT_OTHERPOINTS_POINTS	L"Points"
#define ATTRIB_REF_RUN_Q					L"Q"
#define ATTRIB_REF_RUN_PLACE				L"Place"
#define ATTRIB_REF_RUN_TIME					L"Time"
#define ATTRIB_REF_RUN_HEIGHT				L"Height"

/*
 * These defines list any special values.
 */
#define WILDCARD_DIVISION		L"*"
#define WILDCARD_LEVEL			L"*"