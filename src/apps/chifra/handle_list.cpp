/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_list(void) {

    remainder = substitute(remainder,"|","");
    if (!isAddress(remainder))
        return usage(remainder + " does not appear to be an address. Quitting...");
    CAccountWatch watch;
    watch.address = remainder;
    watch.name = trim(remainder, '|');
    watch.color = colors[watches.size()%nColors];
    watches.push_back(watch);
    if (monitorName.empty())
        monitorName = watch.name;
    cerr << cGreen << cOff << "\tAdded watch: " << watch.color << watch.address << cOff << " (" << watch.name << ")" << endl;
    if (!makeNewMonitor()) {
        remainder = watches[0].address;
        return handle_freshen();
    }
    return true;
}
