package manifest

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// RemoveChunk must remove the underlying chunk (both Bloom filter and the chunk itself) and
// update the manifest by removing all chunks at or after the given path. Note that if this
// function aborts due to error and the backup files still exist, the function will attempt
// to restore the backup files before returning.
func RemoveChunk(chain string, publisher base.Address, bloomFn, indexFn string) (err error) {
	manifestFn := config.PathToManifestFile(chain)

	manifestBackup := manifestFn + ".backup"
	indexBackup := indexFn + ".backup"
	bloomBackup := bloomFn + ".backup"

	defer func() {
		if err != nil {
			// If the backup files still exist when the function ends, something went wrong, reset everything
			if file.FileExists(manifestBackup) {
				_, _ = file.Copy(manifestFn, manifestBackup)
				_ = os.Remove(manifestBackup)
			}
			if file.FileExists(indexBackup) {
				_, _ = file.Copy(indexFn, indexBackup)
				_ = os.Remove(indexBackup)
			}
			if file.FileExists(bloomBackup) {
				_, _ = file.Copy(bloomFn, bloomBackup)
				_ = os.Remove(bloomBackup)
			}
		}
	}()

	exists := file.FileExists(manifestFn)

	var man *Manifest
	if exists {
		man, err = LoadManifest(chain, publisher, LocalCache)
		if err != nil {
			return err
		}
		if _, err = file.Copy(manifestBackup, manifestFn); err != nil {
			return err
		}
	} else {
		man = &Manifest{}
	}

	if _, err = file.Copy(indexBackup, indexFn); err != nil {
		return err
	}

	if _, err = file.Copy(bloomBackup, bloomFn); err != nil {
		return err
	}

	if err := os.Remove(indexFn); err != nil {
		return err
	}

	if err := os.Remove(bloomFn); err != nil {
		return err
	}

	if exists {
		removedRange, err1 := ranges.RangeFromFilenameE(bloomFn)
		if err1 != nil {
			err = err1
			return err
		}

		newChunks := []types.ChunkRecord{}
		for _, chunk := range man.Chunks {
			chunkRange := ranges.RangeFromRangeString(chunk.Range)
			if chunkRange.EarlierThan(removedRange) {
				newChunks = append(newChunks, chunk)
				// 	fmt.Println(colors.Green, "Keeping", chunk.Range, colors.Off)
				// } else {
				// 	fmt.Println(colors.Red, "Removing", chunk.Range, colors.Off)
			}
		}
		man.Chunks = newChunks
		if err = man.SaveManifest(chain, config.PathToManifestFile(chain)); err != nil {
			return err
		}

		os.Remove(manifestBackup)
	}

	os.Remove(indexBackup)
	os.Remove(bloomBackup)

	return nil
}
