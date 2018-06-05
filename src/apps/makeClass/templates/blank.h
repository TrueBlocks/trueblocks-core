#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
[H_INCLUDES]
[{NAMESPACE1}]
//--------------------------------------------------------------------------
class [{CLASS_NAME}];
typedef SFArrayBase<[{CLASS_NAME}]>         [{CLASS_NAME}]Array;
typedef SFList<[{CLASS_NAME}]*>             [{CLASS_NAME}]List;
typedef SFUniqueList<[{CLASS_NAME}]*>       [{CLASS_NAME}]ListU;

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
    friend ostream& operator<<(ostream& os, const [{CLASS_NAME}]& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const [{CLASS_NAME}]& [{SHORT}]);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](const [{CLASS_NAME}]& [{SHORT}]) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy([{SHORT}]);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::~[{CLASS_NAME}](void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Clear(void) {
[FIELD_CLEAR]    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Init(void) {
    [{BASE_CLASS}]::Init();

[FIELD_SET]
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Copy(const [{CLASS_NAME}]& [{SHORT}]) {
    Clear();
    [{BASE_CLASS}]::Copy([{SHORT}]);

[FIELD_COPY]
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]& [{CLASS_NAME}]::operator=(const [{CLASS_NAME}]& [{SHORT}]) {
    Copy([{SHORT}]);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY([{CLASS_NAME}]Array);
IMPLEMENT_ARCHIVE_ARRAY_C([{CLASS_NAME}]Array);
IMPLEMENT_ARCHIVE_LIST([{CLASS_NAME}]List);

[{OPERATORS}]//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

[{NAMESPACE2}]
