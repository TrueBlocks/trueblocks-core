package index

import (
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

var ErrNotInitialized = errors.New("index not initialized")
var ErrIncorrectMagic = errors.New("incorrect magic number")
var ErrIncorrectHash = errors.New("incorrect header hash")

// IsInitialized returns an error if the version in the header is not as requested
func IsInitialized(chain, required string) error {
	fileName := filepath.Join(config.PathToIndex(chain), "blooms/000000000-000000000.bloom")
	if !file.FileExists(fileName) {
		const indexNotInitialized string = `

	  The Unchained Index does not appear to be initialized. You must run 'chifra init'
	  (and allow it to complete) or 'chifra scrape' before using this command.

	  Error: %w

	`
		return fmt.Errorf(indexNotInitialized, ErrNotInitialized)
	}

	var err error
	var bl Bloom
	bl.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer func() {
		bl.File.Close()
		bl.File = nil
	}()

	_, _ = bl.File.Seek(0, io.SeekStart) // already true, but can't hurt
	if err = bl.readHeader(true /* check */); err != nil {
		if errors.Is(err, ErrIncorrectHash) {
			msg := `
	Outdated file:    {WHICH}.
	File version:     {FOUND}
	Manifest version: {WANT}
	Error:            {%w}

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v2.0.0.md.
`
			msg = colors.ColoredWith(msg, colors.Yellow)
			msg = strings.Replace(msg, "WHICH", fileName, -1)
			msg = strings.Replace(msg, "FOUND", config.VersionTags[bl.Header.Hash.Hex()], -1)
			msg = strings.Replace(msg, "WANT", required, -1)
			return fmt.Errorf(msg, ErrIncorrectHash)
		}
		return err
	}

	return nil
}
