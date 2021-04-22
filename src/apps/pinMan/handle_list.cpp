#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list(void) {
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
    return false;
}
