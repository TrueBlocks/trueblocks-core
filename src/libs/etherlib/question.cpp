/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "question.h"

//---------------------------------------------------------------------
CQuestion::CQuestion(const string_q& q, bool h, const string_q& c, QUESTIONFUNC f)
  : question(q), answer(""), color(c), func(f), wantsInput(h), nl(false) {
}

//---------------------------------------------------------------------
bool CQuestion::getResponse(void) {
    cout << color << question;
    if (wantsInput) {
        cout << (contains(question, "(") ? "" : "('q' to quit)") << (nl ? "\n\t" : ": ") << "> " << cOff;
        char buff[1024];
        if (fgets(buff, 1024, stdin)) {}  // leave this here--avoids a warning
        answer = trim(substitute(string_q(buff), "\n", ""));
        if (answer % "q" || answer % "quit") {
            cerr << "Quitting...\n";
            exit(0);
        }
    } else {
        cout << "\n";
    }
    cout << cOff;
    return true;
}
