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

// CachePath helps to keep track of cache paths and extensions depending on
// chunk type
type CachePath struct {
	Type      CacheType
	RootPath  string
	Subdir    string
	Extension string
}

var cacheFolders = map[CacheType]string{
	BloomChunk:    "blooms/",
	IndexChunk:    "finalized/",
	BlockCache:    "blocks/",
	TxCache:       "txs/",
	TraceCache:    "traces/",
	MonitorCache:  "monitors/",
	NeighborCache: "neighbors/",
	ReconCache:    "recons/",
}

// New sets correct values of Subdir and Extension properties based on cacheType
func (cl *CachePath) New(cacheType CacheType) {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	cachePath := config.ReadTrueBlocks().Settings.CachePath

	if cacheType == BloomChunk || cacheType == IndexChunk {
		cl.RootPath = indexPath
	} else {
		cl.RootPath = cachePath
	}

	cl.Extension = ".bin"
	if cacheType == BloomChunk {
		cl.Extension = ".bloom"
	}
	cl.Subdir = cacheFolders[cacheType]
}

// GetFullPath builds a full path from the CachePath type
func (cl *CachePath) GetFullPath(name string) string {
	switch cl.Type {
	case IndexChunk:
		fallthrough
	case BloomChunk:
		fallthrough
	case MonitorCache:
		return path.Join(cl.RootPath, cl.Subdir, name+cl.Extension)
	default:
		return path.Join(cl.RootPath, cl.Subdir)
	}
}

// RemoveExtension removes Extension (".bloom" or ".bin") from fileName
func (cl *CachePath) RemoveExtension(fileName string) string {
	return strings.Replace(fileName, cl.Extension, "", 1)
}

// String turns cachePath data (RootPath and Subdir) into a path
// and returns it as a string
func (cl *CachePath) String() string {
	return path.Join(cl.RootPath, cl.Subdir)
}
