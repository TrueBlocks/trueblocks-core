<<<<<<< HEAD:src/apps/chifra/pkg/cache/list.go
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
=======
// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package pinlib
>>>>>>> develop:src/apps/chifra/pkg/pinlib/list.go

import (
	"io/ioutil"
)

// This package makes it easier to get file names of all blooms/index chunks
// that are currently saved locally

// ListByCacheType returns a slice of file names (strings) of all chunks of
// the given type in UnchainedIndex directory
func ListByCacheType(chunkType CacheType) ([]string, error) {
	cachePath := &CachePath{}
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
	return ListByCacheType(BloomChunk)
}

// ListLocalIndexes returns a slice of file names (strings) of all index
// chunks present in UnchainedIndex directory
func ListLocalIndexes() ([]string, error) {
	return ListByCacheType(IndexChunk)
}
