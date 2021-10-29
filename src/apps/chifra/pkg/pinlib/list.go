package pinlib

import (
	"io/ioutil"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/chunk"
)

// This package makes it easier to get file names of all blooms/index chunks
// that are currently saved locally

func ListByChunkType(chunkType chunk.ChunkType) ([]string, error) {
	cacheLayout := &chunk.CacheLayout{}
	cacheLayout.New(chunkType)

	files, err := ioutil.ReadDir(cacheLayout.String())
	if err != nil {
		return nil, err
	}

	fileNames := make([]string, len(files))
	for _, file := range files {
		fileNames = append(
			fileNames,
			cacheLayout.RemoveExtension(file.Name()),
		)
	}

	return fileNames, nil
}

func ListLocalBlooms() ([]string, error) {
	return ListByChunkType(chunk.BloomChunk)
}

func ListLocalIndexes() ([]string, error) {
	return ListByChunkType(chunk.IndexChunk)
}
