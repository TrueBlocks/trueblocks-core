// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import (
	"io/ioutil"
)

// This package makes it easier to get file names of all blooms/index chunks
// that are currently saved locally

// ListByCacheType returns a slice of file names (strings) of all chunks of
// the given type in UnchainedIndex directory
func ListByCacheType(chunkType CacheType) ([]string, error) {
	cachePath := &Path{}
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
