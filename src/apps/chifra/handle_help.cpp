/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_help(void) {

    ENTER("handle_" + mode);
    nodeNotRequired();

    cerr << cYellow;
    cerr << "\tEnter as many address/name pairs as you wish (space separated)," << endl;
    cerr << "\tor 'n' to show a list of common contract names, or 'h' for help." << endl;
    cerr << cOff;

    EXIT_OK("handle_" + mode);
    return true;
}
