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
package pinlib

import (
	"io/ioutil"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// This package makes it easier to get file names of all blooms/index chunks
// that are currently saved locally

// ListByChunkType returns a slice of file names (strings) of all chunks of
// the given type in UnchainedIndex directory
func ListByChunkType(chunkType cache.CacheType) ([]string, error) {
	cachePath := &cache.CachePath{}
	cachePath.New(chunkType)

	files, err := ioutil.ReadDir(cachePath.String())
	if err != nil {
		return nil, err
	}

	fileNames := make([]string, len(files))
	for _, file := range files {
		fileNames = append(
			fileNames,
			cachePath.RemoveExtension(file.Name()),
		)
	}

	return fileNames, nil
}

// ListLocalBlooms returns a slice of file names (strings) of all bloom
// filters present in UnchainedIndex directory
func ListLocalBlooms() ([]string, error) {
	return ListByChunkType(cache.BloomChunk)
}

// ListLocalIndexes returns a slice of file names (strings) of all index
// chunks present in UnchainedIndex directory
func ListLocalIndexes() ([]string, error) {
	return ListByChunkType(cache.IndexChunk)
}
