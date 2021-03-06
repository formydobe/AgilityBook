#pragma once

/*
 * Copyright (c) David Connet. All Rights Reserved.
 *
 * License: See License.txt
 */

/**
 * @file
 * @brief interface of the CNamedColumns class
 * @author David Connet
 *
 * Revision History
 * 2020-04-24 Moved defines from DlgAssignColumns to here.
 * 2010-03-28 Created.
 */

#include "AgilityBookOptions.h"
#include <vector>

/*
 * If types/columns are added/removed, do not change these numbers.
 * These numbers are stored in a users registry for the order.
 * If a column is removed, then the number must be reserved.
 * (types are stored as a key name "colN", the others are the value of the key)
 */
#define IO_TYPE_RUNS_FAULTS_TIME   0
#define IO_TYPE_RUNS_TIME_FAULTS   1
#define IO_TYPE_RUNS_OPEN_CLOSE    2
#define IO_TYPE_RUNS_POINTS        3
#define IO_TYPE_CALENDAR           4
#define IO_TYPE_TRAINING           5
#define IO_TYPE_VIEW_TREE_DOG      6 // Tree/Run and TreeList
#define IO_TYPE_VIEW_TREE_TRIAL    7
#define IO_TYPE_VIEW_TREE_RUN      8
#define IO_TYPE_VIEW_RUNS_LIST     9
#define IO_TYPE_VIEW_CALENDAR_LIST 10
#define IO_TYPE_VIEW_TRAINING_LIST 11
#define IO_TYPE_CALENDAR_APPT      12
#define IO_TYPE_CALENDAR_TASK      13
#define IO_TYPE_VIEW_CALENDAR      14
#define IO_TYPE_MAX                15

#define IO_RUNS_REG_NAME           0
#define IO_RUNS_CALL_NAME          1
#define IO_RUNS_DATE               2
#define IO_RUNS_VENUE              3
#define IO_RUNS_CLUB               4
#define IO_RUNS_LOCATION           5
#define IO_RUNS_TRIAL_NOTES        6
#define IO_RUNS_DIVISION           7
#define IO_RUNS_LEVEL              8
#define IO_RUNS_EVENT              9
#define IO_RUNS_HEIGHT             10
#define IO_RUNS_JUDGE              11
#define IO_RUNS_HANDLER            12
#define IO_RUNS_CONDITIONS         13
#define IO_RUNS_COURSE_FAULTS      14
#define IO_RUNS_TIME               15
#define IO_RUNS_YARDS              16
#define IO_RUNS_YPS                17
#define IO_RUNS_SCT                18
#define IO_RUNS_TOTAL_FAULTS       19
#define IO_RUNS_REQ_OPENING        20
#define IO_RUNS_REQ_CLOSING        21
#define IO_RUNS_OPENING            22
#define IO_RUNS_CLOSING            23
#define IO_RUNS_REQ_POINTS         24
#define IO_RUNS_POINTS             25
#define IO_RUNS_PLACE              26
#define IO_RUNS_IN_CLASS           27
#define IO_RUNS_DOGSQD             28
#define IO_RUNS_Q                  29
#define IO_RUNS_SCORE              30
#define IO_RUNS_TITLE_POINTS       31
#define IO_RUNS_COMMENTS           32
#define IO_RUNS_FAULTS             33
#define IO_CAL_START_DATE          34
#define IO_CAL_END_DATE            35
#define IO_CAL_TENTATIVE           36
#define IO_CAL_ENTERED             37
#define IO_CAL_LOCATION            38
#define IO_CAL_CLUB                39
#define IO_CAL_VENUE               40
#define IO_CAL_OPENS               41
#define IO_CAL_CLOSES              42
#define IO_CAL_NOTES               43
#define IO_LOG_DATE                44
#define IO_LOG_NAME                45
#define IO_LOG_NOTES               46
#define IO_TREE_DOG_REGNAME        47
#define IO_TREE_DOG_CALLNAME       48
#define IO_TREE_DOG_BREED          49
#define IO_TREE_DOG_DOB            50
#define IO_TREE_DOG_AGE            51
#define IO_TREE_TRIAL_START        52
#define IO_TREE_TRIAL_END          53
#define IO_TREE_TRIAL_CLUB         54
#define IO_TREE_TRIAL_VENUE        55
#define IO_TREE_TRIAL_LOCATION     56
#define IO_TREE_TRIAL_NOTES        57
#define IO_TREE_RUN_DATE           58
#define IO_TREE_RUN_Q              59
#define IO_TREE_RUN_EVENT          60
#define IO_TREE_RUN_DIVISION       61
#define IO_TREE_RUN_LEVEL          62
#define IO_TREE_RUN_HEIGHT         63
#define IO_LOG_SUBNAME             64
#define IO_TREE_TRIAL_VERIFIED     65
#define IO_RUNS_SPEED              66
#define IO_CAL_APPT_SUBJECT        67
#define IO_CAL_APPT_START_DATE     68
#define IO_CAL_APPT_START_TIME     69
#define IO_CAL_APPT_END_DATE       70
#define IO_CAL_APPT_END_TIME       71
#define IO_CAL_APPT_ALLDAY         72
#define IO_CAL_APPT_REMINDER       73
#define IO_CAL_APPT_REMINDER_DATE  74
#define IO_CAL_APPT_REMINDER_TIME  75
#define IO_CAL_APPT_ORGANIZER      76
#define IO_CAL_APPT_REQ_ATTENDEES  77
#define IO_CAL_APPT_OPT_ATTENDEES  78
#define IO_CAL_APPT_RESOURCES      79
#define IO_CAL_APPT_BILLING        80
#define IO_CAL_APPT_CATEGORIES     81
#define IO_CAL_APPT_DESCRIPTION    82
#define IO_CAL_APPT_LOCATION       83
#define IO_CAL_APPT_MILEAGE        84
#define IO_CAL_APPT_PRIORITY       85
#define IO_CAL_APPT_PRIVATE        86
#define IO_CAL_APPT_SENSITIVITY    87
#define IO_CAL_APPT_SHOW_TIME_AS   88
#define IO_CAL_TASK_SUBJECT        89
#define IO_CAL_TASK_START_DATE     90
#define IO_CAL_TASK_DUE_DATE       91
#define IO_CAL_TASK_REMINDER       92
#define IO_CAL_TASK_REMINDER_DATE  93
#define IO_CAL_TASK_REMINDER_TIME  94
#define IO_CAL_TASK_COMPLETED_DATE 95
#define IO_CAL_TASK_COMPLETE       96
#define IO_CAL_TASK_TOTAL_WORK     97
#define IO_CAL_TASK_ACTUAL_WORK    98
#define IO_CAL_TASK_BILLING        99
#define IO_CAL_TASK_CATEGORIES     100
#define IO_CAL_TASK_COMPANIES      101
#define IO_CAL_TASK_CONTACTS       102
#define IO_CAL_TASK_MILEAGE        103
#define IO_CAL_TASK_NOTES          104
#define IO_CAL_TASK_PRIORITY       105
#define IO_CAL_TASK_PRIVATE        106
#define IO_CAL_TASK_ROLE           107
#define IO_CAL_TASK_SCH_PRIORITY   108
#define IO_CAL_TASK_SENSITIVITY    109
#define IO_CAL_TASK_STATUS         110
#define IO_RUNS_MIN_YPS            111
#define IO_CAL_DRAWS               112
#define IO_RUNS_OBSTACLES          113
#define IO_RUNS_OPS                114
#define IO_RUNS_SUBNAME            115
#define IO_MAX                     116


class CNamedColumns
{
private:
	CAgilityBookOptions::ColumnOrder m_eOrder;
	std::vector<long> m_Columns[IO_TYPE_MAX];

	int m_numConfigs;
	std::wstring m_curConfig;
	struct CNamedColumnsData
	{
		std::wstring configName;
		std::vector<long> m_Columns[IO_TYPE_MAX];
	};
	std::vector<CNamedColumnsData> m_Configs;

	std::vector<CNamedColumnsData>::iterator FindConfig(std::wstring const& inName);

public:
	CNamedColumns(CAgilityBookOptions::ColumnOrder eOrder);
	void Save();

	CAgilityBookOptions::ColumnOrder Order() const
	{
		return m_eOrder;
	}
	std::wstring GetCurrentConfig() const
	{
		return m_curConfig;
	}
	std::vector<long> const& Columns(int idx) const
	{
		return m_Columns[idx];
	}

	size_t GetAllConfigNames(std::vector<std::wstring>& outNames) const;
	bool SetCurrentConfig(std::wstring const& inName);
	bool AddConfig(std::wstring const& inName); ///< Returns if a new one was added
	bool DeleteConfig(std::wstring const& inName);

	void ResetDefault(int idxType);
	void SetColumn(int idxType, std::vector<long> const& columns);
};
