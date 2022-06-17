package globals

import (
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// TODO: BOGUS - Complete this work - Once we go to fully using the JSON manifest this whole function can dissapear
func (opts *GlobalOptions) RenderManifest(w io.Writer, fileType string, man *manifest.Manifest) error {
	var err error
	tmp := opts
	tmp.Format = fileType
	tmp.Writer = w
	tmp.NoHeader = false
	tmp.ApiMode = false
	if fileType == "txt" {
		var sc []types.SimpleChunk
		for _, c := range man.Chunks {
			cc := types.SimpleChunk{
				FileName:  c.FileName,
				BloomHash: c.BloomHash,
				IndexHash: c.IndexHash,
			}
			sc = append(sc, cc)
		}
		err = RenderSlice(tmp, sc)
	} else {
		err = tmp.RenderObject(man, true)
	}

	if err != nil {
		return err
	}

	return nil
}
