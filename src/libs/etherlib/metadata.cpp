/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "node.h"
#include "metadata.h"

//--------------------------------------------------------------------------
extern blknum_t getLatestBlock_cache_final(void);
extern blknum_t getLatestBlock_cache_staging(void);
extern blknum_t getLatestBlock_cache_ripe(void);
extern blknum_t getLatestBlock_cache_unripe(void);
extern blknum_t getLatestBlock_client(void);

//-------------------------------------------------------------------------
CMetaData getMetaData(void) {
    CMetaData ret;
    ret.client = getLatestBlock_client();
    ret.finalized = getLatestBlock_cache_final();
    ret.staging = getLatestBlock_cache_staging();
    ret.ripe = getLatestBlock_cache_ripe();
    ret.unripe = getLatestBlock_cache_unripe();
    ret.chainId = str_2_Uint(callRPC("eth_chainId", "[]", false));
    ret.networkId = str_2_Uint(callRPC("net_version", "[]", false));
    ret.chain = getChain();
    return ret;
}

//--------------------------------------------------------------------------
CMetaData getBlockProgress(size_t which) {
    CMetaData ret;

    if (which & BP_CLIENT)
        ret.client = getLatestBlock_client();

    if (which & BP_FINAL)
        ret.finalized = getLatestBlock_cache_final();

    if (which & BP_STAGING)
        ret.staging = getLatestBlock_cache_staging();

    if (which & BP_RIPE)
        ret.ripe = getLatestBlock_cache_ripe();

    if (which & BP_RIPE)
        ret.unripe = getLatestBlock_cache_unripe();

    return ret;
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_final(void) {
    string_q finLast = getLastFileInFolder(indexFolder_blooms, false);
    if (!finLast.empty()) {
        // Files in this folder are n-m.bin
        blknum_t last;
        timestamp_t unused;
        path_2_Bn(finLast, last, unused);
        return last;
    }
    return 0;
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_staging(void) {
    string_q stageLast = getLastFileInFolder(indexFolder_staging, false);
    // Files in this folder are n.txt, if empty, we fall back on finalized folder
    if (!stageLast.empty())
        return path_2_Bn(stageLast);
    return getLatestBlock_cache_final();
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_ripe(void) {
    string_q ripeLast = getLastFileInFolder(indexFolder_ripe, false);
    // Files in this folder are n.txt, if empty, we fall back on staging folder
    if (!ripeLast.empty())
        return path_2_Bn(ripeLast);
    return getLatestBlock_cache_staging();
}

//--------------------------------------------------------------------------
blknum_t getLatestBlock_cache_unripe(void) {
    string_q unripeLast = getLastFileInFolder(indexFolder_unripe, false);
    // Files in this folder are n.txt, if empty, we fall back on ripe folder
    if (!unripeLast.empty())
        return path_2_Bn(unripeLast);
    return getLatestBlock_cache_ripe();
}

//-------------------------------------------------------------------------
blknum_t getLatestBlock_client(void) {
    static blknum_t lastBlock = NOPOS;
    static timestamp_t lastTime = timestamp_t(NOPOS);
    timestamp_t thisTime = date_2_Ts(Now());
    if (thisTime != timestamp_t(NOPOS) && thisTime < timestamp_t(lastTime + 13)) {
        return lastBlock;
    }

    string_q ret = callRPC("eth_blockNumber", "[]", false);
    lastBlock = str_2_Uint(ret);
    if (lastBlock == 0) {
        // Try a different way just in case. Geth, for example, doesn't
        // return blockNumber until the chain is synced (Parity may--don't know
        // We fall back to this method just in case
        string_q str = callRPC("eth_syncing", "[]", false);
        replace(str, "currentBlock:", "|");
        nextTokenClear(str, '|');
        str = nextTokenClear(str, ',');
        lastBlock = str_2_Uint(str);
    }
    return lastBlock;
}
