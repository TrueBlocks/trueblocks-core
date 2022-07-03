package globals

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: BOGUS - MANIFEST WRITING THE MANIFEST
func (opts *GlobalOptions) RenderManifest(w io.Writer, man *manifest.Manifest) error {
	var err error
	tmp := opts
	tmp.Writer = w
	tmp.NoHeader = false
	tmp.ApiMode = false
	if opts.Format == "txt" {
		var sc []types.SimpleChunkRecord
		for _, c := range man.Chunks {
			cc := types.SimpleChunkRecord{
				Range:     c.Range,
				BloomHash: string(c.BloomHash),
				IndexHash: string(c.IndexHash),
			}
			sc = append(sc, cc)
		}
		err = RenderSlice(tmp, sc)
	} else {
		err = tmp.RenderObject(man, true)
	}

	return err
}
