package index

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_EstablishValidFilename(t *testing.T) {
	// Such a range should not be listed in manifest, so the function should fail
	r := ranges.FileRange{First: 0, Last: 1}
	man := manifest.Manifest{}
	man.ChunkMap = make(map[string]*types.ChunkRecord, 0)
	err := DownloadOneChunk("mainnet", &man, r)
	if err == nil {
		t.Fatal("error expected")
	}
}
