/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list(void) {
    if (remote) {
        cerr << "remote not implemented" << endl;

    } else {
        ASSERT(pins.size());  // local pins have already been loaded
        for (auto pin : pins) {
            if (!isJson()) {
                cout << trim(pin.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                cout << ((isJson() && !firstOut) ? ", " : "");
                indent();
                pin.toJson(cout);
                unindent();
            }
            firstOut = false;
        }
    }
    return false;
}
