// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunk

import (
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// CacheLayout helps to keep track of cache paths and extensions depending on
// chunk type
type CacheLayout struct {
	outputDir string
	subdir    string
	extension string
}

// New sets correct values of subdir and extension properties based on
// chunkType
func (cl *CacheLayout) New(chunkType ChunkType) {
	subdir := "blooms/"
	extension := ".bloom"
	if chunkType == IndexChunk {
		subdir = "finalized/"
		extension = ".bin"
	}

	cl.outputDir = config.ReadTrueBlocks().Settings.IndexPath
	cl.subdir = subdir
	cl.extension = extension
}

// GetPathTo uses the data stored in outputConfig to build a path and return it
// as a string
func (cl *CacheLayout) GetPathTo(fileName string) string {
	return path.Join(cl.outputDir, cl.subdir, fileName+cl.extension)
}

// RemoveExtension removes extension (".bloom" or ".bin") from fileName
func (cl *CacheLayout) RemoveExtension(fileName string) string {
	return strings.Replace(fileName, cl.extension, "", 1)
}

// String turns cacheLayout data (outputDir and subdir) into a path
// and returns it as a string
func (cl *CacheLayout) String() string {
	return path.Join(cl.outputDir, cl.subdir)
}
