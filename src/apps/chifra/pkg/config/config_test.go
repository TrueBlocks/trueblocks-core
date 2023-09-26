// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_ReadTrueBlocks(t *testing.T) {
	values := GetRootConfig()
	if len(values.Settings.CachePath) == 0 {
		t.Error("CachePath is empty")
	}
	if values.Settings.CachePath[len(values.Settings.CachePath)-1] != '/' {
		// the raw path may not end with a slash, but the returned path better
		ret := GetPathToCache(utils.GetTestChain())
		if len(ret) > 0 && ret[len(ret)-1] != '/' {
			t.Error("CachePath does not end with a '/'")
		}
	}
	if values.Settings.CachePath[0] != '/' {
		t.Error("CachePath is not an absolute path")
	}
	if len(values.Settings.IndexPath) == 0 {
		t.Error("IndexPath is empty")
	}
	if values.Settings.IndexPath[len(values.Settings.IndexPath)-1] != '/' {
		// the raw path may not end with a slash, but the returned path better
		ret := GetPathToIndex(utils.GetTestChain())
		if len(ret) > 0 && ret[len(ret)-1] != '/' {
			t.Error("IndexPath does not end with a '/'")
		}
	}
	if values.Settings.IndexPath[0] != '/' {
		t.Error("IndexPath is not an absolute path")
	}
	if len(values.Settings.DefaultChain) == 0 {
		t.Error("DefaultChain is empty.")
	}
}
