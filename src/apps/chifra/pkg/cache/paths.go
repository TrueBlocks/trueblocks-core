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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// Examples of cache paths from the C++ code
// IndexFn:      $INDEX_PATH/finalized/0010000000-0010200000.bin
// BloomFn:      $INDEX_PATH/blooms/0010000000-0010200000.bloom
// BlockFn:      $CACHE_PATH/blocks/00/10/01/001001001.bin
// TxFn:         $CACHE_PATH/txs/00/10/01/001001001-00020.bin
// TraceFn:      $CACHE_PATH/traces/00/10/01/001001001-00020-10.bin
// NeighborFn:   $CACHE_PATH/neighbors/00/10/01/001001001-00020.bin
// ReconFn:      $CACHE_PATH/recons/c011/a724/00e58ecd99ee497cf89e3775d4bd732f/000000012.00013.bin

// IndexPath:    $INDEX_PATH/finalized/
// BloomPath:    $INDEX_PATH/blooms/
// BlockPath:    $CACHE_PATH/blocks/00/10/01/
// TxPath:       $CACHE_PATH/txs/00/10/01/
// TracePath:    $CACHE_PATH/traces/00/10/01/
// NeighborPath: $CACHE_PATH/neighbors/00/10/01/
// ReconPath:    $CACHE_PATH/recons/c011/a724/00e58ecd99ee497cf89e3775d4bd732f/

// CacheLayout helps to keep track of cache paths and extensions depending on
// chunk type
type CacheLayout struct {
	OutputDir string
	Subdir    string
	Extension string
}

// New sets correct values of Subdir and Extension properties based on
// chunkType
func (cl *CacheLayout) New(chunkType ChunkType) {
	Subdir := "blooms/"
	Extension := ".bloom"
	if chunkType == IndexChunk {
		Subdir = "finalized/"
		Extension = ".bin"
	}

	cl.OutputDir = config.ReadTrueBlocks().Settings.IndexPath
	cl.Subdir = Subdir
	cl.Extension = Extension
}

// GetPathTo uses the data stored in outputConfig to build a path and return it
// as a string
func (cl *CacheLayout) GetPathTo(fileName string) string {
	return path.Join(cl.OutputDir, cl.Subdir, fileName+cl.Extension)
}

// RemoveExtension removes Extension (".bloom" or ".bin") from fileName
func (cl *CacheLayout) RemoveExtension(fileName string) string {
	return strings.Replace(fileName, cl.Extension, "", 1)
}

// String turns cacheLayout data (OutputDir and Subdir) into a path
// and returns it as a string
func (cl *CacheLayout) String() string {
	return path.Join(cl.OutputDir, cl.Subdir)
}
