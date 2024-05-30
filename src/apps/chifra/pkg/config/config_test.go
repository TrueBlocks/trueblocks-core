// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_ReadTrueBlocks(t *testing.T) {
	if len(GetSettings().CachePath) == 0 {
		t.Error("CachePath is empty")
	}
	if GetSettings().CachePath[len(GetSettings().CachePath)-1] != '/' {
		// the path may not end with a slash, but the returned path better
		ret := PathToCache(utils.GetTestChain())
		if len(ret) > 0 && ret[len(ret)-1] != '/' {
			t.Error("CachePath does not end with a '/'")
		}
	}
	if GetSettings().CachePath[0] != '/' {
		t.Error("CachePath is not an absolute path")
	}
	if len(GetSettings().IndexPath) == 0 {
		t.Error("IndexPath is empty")
	}
	if GetSettings().IndexPath[len(GetSettings().IndexPath)-1] != '/' {
		// the path may not end with a slash, but the returned path better
		ret := PathToIndex(utils.GetTestChain())
		if len(ret) > 0 && ret[len(ret)-1] != '/' {
			t.Error("IndexPath does not end with a '/'")
		}
	}
	settings := GetSettings()
	if settings.IndexPath[0] != '/' {
		t.Error("IndexPath is not an absolute path")
	}
	if len(GetSettings().DefaultChain) == 0 {
		t.Error("DefaultChain is empty.")
	}
}
