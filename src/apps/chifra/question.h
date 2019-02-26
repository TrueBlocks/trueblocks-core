#pragma once
/*-------------------------------------------------------------------------
 * Derived from https://mattmccutchen.net/bigint/index.html
 * From the website under the Legal section: I, Matt McCutchen, the sole author
 * of the original Big Integer Library, waive my copyright to it, placing it
 * in the public domain.  The library comes with absolutely no warranty.
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------------
class CQuestion;
typedef bool (*QUESTIONFUNC)(const COptions& options, CQuestion *q);

//-----------------------------------------------------------------------------
class CQuestion {
public:
    string_q question;
    string_q answer;
    string_q color;
    QUESTIONFUNC func;
    bool wantsInput;
    bool nl;

         CQuestion  (const string_q& q, bool h, const string_q& c, QUESTIONFUNC f = NULL);
    bool getResponse(void);
};
