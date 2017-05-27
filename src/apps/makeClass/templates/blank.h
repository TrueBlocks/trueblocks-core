#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
class [{CLASS_NAME}] : public [{BASE_CLASS}]
{
public:
[FIELD_DEC]
public:
    [{CLASS_NAME}](void);
    [{CLASS_NAME}](const [{CLASS_NAME}]& [{SHORT}]);
    ~[{CLASS_NAME}](void);
    [{CLASS_NAME}]& operator=(const [{CLASS_NAME}]& [{SHORT}]);

    DECLARE_NODE ([{CLASS_NAME}]);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const [{CLASS_NAME}]& [{SHORT}]);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](void)
{
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::[{CLASS_NAME}](const [{CLASS_NAME}]& [{SHORT}])
{
    // EXISTING_CODE
    // EXISTING_CODE
    Copy([{SHORT}]);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]::~[{CLASS_NAME}](void)
{
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Clear(void)
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Init(void)
{
    [{BASE_CLASS}]::Init();

[FIELD_SET]
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void [{CLASS_NAME}]::Copy(const [{CLASS_NAME}]& [{SHORT}])
{
    Clear();
    [{BASE_CLASS}]::Copy([{SHORT}]);

[FIELD_COPY]
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline [{CLASS_NAME}]& [{CLASS_NAME}]::operator=(const [{CLASS_NAME}]& [{SHORT}])
{
    Copy([{SHORT}]);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString [{CLASS_NAME}]::getValueByName(const SFString& fieldName) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY([{CLASS_NAME}]Array);
IMPLEMENT_ARCHIVE_ARRAY_C([{CLASS_NAME}]Array);
IMPLEMENT_ARCHIVE_LIST([{CLASS_NAME}]List);

[{OPERATORS}]//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
[{NAMESPACE2}]
