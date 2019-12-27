/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern bool makeHeatmap(const string_q& path, void* data);
//--------------------------------------------------------------------------------
bool COptions::handle_heatmap(ostream& os) {
    CHeatmap hm;
    blknum_t latest = ((getLatestBlock_client() / 50000) + 1) * 50000;
    for (uint32_t i = 0; i < latest; i = i + 50000)
        hm.heatmap[i] = 0;
    forEveryFileInFolder(getCachePath(substitute(heatmap, "transactions", "txs")), makeHeatmap, &hm);
    for (auto& m : hm.heatmap) {
        static int cnt = 0;
        cout << (m.second ? uint_2_Str(m.second) : " ") << "\t";
        if (!(++cnt % 10))
            cout << endl;
    }
    return true;
}

//---------------------------------------------------------------------------
bool makeHeatmap(const string_q& path, void* data) {
    CHeatmap* heatmap = reinterpret_cast<CHeatmap*>(data);
    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", makeHeatmap, data);
        return true;
    } else if (endsWith(path, ".bin")) {
        CFilename filename(path);
        blknum_t bn = (str_2_Uint(filename.getFilename()) / 50000) * 50000;
        heatmap->heatmap[bn] = heatmap->heatmap[bn] + 1;
        // if (heatmap->heatmap[bn] > 100)
        //    return false;
        //        cout << path << "\t" << filename.getFilename() << "\t" << bn << "\t" << heatmap->heatmap[bn] << endl;
    }
    return !shouldQuit();
}
