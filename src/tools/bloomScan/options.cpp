/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] =
{
	CParams( "-check",   "check the blooms"),
	CParams( "-display", "visualize the bloom filters"),
	CParams( "", "Scans blocks looking for saturated bloomFilters.\n"),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command)
{

	Init();
        while (!command.empty())
        {
                SFString arg = nextTokenClear(command,' ');
		if (arg == "-d" || arg == "--display")
		{
			mode += "display|";

		} else if (arg == "-c" || arg == "--check")
		{
			mode += "check|";

		} else if (arg.startsWith('-'))
		{
			if (arg != "-h" && !arg.Contains("-v") && arg != "-t")
				return usage("Invalid option: " + arg);
		} else
		{
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void)
{
	paramsPtr  = params;
	nParamsRef = nParams;

	mode = "";

	useVerbose=true;
	useTesting=false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void)
{
	Init();
    // header = "";
    // footer = "";
    // seeAlso = "";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void)
{
}
