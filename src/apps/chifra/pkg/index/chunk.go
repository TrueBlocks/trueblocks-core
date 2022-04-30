// Package index provides tools needed to acquire, read, write and test for set inclusion in an index chunk.
//
// An index chunk is a data structure with three parts. A FileRange which indicates the first block
// and last block of the chunk (inclusive), the ChunkData which carries the list of address appearances
// in the given block range, and a ChunkBloom which allows for rapid queries to determine if a given address
// appears in the ChunkData without having to read the data from disc.
//
// The bloom filter returns true or false indicating either that the address MAY appear in the index or
// that it definitely does not. (In other words, there are false positives but no false negatives.)
//
// We do not read the actual data into memory, choosing instead to Seek the data directly from disc. Experimentation
// teaches us that this is faster given due to the nature of the data.

package index

import (
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// The Chunk data structure consists of three parts. A FileRange, a ChunkData structure, and a ChunkBloom that
// carries set membership information for the ChunkData.
type Chunk struct {
	Range cache.FileRange
	Data  ChunkData
	Bloom ChunkBloom
}

// NewChunk returns a fully initialized index chunk. The path argument may point to either a bloom filter file or the
// index data file. Either will work. The bloom filter file must exist and will be opened for reading and its header
// will be read into memory, but the filter itself is not. The index data file need not exist (it will be downloaded
// later if the bloom indicates that its needed). If the index file does exist, however, it will be opened for reading
// and its header will be read into memory, but the index data itself will not be.
func NewChunk(path string) (chunk Chunk, err error) {
	chunk.Range, err = cache.RangeFromFilename(path)
	if err != nil {
		return
	}

	chunk.Bloom, err = NewChunkBloom(ToBloomPath(path))
	if err != nil {
		return
	}

	chunk.Data, err = NewChunkData(ToIndexPath(path))
	return
}

// String returns a JSON representation of the Chunk
func (chunk Chunk) String() string {
	s, _ := json.MarshalIndent(chunk, "", " ")
	return string(s)
}

// Close closes both the bloom filter file pointer and the index data file pointer (if they are open)
func (chunk *Chunk) Close() {
	if chunk.Bloom.File != nil {
		chunk.Bloom.File.Close()
		chunk.Bloom.File = nil
	}

	if chunk.Data.File != nil {
		chunk.Data.File.Close()
		chunk.Data.File = nil
	}
}
