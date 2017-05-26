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
#include "abi.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextAbiChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextAbiChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CAbi::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    if (!m_showing)
        return;

    if (fmtIn.empty())
    {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAbiChunk(const SFString& fieldIn, bool& force, const void *data)
{
    const CAbi *abi = (const CAbi *)data;
    if (abi)
    {
        // Give customized code a chance to override first
        SFString ret = nextAbiChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0]))
        {
            case 'a':
                if ( fieldIn % "abiByName" )
                {
                    uint32_t cnt = abi->abiByName.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i=0;i<cnt;i++)
                    {
                        ret += abi->abiByName[i].Format();
                        ret += ((i<cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                if ( fieldIn % "abiByEncoding" )
                {
                    uint32_t cnt = abi->abiByEncoding.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i=0;i<cnt;i++)
                    {
                        ret += abi->abiByEncoding[i].Format();
                        ret += ((i<cnt-1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, abi);
        if (!ret.empty())
            return ret;
    }

    return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0]))
    {
        case 'a':
            if ( fieldName % "abiByName" ) return true;
            if ( fieldName % "abiByEncoding" ) return true;
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAbi::finishParse()
{
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Serialize(SFArchive& archive)
{
    if (!archive.isReading())
        return ((const CAbi*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> abiByName;
    archive >> abiByEncoding;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(SFArchive& archive) const
{
    if (!preSerializeC(archive))
        return false;

    archive << abiByName;
    archive << abiByEncoding;

    return true;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void)
{
    static bool been_here=false;
    if (been_here) return;
    been_here=true;

    uint32_t fieldNum=1000;
    ADD_FIELD(CAbi, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CAbi, "abiByName", T_TEXT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAbi, "abiByEncoding", T_TEXT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAbiChunk_custom(const SFString& fieldIn, bool& force, const void *data)
{
    const CAbi *abi = (const CAbi *)data;
    if (abi)
    {
        switch (tolower(fieldIn[0]))
        {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn,force,abi);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CAbi::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAbi::readBackLevel(SFArchive& archive)
{
    bool done=false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
int sortFuncTableByName(const void *ob1, const void *ob2)
{
    CFunction *p1 = (CFunction*)ob1;
    CFunction *p2 = (CFunction*)ob2;
    return p2->name.compare(p1->name);
}

//---------------------------------------------------------------------------
int sortFuncTableByEncoding(const void *ob1, const void *ob2)
{
    CFunction *p1 = (CFunction*)ob1;
    CFunction *p2 = (CFunction*)ob2;
    return p2->encoding.compare(p1->encoding);
}

//---------------------------------------------------------------------------
int findByName(const void *rr1, const void *rr2)
{
    CFunction *f1 = (CFunction *)rr1;
    CFunction *f2 = (CFunction *)rr2;
    return f2->name.compare(f1->name);
}

//---------------------------------------------------------------------------
int findByEncoding(const void *rr1, const void *rr2)
{
    CFunction *f1 = (CFunction *)rr1;
    CFunction *f2 = (CFunction *)rr2;
    return f2->encoding.compare(f1->encoding);
}

//---------------------------------------------------------------------------
CFunction *CAbi::findFunctionByName(const SFString& name)
{
    CFunction search;
    search.name = name;
    return abiByName.Find(&search,findByName);
}

//---------------------------------------------------------------------------
CFunction *CAbi::findFunctionByEncoding(const SFString& enc)
{
    CFunction search;
    search.encoding = enc;
    return abiByEncoding.Find(&search,findByEncoding);
}

//---------------------------------------------------------------------------
void CAbi::clearABI(void)
{
    abiByName.Clear();
    abiByEncoding.Clear();
}

//---------------------------------------------------------------------------
SFString abis[1000][2];
uint32_t nAbis=0;

//---------------------------------------------------------------------------
void clearAbis(void)
{
    nAbis = 0;
}

SFString findEncoding(const SFString& addr, CFunction& func)
{
    if (!nAbis)
    {
        SFString contents = asciiFileToString(configPath("abis/"+addr+".abi"));
        while (!contents.empty())
        {
            abis[nAbis][1] = nextTokenClear(contents,'\n');
            abis[nAbis][0] = nextTokenClear(abis[nAbis][1],'|');
            nAbis++;
        }
    }

    for (int i=0;i<nAbis;i++)
        if (abis[i][0] == func.name)
            return abis[i][1];
    return EMPTY;
}

//---------------------------------------------------------------------------
static bool getEncoding(const SFString& abiFilename, const SFString& addr, CFunction& func)
{
    if (func.type != "function")
        return false;

    SFString fullName = func.name; // we need the signature for ethabi
    func.name     = nextTokenClear(func.name,'('); // Cleanup because we only need the name, not the signature
    func.encoding = findEncoding(addr, func);
    if (func.encoding.empty() && fileExists("/usr/local/bin/ethabi"))
    {
        // When we call ethabi, we want the full function declaration (if it's present)
        SFString cmd = "/usr/local/bin/ethabi encode function \"" + abiFilename + "\" " + fullName.Substitute("(","\\(").Substitute(")","\\)");
        func.encoding = doCommand(cmd);
    }
    return !func.encoding.empty();
}

//---------------------------------------------------------------------------
bool CAbi::loadABI(const SFString& addr, bool append)
{
    // Already loaded?
    if (abiByName.getCount() && abiByEncoding.getCount())
        return true;

    SFString abiFilename =     configPath("abis/"+addr+".json");
    if (!fileExists(abiFilename))
        return false;

    cerr << "\tLoading abi file: " << abiFilename << "...\n";
    SFString contents = asciiFileToString(abiFilename);
    ASSERT(!contents.empty());

    SFString abis;
    char *p = cleanUpJson((char *)(const char*)contents);
    while (p && *p)
    {
        CFunction func;uint32_t nFields=0;
        p = func.parseJson(p,nFields);
        if (nFields && getEncoding(abiFilename, addr, func))
        {
            abiByName     [ abiByName.getCount     () ] = func;
            abiByEncoding [ abiByEncoding.getCount () ] = func;
            abis += func.Format("[{NAME}]|[{ENCODING}]\n");
        }
    }
    if (!fileExists(configPath("abis/"+addr+".abi")))
        stringToAsciiFile(configPath("abis/"+addr+".abi"),abis);

    abiByName    .Sort( sortFuncTableByName     );
    abiByEncoding.Sort( sortFuncTableByEncoding );
    if (verbose)
    {
        for (uint32_t i=0;i<abiByName.getCount();i++)
        {
            CFunction *f = &abiByName[i];
            if (f->type == "function")
                cerr << f->Format("[\"{NAME}|][{ENCODING}\"]").Substitute("\n"," ") << "\n";
        }
    }
    return abiByName.getCount();
}
// EXISTING_CODE
}  // namespace qblocks

