/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <string>
#include <iostream>
#include <sstream>
#include "utillib.h"

using namespace std;

//----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
    cout << urlToString("http://google.com");
    cout << urlToString("https://quickblocks.io");
	return 0;
}
