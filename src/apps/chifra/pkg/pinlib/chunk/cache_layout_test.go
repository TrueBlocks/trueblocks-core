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
package chunk

import (
	"path"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func TestCacheLayout_NewIndex(t *testing.T) {
	indexPath := config.ReadGlobal().Settings.IndexPath
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
	indexPath := config.ReadGlobal().Settings.IndexPath
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
