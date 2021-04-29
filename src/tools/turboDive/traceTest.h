#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceTest : public CBaseNode {
  public:
    string_q enabled;
    string_q number;
    string_q method;
    string_q name;
    string_q params;
    string_q fileName;

  public:
    CTraceTest(void);
    CTraceTest(const CTraceTest& tr);
    virtual ~CTraceTest(void);
    CTraceTest& operator=(const CTraceTest& tr);

    DECLARE_NODE(CTraceTest);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTraceTest& it) const;
    bool operator!=(const CTraceTest& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTraceTest& v1, const CTraceTest& v2);
    friend ostream& operator<<(ostream& os, const CTraceTest& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceTest& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceTest::CTraceTest(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceTest::CTraceTest(const CTraceTest& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceTest::~CTraceTest(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceTest::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceTest::initialize(void) {
    CBaseNode::initialize();

    enabled = "";
    number = "";
    method = "";
    name = "";
    params = "";
    fileName = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceTest::duplicate(const CTraceTest& tr) {
    clear();
    CBaseNode::duplicate(tr);

    enabled = tr.enabled;
    number = tr.number;
    method = tr.method;
    name = tr.name;
    params = tr.params;
    fileName = tr.fileName;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceTest& CTraceTest::operator=(const CTraceTest& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceTest::operator==(const CTraceTest& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTraceTest& v1, const CTraceTest& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTraceTest> CTraceTestArray;
extern CArchive& operator>>(CArchive& archive, CTraceTestArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceTestArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACETEST;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
