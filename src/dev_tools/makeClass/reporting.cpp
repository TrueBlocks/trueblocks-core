#include "utillib.h"
#include "options.h"

const string_q reportPath = "../src/dev_tools/makeClass/results/report.csv";

void reportOneOption(const string_q& route, const string_q& option, const string_q& codebase) {
    ostringstream newLine;
    newLine << route << "," << (option % "noheader" ? "noheader" : option) << "," << codebase;

    CStringArray lines, out;
    asciiFileToLines(reportPath, lines);
    for (auto line : lines) {
        if (line != newLine.str()) {
            out.push_back(line);
        }
    }
    out.push_back(newLine.str());

    sort(out.begin(), out.end());

    ostringstream os;
    for (auto o : out) {
        os << o << endl;
    }

    stringToAsciiFile(reportPath, os.str());
}
