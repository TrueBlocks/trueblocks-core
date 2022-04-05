// The index package provides access to the bloom filter and address/appearance relational tables for a given
// range of blocks. The general idea is that one would query the bloom filter for the chunk, decide if a given
// address appears in the bloom and if "yes" then download the corresponding index portion of it is not
// already on the end user's machine.
package index

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// IndexChunk represents both parts of a consolidated portion of the index (represented by the Range field).
// The two parts of an IndexChunk are the BloomFilter which reveals set membership for all the addresses
// in the index portion and the Index itself which is a relational table of addresses to appearances.
type IndexChunk struct {
	Range cache.FileRange
	Index IndexData
	Bloom BloomFilter
}

// NewIndexChunk returns an index chunk with the bloom filter read in but not the index itself
func NewIndexChunk(path string) (chunk IndexChunk, err error) {
	bloomPath := toBloomPath(path)
	chunk.Range, err = cache.RangeFromFilename(bloomPath)
	if err != nil {
		return
	}

	err = chunk.Bloom.ReadBloomFilter(path)
	if err != nil {
		return
	}

	indexPath := toIndexPath(path)
	chunk.Index, err = NewIndexData(indexPath)
	return
}

// toBloomPath returns a path pointing to the bloom filter
func toBloomPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bloom") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bin", ".bloom", -1)
	ret = strings.Replace(ret, "/finalized/", "/blooms/", -1)
	return ret
}

// toIndexPath returns a path pointing to the bloom filter
func toIndexPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bin") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bloom", ".bin", -1)
	ret = strings.Replace(ret, "/blooms/", "/finalized/", -1)
	return ret
}
