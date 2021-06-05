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
// clang-format off
[H_INCLUDES]
[{NAMESPACE1}]

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class [{CLASS_NAME}] : public [{BASE_CLASS}] {
  public:
[FIELD_DEC]
  public:
    [{CLASS_NAME}](void);
    [{CLASS_NAME}](const [{CLASS_NAME}]& [{SHORT}]);
    ~[{CLASS_NAME}](void);
    [{CLASS_NAME}]& operator=(const [{CLASS_NAME}]& [{SHORT}]);

    DECLARE_NODE([{CLASS_NAME}]);

[{GET_OBJ}][{GET_STR}]    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const [{CLASS_NAME}]& it) const;
    bool operator!=(const [{CLASS_NAME}]& it) const {
        return !operator==(it);
    }
    friend bool operator<(const [{CLASS_NAME}]& v1, const [{CLASS_NAME}]& v2);
    friend ostream& operator<<(ostream& os, const [{CLASS_NAME}]& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const [{CLASS_NAME}]& [{SHORT}]);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](const [{CLASS_NAME}]& [{SHORT}]) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate([{SHORT}]);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::~[{CLASS_NAME}](void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::clear(void) {
[FIELD_CLEAR]    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::initialize(void) {
    [{BASE_CLASS}]::initialize();

[INIT_DEFAULTS]
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::duplicate(const [{CLASS_NAME}]& [{SHORT}]) {
    clear();
    [{BASE_CLASS}]::duplicate([{SHORT}]);

[FIELD_COPY]
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]& [{CLASS_NAME}]::operator=(const [{CLASS_NAME}]& [{SHORT}]) {
    duplicate([{SHORT}]);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool [{CLASS_NAME}]::operator==(const [{CLASS_NAME}]& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // [{EQUAL_COMMENT}]
    return [{EQUAL_CODE}];
}

//-------------------------------------------------------------------------
inline bool operator<(const [{CLASS_NAME}]& v1, const [{CLASS_NAME}]& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // [{SORT_COMMENT}]
    return [{SORT_CODE}];
}

//---------------------------------------------------------------------------
typedef vector<[{CLASS_NAME}]> [{CLASS_NAME}]Array;
extern CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]Array& array);
extern CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]Array& array);

[OPERATORS_DECL]//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_[{CLASS_UPPER}];

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

[{NAMESPACE2}]
// clang-format on
