// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ChunksOptions) HandlePins(blockNums []uint64) error {
	maxTestItems = 10

	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleChunkRecord{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	source := manifest.FromCache
	if opts.Remote {
		source = manifest.FromContract
	}

	man, err := manifest.ReadManifest(opts.Globals.Chain, source)
	if err != nil {
		return err
	}

	sort.Slice(man.Chunks, func(i, j int) bool {
		iPin := man.Chunks[i]
		jPin := man.Chunks[j]
		return iPin.Range < jPin.Range
	})

	pinList := make([]types.SimpleChunkRecord, len(man.Chunks))
	for i := range man.Chunks {
		pinList[i].Range = man.Chunks[i].Range
		pinList[i].BloomHash = string(man.Chunks[i].BloomHash)
		pinList[i].IndexHash = string(man.Chunks[i].IndexHash)
	}

	for i, r := range pinList {
		if opts.Globals.TestMode && i > maxTestItems {
			continue
		}
		err := opts.Globals.RenderObject(r, i == 0)
		if err != nil {
			return err
		}
	}

	return nil
}

// TODO: BOGUS - OLD C++ CODE TO READ FROM PINATA
/*
//---------------------------------------------------------------------------
// Assumes the license is valid
bool pinlib_getPinList(const CPinApiLicense& lic, string& result) {
    result = "";
    result.clear();

    bool ret = true;  // assume success

    CURL* curl;
    curl = curl_easy_init();  // TODO(tjayrush): Creating and destroying curl context is slow
    if (!curl) {
        curl_easy_cleanup(curl);
        result = "Could not open curl context";
        ret = false;
    } else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        // TODO(tjayrush): This has pagination, I just didn't notice it
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/data/pinList?status=pinned");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secretKey).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode res = curl_easy_perform(curl);
        if (res != 0) {
            curl_easy_cleanup(curl);
            result = curl_easy_strerror(res);
            ret = false;
        }
    }
    curl_easy_cleanup(curl);
    if (!ret)  // if we already have an error
        return ret;

    if (result.empty()) {
        result = "No result";
        return false;
    }

    return true;
}
*/
