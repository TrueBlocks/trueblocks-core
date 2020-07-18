#include "etherlib.h"

//----------------------------------------------------------------
bool isAlreadyRunning(const string_q& progName) {
    string_q pList = listProcesses(progName);
    replaceAll(pList, "`", ""); // remove separators if present
    replaceAll(pList, progName, "`"); // change program name to separator
    replace(pList, "`", ""); // remove ourselves
    size_t count = countOf(pList, '`');
    return count > 0;
}

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    string_q result = doCommand("ps uxww | grep removeMe");
    replaceAll(result, "grep removeMe", "");
    CStringArray lines;
    explode(lines, result, '\n');
    uint32_t cnt = 0;
    for (auto line : lines) {
        cnt += contains(line, "removeMe");
        cout << contains(line, "removeMe") << ": " << line.substr(0, 100) << endl;
    }
    cout << "nRunning: " << cnt << endl;
    return 1;
}
