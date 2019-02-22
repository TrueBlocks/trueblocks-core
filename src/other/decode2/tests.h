#pragma once

class T {
public:
    string_q desc;
    bool bVal;
    string_q input;
    string_q expected;
    T(const string_q& d, bool v, const string_q& in, const string_q& ex)
        : desc(d), bVal(v), input(in), expected(ex) {}
    T(void) : desc(""), bVal(false), input(""), expected("") {}
    string_q check(const string_q& res) {
        ostringstream os;
        os << "  " << ((res == expected) ? greenCheck : redX);
        os << "\tdesc:     " << desc << endl;
        os << "\tinput:    " << input << endl;
        os << "\texpected: " << expected << endl;
        os << "\tresult:   " << res << endl;
        if (res != expected) {
            cerr << os.str() << "\t" << cRed << "Test failed. Quitting" << cOff << endl;
            quickQuitHandler(0);
        }
        return os.str();
    }
};
