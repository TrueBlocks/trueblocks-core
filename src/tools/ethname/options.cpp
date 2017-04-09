/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] =
{
	CParams("~term [name]",	"search for 'term' in either name or address. If 'name' is present, term \n\t\t\t     is assumed to be an address. In this case, both must match" ),
	CParams("-all",		"search 'source' field as well name and address" ),
	CParams("-count",	"print only the count of the number of matches" ),
	CParams("-list",        "list all names" ),
	CParams("-matchCase",	"matches must agree in case (the default is to ignore case)" ),
	CParams("-open",	"open the name database for editing" ),
	CParams("-write",	"write the name/address pair to database. Will prompt for an optional source" ),
	CParams( "",		"Find a name given an Ethereum address, or find an address given a name.\n" ),
};
uint32_t nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(SFString& command)
{
	Init();
	while (!command.empty())
	{
		bool needWrite=false;
		SFString arg = nextTokenClear(command,' ');
		if (arg == "-a" || arg == "-all")
		{
			all = true;

		} else if (arg == "-c" || arg == "-count")
		{
			count = true;

		} else if (arg == "-l" || arg == "-list")
		{
			list = true;

		} else if (arg == "-m" || arg == "-matchCase")
		{
			matchCase = true;

		} else if (arg == "-o" || arg == "-open")
		{
			// open command stuff
			system("pico " + DATA_FILE);
			exit(0);

		} else if (arg == "-w" || arg == "-write")
		{
			needWrite = true;

		} else if (arg.startsWith('-'))
		{
			if (arg != "-t" && arg != "-h" && !arg.Contains("-v"))
			{
				return usage("Invalid option: " + arg);
			}

		} else
		{
			if (!addr.empty() && !name.empty() && !source.empty())
				return usage("Invalid option: " + arg);
			else if (!addr.empty() && !name.empty())
				source = arg;
			else if (!addr.empty())
				name = arg;
			else
				addr = arg;
		}

		if (needWrite && (name.empty() || addr.empty() || source.empty()))
			return usage("If -w is given, all three of 'addr', 'name', and 'source' must be given");
	}
	return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void)
{
	paramsPtr = params;
	nParamsRef = nParams;

	outScreen.setOutput(stdout); // so we know where it is at the start of each run
	addr = EMPTY;
	name = EMPTY;
	source = EMPTY;
	all = false;
	write = false;
	matchCase = false;
	list = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void)
{
	COptionsBase::useVerbose = true;
	COptionsBase::useTesting = false;
	Init();
        msg = "  Powered by QuickBlocks.io\n\n";
}
