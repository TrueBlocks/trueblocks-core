package index

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Turn these tests back on if possible
func Test_EstablishValidFilename(t *testing.T) {
	// Such a range should not be listed in manifest, so the function should fail
	r := base.FileRange{First: 0, Last: 1}
	man := manifest.Manifest{}
	man.ChunkMap = make(map[string]*types.SimpleChunkRecord, 0)
	_, err := DownloadOneChunk("mainnet", &man, r)
	if err == nil {
		t.Fatal("error expected")
	}
}
