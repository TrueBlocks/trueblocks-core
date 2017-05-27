#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"

class CReporter {
public:
    SFUintBN nFiles;
    SFUintBN nBytes;
    SFUintBN totFiles;
    SFUintBN totBytes;
    SFUintBN firstSize;
    SFString largest;
    SFString smallest;
    CReporter() : nFiles(0), nBytes(0), totFiles(0), totBytes(0), firstSize(0) { }
};

//-----------------------------------------------------------------------
extern bool countBytes(const SFString& fileName, void *data);
