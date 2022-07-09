package migrate

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// TODO: BOGUS - MIGRATION SENTINAL? REENTRANCY SAFE?
func HasBackLevelIndex(chain string) bool {
	knownBadFile := config.GetPathToIndex(chain) + "/finalized/013308630-013321453.bin"
	if file.FileExists(knownBadFile) {
		return true
	}
	return file.FileExists(index.ToBloomPath(knownBadFile))
}

func CheckBackLevelIndex(chain string) {
	if !HasBackLevelIndex(chain) {
		return
	}

	const BackLevelVersion string = `

	A back-level version of an index file was found. Please carefully follow all
	migrations up to and including {0} before proceeding.

	See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	[{VERSION}]

`
	msg := strings.Replace(BackLevelVersion, "{0}", "{v0.40.0-beta}", -1)
	msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
	msg = strings.Replace(msg, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	log.Fatalf(msg)
}
