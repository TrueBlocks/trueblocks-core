package manifest

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// RemoveChunk must remove the underlying chunk (both Bloom filter and the chunk itself) and
// update the manifest by removing all chunks at or after the given path. Note that if this
// function aborts due to error and the backup files still exist, the function will attempt
// to restore the backup files before returning.
func RemoveChunk(chain, bloomFn, indexFn string) (err error) {
	manifestFn := filepath.Join(config.GetPathToChainConfig(chain), "manifest.json")

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

	if _, err = file.Copy(manifestBackup, manifestFn); err != nil {
		return err
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

	var man *Manifest
	man, err = ReadManifest(chain, base.ZeroAddr, FromCache)

	removedRange, err1 := base.RangeFromFilenameE(bloomFn)
	if err1 != nil {
		err = err1
		return err
	}

	newChunks := []ChunkRecord{}
	for _, chunk := range man.Chunks {
		chunkRange := base.RangeFromRangeString(chunk.Range)
		if chunkRange.EarlierThan(removedRange) {
			newChunks = append(newChunks, chunk)
			// 	fmt.Println(colors.Green, "Keeping", chunk.Range, colors.Off)
			// } else {
			// 	fmt.Println(colors.Red, "Removing", chunk.Range, colors.Off)
		}
	}
	man.Chunks = newChunks
	if err = man.SaveManifest(chain); err != nil {
		return err
	}

	os.Remove(manifestBackup)
	os.Remove(indexBackup)
	os.Remove(bloomBackup)

	return nil
}
