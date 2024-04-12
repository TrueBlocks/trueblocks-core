#include "configenv.h"
#include "filenames.h"

namespace qblocks {

IMPLEMENT_NODE(CConfigEnv, CBaseNode);

extern string_q nextConfigenvChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigenvChunk_custom(const string_q& fieldIn, const void* dataPtr);

void CConfigEnv::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configenv_fmt"] : fmtIn);
    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextConfigenvChunk, this);
}

string_q nextConfigenvChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfigEnv*>(dataPtr)->getValueByName(fieldIn);
    return fldNotFound(fieldIn);
}

string_q CConfigEnv::getValueByName(const string_q& fieldName) const {
    string_q ret = nextConfigenvChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "chain") {
                return chain;
            }
            if (fieldName % "configPath") {
                return configPath;
            }
            if (fieldName % "chainConfigPath") {
                return chainConfigPath;
            }
            if (fieldName % "cachePath") {
                return cachePath;
            }
            break;
        default:
            break;
    }

    return CBaseNode::getValueByName(fieldName);
}

bool CConfigEnv::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "chain") {
                chain = fieldValue;
                return true;
            }
            if (fieldName % "configPath") {
                configPath = fieldValue;
                return true;
            }
            if (fieldName % "chainConfigPath") {
                chainConfigPath = fieldValue;
                return true;
            }
            if (fieldName % "cachePath") {
                cachePath = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

void CConfigEnv::finishParse() {
}

void CConfigEnv::registerClass(void) {
    if (HAS_FIELD(CConfigEnv, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfigEnv, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfigEnv, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigEnv, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigEnv, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigEnv, "chain", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "configPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "chainConfigPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "cachePath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CConfigEnv, "schema");
    HIDE_FIELD(CConfigEnv, "deleted");
    HIDE_FIELD(CConfigEnv, "showing");
    HIDE_FIELD(CConfigEnv, "cname");

    builtIns.push_back(_biCConfigEnv);
}

//---------------------------------------------------------------------------
string_q nextConfigenvChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfigEnv* con = reinterpret_cast<const CConfigEnv*>(dataPtr);
    if (con) {
        switch (tolower(fieldIn[0])) {
            case 'p':
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, con);
                break;

            default:
                break;
        }
    }

    return "";
}

ostream& operator<<(ostream& os, const CConfigEnv& it) {
    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

static CConfigEnv g_configEnv;
string_q getPathToCommands(const string_q& _part) {
    return getHomeFolder() + ".local/bin/chifra/" + _part;
}

const CConfigEnv* getConfigEnv(void) {
    return &g_configEnv;
}

void loadEnvironmentPaths(void) {
    g_configEnv.chain = "mainnet";
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    g_configEnv.configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    g_configEnv.configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#endif
    g_configEnv.chainConfigPath = g_configEnv.configPath + "config/mainnet/";
    g_configEnv.cachePath = g_configEnv.configPath + "cache/mainnet/";
}

}  // namespace qblocks
