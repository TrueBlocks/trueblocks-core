package index

import (
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
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
	  Chain: %s
	  Path: %s

	`
		return fmt.Errorf(indexNotInitialized, ErrNotInitialized, chain, fileName)
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
			msg = strings.ReplaceAll(msg, "WHICH", fileName)
			msg = strings.ReplaceAll(msg, "FOUND", config.VersionTags[bl.Header.Hash.Hex()])
			msg = strings.ReplaceAll(msg, "WANT", required)
			return fmt.Errorf(msg, ErrIncorrectHash)
		}
		return err
	}

	return nil
}
