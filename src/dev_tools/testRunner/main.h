#pragma once
#include "utillib.h"

class CTestCase {
  public:
    string_q origLine;
    string_q onOff;
    string_q mode;
    string_q speed;
    string_q route;
    string_q tool;
    string_q name;
    string_q post;
    string_q options;
    string_q path;
    string_q goldPath;
    string_q workPath;
    string_q fileName;

    CTestCase(void);
    CTestCase(const CTestCase& te);
    virtual ~CTestCase(void);
    CTestCase& operator=(const CTestCase& te);

    explicit CTestCase(const string_q& line);
    void prepareTest(bool isCmd);
    bool operator==(const CTestCase& it) const;
    bool operator!=(const CTestCase& it) const {
        return !operator==(it);
    }
    void copyBack(void);
};

inline CTestCase::CTestCase(void) {
    origLine = "";
    onOff = "";
    mode = "";
    speed = "";
    route = "";
    tool = "";
    name = "";
    post = "";
    options = "";
    path = "";
    goldPath = "";
    workPath = "";
    fileName = "";
}

inline CTestCase::CTestCase(const CTestCase& te) {
    origLine = te.origLine;
    onOff = te.onOff;
    mode = te.mode;
    speed = te.speed;
    route = te.route;
    tool = te.tool;
    name = te.name;
    post = te.post;
    options = te.options;
    path = te.path;
    goldPath = te.goldPath;
    workPath = te.workPath;
    fileName = te.fileName;
}

inline CTestCase::~CTestCase(void) {
}

inline CTestCase& CTestCase::operator=(const CTestCase& te) {
    origLine = te.origLine;
    onOff = te.onOff;
    mode = te.mode;
    speed = te.speed;
    route = te.route;
    tool = te.tool;
    name = te.name;
    post = te.post;
    options = te.options;
    path = te.path;
    goldPath = te.goldPath;
    workPath = te.workPath;
    fileName = te.fileName;
    return *this;
}

inline bool CTestCase::operator==(const CTestCase& it) const {
    return (route % it.route && tool % it.tool && name % it.name);
}

inline CTestCase::CTestCase(const string_q& line) {
    origLine = line;

    CStringArray parts;
    explode(parts, line, ',');
    onOff = parts.size() > 0 ? trim(parts[0]) : "";
    mode = parts.size() > 1 ? trim(parts[1]) : "";
    speed = parts.size() > 2 ? trim(parts[2]) : "";
    route = parts.size() > 3 ? trim(parts[3]) : "";
    tool = parts.size() > 4 ? trim(parts[4]) : "";
    name = parts.size() > 5 ? trim(parts[5]) : "";
    post = parts.size() > 6 ? (trim(parts[6]) == "y" ? "jq ." : "") : "";
    options = parts.size() > 7 ? trim(parts[7]) : "";

    CStringArray chars = {"=", "&", "@"};
    for (auto ch : chars) {
        replaceAll(options, " " + ch, ch);
        replaceAll(options, ch + " ", ch);
    }
    replaceAll(options, "&#44;", ",");

    path = nextTokenClear(tool, '/');
    fileName = tool + "_" + name + ".txt";

    string_q tr = "/test/gold/dev_tools/testRunner/";
    goldPath = substitute(getCWD(), tr, "/test/gold/" + path + "/" + tool + "/");
    workPath = substitute(getCWD(), tr, "/test/working/" + path + "/" + tool + "/");
}

inline void CTestCase::prepareTest(bool isCmd) {
    if (isCmd) {
        CStringArray opts = {"val",   "addrs",     "blocks", "files", "dates",  "transactions",
                             "terms", "functions", "modes",  "mode",  "topics", "fourbytes"};
        options = "&" + options;
        for (auto opt : opts)
            replaceAll(options, "&" + opt + "=", " ");
        replaceAll(options, "%20", " ");
        replaceAll(options, "@", " -");
        replaceAll(options, "&", " --");
        // replaceAll(options, "\\*", " \"*\"");
        replaceAll(options, "=", " ");
        if (trim(options) == "--" || startsWith(trim(options), "-- "))
            replace(options, "--", "");

    } else {
        if (tool == "chifra")
            nextTokenClear(options, '&');
        CStringArray parts;
        explode(parts, options, '&');
        ostringstream os;
        for (auto part : parts) {
            string_q key = nextTokenClear(part, '=');
            if (!os.str().empty())
                os << "&";
            os << toCamelCase(key) << (part.empty() ? "" : "=" + part);
        }
        options = os.str();
        replaceAll(options, "@", "");
        replaceAll(options, " ", "%20");
        goldPath += "api_tests/";
        workPath += "api_tests/";
    }
}

extern int copyFile(const string_q& fromIn, const string_q& toIn);
inline void CTestCase::copyBack(void) {
    string_q tr = "/test/gold/dev_tools/testRunner/";
    string_q fn = path + "/" + tool + "/" + fileName;
    string_q fnA = path + "/" + tool + "/api_tests/" + fileName;

    string_q goldPath = substitute(getCWD(), tr, "/test/gold/" + fn);
    string_q workPath = substitute(getCWD(), tr, "/test/working/" + fn);
    if (fileExists(goldPath)) {
        copyFile(goldPath, workPath);
    }

    goldPath = substitute(getCWD(), tr, "/test/gold/" + fnA);
    workPath = substitute(getCWD(), tr, "/test/working/" + fnA);
    if (fileExists(goldPath)) {
        copyFile(goldPath, workPath);
    }
}

#define API (1 << 0)
#define CMD (1 << 1)
#define BOTH (API | CMD)

class COptions {
  public:
    string_q sourceFolder;
    uint64_t totalTests = 0;
    uint64_t totalPassed = 0;
    CStringArray fails;
    CStringArray tests;

    COptions(void);
    ~COptions(void);

    void init(void);
    void doTests(vector<CTestCase>& testArray, const string_q& testName, int which);
};

inline COptions::COptions(void) {
}

inline COptions::~COptions(void) {
}

extern bool rmWorkingTests(const string_q& path, const string_q& testName);
extern string_q getOutputFile(const string& orig, const string_q& goldApiPath);
extern string_q linesToString(const CStringArray& lines, char sep = '\n');
extern string_q doCommand(const string_q& cmd, bool readStderr = false);
extern string_q padRight(const string_q& str, size_t len, char p = ' ');
extern string_q padLeft(const string_q& str, size_t len, char p = ' ');
extern string_q getEnvStr(const string_q& name);
extern int copyFile(const string_q& from, const string_q& to);
extern int cleanFolder(const string_q& path, bool recurse = false, bool interactive = false);
extern string_q makeValidName(const string_q& inOut);
extern string_q getCachePath(void);
