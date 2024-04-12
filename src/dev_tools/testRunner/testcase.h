#pragma once

#include "utillib.h"

class CTestCase : public CBaseNode {
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
    void prepareTest(bool cmdLine);
    bool operator==(const CTestCase& it) const;
    bool operator!=(const CTestCase& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTestCase& v1, const CTestCase& v2);
    friend ostream& operator<<(ostream& os, const CTestCase& it);
};

inline CTestCase::CTestCase(void) {
    CBaseNode::initialize();
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
    CBaseNode::duplicate(te);
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
    CBaseNode::duplicate(te);
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

inline bool operator<(const CTestCase& v1, const CTestCase& v2) {
    return true;
}

inline void CTestCase::prepareTest(bool cmdLine) {
    goldPath = substitute(getCWD(), "/test/gold/dev_tools/testRunner/", "/test/gold/" + path + "/" + tool + "/");
    workPath = substitute(goldPath, "/gold/", "/working/");
    establishFolder(goldPath);
    establishFolder(workPath);

    if (cmdLine) {
        CStringArray opts = {"val",   "addrs",     "blocks", "files", "dates",  "transactions",
                             "terms", "functions", "modes",  "mode",  "topics", "fourbytes"};
        options = "&" + options;
        for (auto opt : opts)
            replaceAll(options, "&" + opt + "=", " ");
        replaceAll(options, "%20", " ");
        replaceAll(options, "@", " -");
        replaceAll(options, "&", " --");
        replaceAll(options, "\\*", " \"*\"");
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
    post = parts.size() > 6 ? trim(parts[6]) : "";
    options = parts.size() > 7 ? trim(parts[7]) : "";

    replaceAll(post, "n", "");
    replaceAll(post, "y", "jq .");

    path = nextTokenClear(tool, '/');
    fileName = tool + "_" + name + ".txt";

    replaceAll(options, " = ", "=");
    replaceAll(options, "= ", "=");
    replaceAll(options, " & ", "&");
    replaceAll(options, "& ", "&");
    replaceAll(options, " @ ", "@");
    replaceAll(options, "@ ", "@");
    replaceAll(options, "&#44;", ",");
}
