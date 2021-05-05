#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "acctlib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTestCase : public CBaseNode {
  public:
    string_q origLine;
    bool builtin;
    string_q onOff;
    string_q mode;
    string_q speed;
    string_q route;
    string_q tool;
    string_q name;
    string_q post;
    string_q options;
    string_q extra;
    string_q path;
    string_q goldPath;
    string_q workPath;
    string_q fileName;
    uint32_t test_id;

  public:
    CTestCase(void);
    CTestCase(const CTestCase& te);
    virtual ~CTestCase(void);
    CTestCase& operator=(const CTestCase& te);

    DECLARE_NODE(CTestCase);

    // EXISTING_CODE
    explicit CTestCase(const string_q& line, uint32_t id);
    void prepareTest(bool cmdLine, bool removeWorking);
    // EXISTING_CODE
    bool operator==(const CTestCase& it) const;
    bool operator!=(const CTestCase& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTestCase& v1, const CTestCase& v2);
    friend ostream& operator<<(ostream& os, const CTestCase& it);

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
    builtin = false;
    onOff = "";
    mode = "";
    speed = "";
    route = "";
    tool = "";
    name = "";
    post = "";
    options = "";
    extra = "";
    path = "";
    goldPath = "";
    workPath = "";
    fileName = "";
    test_id = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTestCase::duplicate(const CTestCase& te) {
    clear();
    CBaseNode::duplicate(te);

    origLine = te.origLine;
    builtin = te.builtin;
    onOff = te.onOff;
    mode = te.mode;
    speed = te.speed;
    route = te.route;
    tool = te.tool;
    name = te.name;
    post = te.post;
    options = te.options;
    extra = te.extra;
    path = te.path;
    goldPath = te.goldPath;
    workPath = te.workPath;
    fileName = te.fileName;
    test_id = te.test_id;

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
inline bool CTestCase::operator==(const CTestCase& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return (route % it.route && tool % it.tool && name % it.name);
}

//-------------------------------------------------------------------------
inline bool operator<(const CTestCase& v1, const CTestCase& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.test_id < v2.test_id;
}

//---------------------------------------------------------------------------
typedef vector<CTestCase> CTestCaseArray;
extern CArchive& operator>>(CArchive& archive, CTestCaseArray& array);
extern CArchive& operator<<(CArchive& archive, const CTestCaseArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TESTCASE;

//---------------------------------------------------------------------------
// EXISTING_CODE
extern void establishTestMonitors(void);
// EXISTING_CODE
}  // namespace qblocks
