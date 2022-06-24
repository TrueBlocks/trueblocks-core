package index

import (
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// func validateFileSize(file *os.File) error {
// 	header, err := read Header(file)
// 	if err != nil {
// 		return err
// 	}
// 	stat, err := file.Stat()
// 	if err != nil {
// 		return err
// 	}
// 	addressTableSize := AddrRecordWidth * header.AddressCount
// 	appearanceTableSize := AppRecordWidth * header.AppearanceCount
// 	expectedSize := HeaderWidth + addressTableSize + appearanceTableSize
// 	if stat.Size() != int64(expectedSize) {
// 		return fmt.Errorf("wrong file size, expected %d, but got %d", expectedSize, stat.Size())
// 	}
// 	return nil
// }

// func validateStructure(chunk ChunkData) error {
// 	// Try to read first address
// 	_, err := chunk.File.Seek(chunk.AddrTableStart, 0)
// 	if err != nil {
// 		return err
// 	}
// 	err = binary.Read(chunk.File, binary.LittleEndian, &AddressRecord{})
// 	if err != nil {
// 		return err
// 	}
// 	// Try to read first appearance
// 	_, err = chunk.File.Seek(chunk.AppTableStart, 0)
// 	if err != nil {
// 		return err
// 	}
// 	err = binary.Read(chunk.File, binary.LittleEndian, &AppearanceRecord{})
// 	if err != nil {
// 		return err
// 	}
// 	return nil
// }

// TODO: BOGUS - IF USER STOPS PROCESSING, THIS FILE WILL BE GONE, AND NEXT TIME THIS TEST WILL PASS
// TODO: BOGUS - DON'T REMOVE THIS SENTINAL FILE UNTIL WE'RE CLEAN - OR BETTER - WRITE A SENTINAL
// TODO: BOGUS - AND TEST FOR IT HERE - AND DELETE IT WHEN WE'RE DONE.
func HasBackLevelIndex(chain string) bool {
	knownBadFile := config.GetPathToIndex(chain) + "/finalized/013308630-013321453.bin"
	if file.FileExists(knownBadFile) {
		return true
	}
	return file.FileExists(ToBloomPath(knownBadFile))
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
