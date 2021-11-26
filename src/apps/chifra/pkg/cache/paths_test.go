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
package cache

import (
	"path"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func TestCacheLayout(t *testing.T) {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	// cachePath := config.ReadTrueBlocks().Settings.CachePath
	tests := []struct {
		name      string
		cacheType CacheType
		param     string
		expected  CachePath
		path      string
		wantErr   bool
	}{
		{
			name:      "index path",
			cacheType: IndexChunk,
			param:     "filename",
			expected: CachePath{
				RootPath:  indexPath,
				Subdir:    "finalized/",
				Extension: ".bin",
			},
			path:    "finalized/filename.bin",
			wantErr: false,
		},
		{
			name:      "blooms path",
			cacheType: BloomChunk,
			param:     "filename",
			expected: CachePath{
				RootPath:  indexPath,
				Subdir:    "blooms/",
				Extension: ".bloom",
			},
			path:    "blooms/filename.bloom",
			wantErr: false,
		},
		// {
		// 	name:      "blocks cache path",
		// 	cacheType: BlockCache,
		// 	param:     "10001001",
		// 	expected: CachePath{
		// 		RootPath:  cachePath,
		// 		Subdir:    "blocks/00/10/01/",
		// 		Extension: ".bin",
		// 	},
		// 	path:    "blooms/filename.bloom",
		// 	wantErr: false,
		// },
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			cachePath := &CachePath{}
			cachePath.New(tt.cacheType)

			if cachePath.Extension != tt.expected.Extension {
				t.Error("Wrong Extension", cachePath.Extension)
			}

			if cachePath.Subdir != tt.expected.Subdir {
				t.Error("Wrong Subdir", cachePath.Subdir)
			}

			p := cachePath.GetPathTo(tt.param)
			if p != path.Join(tt.expected.RootPath, tt.path) {
				t.Error("Wrong ToPathResult", p)
			}
		})
	}
}
