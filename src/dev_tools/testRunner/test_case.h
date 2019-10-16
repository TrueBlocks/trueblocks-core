#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTestCase : public CBaseNode {
public:
    string_q origLine;
    string_q onOff;
    string_q mode;
    string_q speed;
    string_q route;
    string_q tool;
    string_q filename;
    string_q post;
    string_q options;
    string_q optTool;
    string_q path;
    string_q goldPath;
    string_q workPath;
    string_q fileName;

public:
    CTestCase(void);
    CTestCase(const CTestCase& te);
    virtual ~CTestCase(void);
    CTestCase& operator=(const CTestCase& te);

    DECLARE_NODE(CTestCase);

    // EXISTING_CODE
    CTestCase(const string_q& line);
    void prepareTest(bool cmdLine);
    // EXISTING_CODE
    bool operator==(const CTestCase& item) const;
    bool operator!=(const CTestCase& item) const { return !operator==(item); }
    friend bool operator<(const CTestCase& v1, const CTestCase& v2);
    friend ostream& operator<<(ostream& os, const CTestCase& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTestCase& te);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTestCase::CTestCase(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTestCase::CTestCase(const CTestCase& te) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(te);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTestCase::~CTestCase(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTestCase::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTestCase::initialize(void) {
    CBaseNode::initialize();

    origLine = "";
    onOff = "";
    mode = "";
    speed = "";
    route = "";
    tool = "";
    filename = "";
    post = "";
    options = "";
    optTool = "";
    path = "";
    goldPath = "";
    workPath = "";
    fileName = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTestCase::duplicate(const CTestCase& te) {
    clear();
    CBaseNode::duplicate(te);

    origLine = te.origLine;
    onOff = te.onOff;
    mode = te.mode;
    speed = te.speed;
    route = te.route;
    tool = te.tool;
    filename = te.filename;
    post = te.post;
    options = te.options;
    optTool = te.optTool;
    path = te.path;
    goldPath = te.goldPath;
    workPath = te.workPath;
    fileName = te.fileName;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTestCase& CTestCase::operator=(const CTestCase& te) {
    duplicate(te);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTestCase::operator==(const CTestCase& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTestCase& v1, const CTestCase& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTestCase> CTestCaseArray;
extern CArchive& operator>>(CArchive& archive, CTestCaseArray& array);
extern CArchive& operator<<(CArchive& archive, const CTestCaseArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TESTCASE;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

