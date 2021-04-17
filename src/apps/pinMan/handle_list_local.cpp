#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list_local(void) {
    ASSERT(localPins.size());  // local pins have already been loaded
    for (auto pin : localPins) {
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
