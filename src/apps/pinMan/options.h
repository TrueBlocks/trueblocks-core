#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "pinlib.h"

// BEG_ERROR_DEFINES
// END_ERROR_DEFINES

//-------------------------------------------------------------------------
class COptions : public CAbiOptions {
  public:
    // BEG_CODE_DECLARE
    bool compare;
    bool init;
    bool freshen;
    // END_CODE_DECLARE

    CPinnedChunkArray pins;
    bool firstOut;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool handle_init(void);
    bool handle_compare(void);
    bool handle_list(void);
};
