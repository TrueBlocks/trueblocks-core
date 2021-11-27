// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunk

import (
	"path"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func TestCacheLayout_NewIndex(t *testing.T) {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	indexConfig := &CacheLayout{}
	indexConfig.New(IndexChunk)

	if indexConfig.extension != ".bin" {
		t.Error("Wrong extension", indexConfig.extension)
	}

	if indexConfig.subdir != "finalized/" {
		t.Error("Wrong subdir", indexConfig.subdir)
	}

	p := indexConfig.GetPathTo("filename")

	if p != path.Join(indexPath, "finalized/filename.bin") {
		t.Error("Wrong ToPathResult", p)
	}
}

func TestCacheLayout_NewBloom(t *testing.T) {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	indexConfig := &CacheLayout{}
	indexConfig.New(BloomChunk)

	if indexConfig.extension != ".bloom" {
		t.Error("Wrong extension", indexConfig.extension)
	}

	if indexConfig.subdir != "blooms/" {
		t.Error("Wrong subdir", indexConfig.subdir)
	}

	p := indexConfig.GetPathTo("filename")

	if p != path.Join(indexPath, "blooms/filename.bloom") {
		t.Error("Wrong ToPathResult", p)
	}
}
