package globals

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: Why does this have a separate render routine? Why not just a RenderObject?
func (opts *GlobalOptions) RenderManifest(w io.Writer, man *manifest.Manifest) error {
	var err error
	tmp := opts
	tmp.Writer = w
	tmp.NoHeader = false
	if opts.Format == "txt" {
		var sc []types.SimpleChunkRecord
		for _, c := range man.Chunks {
			cc := types.SimpleChunkRecord{
				Range:     c.Range,
				BloomHash: c.BloomHash,
				BloomSize: c.BloomSize,
				IndexHash: c.IndexHash,
				IndexSize: c.IndexSize,
			}
			sc = append(sc, cc)
		}
		err = RenderSlice(tmp, sc)
	} else {
		err = tmp.RenderObject(man, true)
	}

	return err
}
