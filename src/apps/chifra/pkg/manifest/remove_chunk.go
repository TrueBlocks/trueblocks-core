package manifest

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func RemoveChunk(chain, path string) (err error) {
	manifestFn := filepath.Join(config.GetPathToChainConfig(chain), "manifest.json")
	indexFn := cache.ToIndexPath(path)
	bloomFn := cache.ToBloomPath(path)

	manifestBackup := manifestFn + ".backup"
	indexBackup := indexFn + ".backup"
	bloomBackup := bloomFn + ".backup"

	defer func() {
		if err != nil {
			// If the backup files still exist when the function ends, something went wrong, reset everything
			if file.FileExists(manifestBackup) {
				file.Copy(manifestFn, manifestBackup)
			}
			if file.FileExists(indexBackup) {
				file.Copy(indexFn, indexBackup)
			}
			if file.FileExists(bloomBackup) {
				file.Copy(bloomFn, bloomBackup)
			}
			os.Remove(manifestBackup)
			os.Remove(indexBackup)
			os.Remove(bloomBackup)
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
	man, err = ReadManifest(chain, FromCache)

	rng, err1 := base.RangeFromFilenameE(path)
	if err1 != nil {
		err = err1
		return err
	}

	newChunks := []ChunkRecord{}
	for _, chunk := range man.Chunks {
		chunkRange := base.RangeFromRangeString(chunk.Range)
		if !chunkRange.Preceeds(rng, true) {
			// fmt.Println(chunkRange, rng, chunkRange.Intersects(rng))
			// } else {
			newChunks = append(newChunks, chunk)
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
