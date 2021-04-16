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
    string_q list;
    bool freshen;
    bool compare;
    bool init;
    // END_CODE_DECLARE

    CPinnedChunkArray pList;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    hash_t getCurrentManifest(void);
    bool freshenBlooms(bool download, const string_q& currManifest);
    void handle_unpin(void);
    void handle_pin(void);
};

//-------------------------------------------------------------------------
#define unchainedIndexAddr string_q("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd")
#define manifestHashEncoding string_q("0x337f3f32")
