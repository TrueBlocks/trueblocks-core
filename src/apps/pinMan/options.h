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

    bool freshen_from_remote(void);
};

//-------------------------------------------------------------------------
#define hashToIndexFormatFile string_q("Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
#define hashToBloomFormatFile string_q("QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
#define unchainedIndexAddr string_q("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd")
#define manifestHashEncoding string_q("0x337f3f32")
