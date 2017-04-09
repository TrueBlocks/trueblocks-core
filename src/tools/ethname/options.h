#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
class COptions : public COptionsBase
{
public:
	SFString addr;
	SFString name;
	SFString source;
	bool all;
	bool count;
	bool matchCase;
	bool open;
	bool write;
	bool list;

	       COptions   (void);
	      ~COptions   (void) {};

	bool parseArguments (SFString& command);
	void Init(void);
};

#define DATA_FILE getHomeFolder()+".ethslurp/configs/names.conf"
