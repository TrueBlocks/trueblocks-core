package index

import (
	"errors"
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

var ErrBloomHeaderDiffMagic = errors.New("invalid magic number in bloom header")
var ErrBloomHeaderDiffHash = errors.New("invalid hash in bloom header")

var ErrIndexHeaderDiffMagic = errors.New("magic number is incorrect")
var ErrIncorrectHash = errors.New("version hash is incorrect")

// MustGetVersion returns an error if the version in the header is not as requested
func MustGetVersion(chain, required string) error {
	fileName := config.PathToIndex(chain) + "blooms/000000000-000000000.bloom"
	if !file.FileExists(fileName) {
		// It's okay if the file does not exist
		return nil
	}

	var bl Bloom
	var err error
	bl.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer func() {
		bl.File.Close()
		bl.File = nil
	}()

	_, _ = bl.File.Seek(0, io.SeekStart)   // already true, but can't hurt
	if err = bl.readHeader(); err != nil { // Note that it may not find a header, but it leaves the file pointer pointing to the count
		return err
	}

	vers := config.VersionTags[bl.Header.Hash.Hex()]
	if vers != required {
		msg := `Index file outdated (found {FOUND} - wanted {WANT}). See {https://github.com/TrueBlocks/trueblocks-core/blob/master/src/other/migrations/README-v2.0.0.md}.`
		msg = colors.ColoredWith(msg, colors.Yellow)
		msg = strings.Replace(msg, "FOUND", vers, -1)
		msg = strings.Replace(msg, "WANT", required, -1)
		return validate.Usage(msg)
	}

	return nil
}

func IsInitialized(chain string) error {
	fileName := config.PathToIndex(chain) + "finalized/000000000-000000000.bin"
	if file.FileExists(fileName) {
		return nil
	}

	const indexNotInitialized string = `

	  The Unchained Index does not appear to be initialized. You must run 'chifra init'
	  (and allow it to complete) or 'chifra scrape' before using this command.
	  
	  Path: [{PATH}]

	  [{VERSION}]

	`
	msg := strings.Replace(indexNotInitialized, "{0}", "{v0.40.0-beta}", -1)
	msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
	msg = strings.Replace(msg, "[{PATH}]", fileName, -1)
	msg = strings.Replace(msg, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	return fmt.Errorf(msg)
}
