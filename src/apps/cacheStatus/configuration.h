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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "configfile.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CConfiguration : public CBaseNode {
  public:
    CConfigFileArray files;

  public:
    CConfiguration(void);
    CConfiguration(const CConfiguration& co);
    virtual ~CConfiguration(void);
    CConfiguration& operator=(const CConfiguration& co);

    DECLARE_NODE(CConfiguration);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CConfiguration& it) const;
    bool operator!=(const CConfiguration& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CConfiguration& v1, const CConfiguration& v2);
    friend ostream& operator<<(ostream& os, const CConfiguration& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CConfiguration& co);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CConfiguration::CConfiguration(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfiguration::CConfiguration(const CConfiguration& co) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(co);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CConfiguration::~CConfiguration(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfiguration::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfiguration::initialize(void) {
    CBaseNode::initialize();

    files.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CConfiguration::duplicate(const CConfiguration& co) {
    clear();
    CBaseNode::duplicate(co);

    files = co.files;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CConfiguration& CConfiguration::operator=(const CConfiguration& co) {
    duplicate(co);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CConfiguration::operator==(const CConfiguration& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CConfiguration& v1, const CConfiguration& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CConfiguration> CConfigurationArray;
extern CArchive& operator>>(CArchive& archive, CConfigurationArray& array);
extern CArchive& operator<<(CArchive& archive, const CConfigurationArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CONFIGURATION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
