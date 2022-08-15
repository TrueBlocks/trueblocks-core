package index

import (
	"fmt"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type SimpleProgress struct {
	Finalized string `json:"finalized"`
	Blooms    string `json:"blooms"`
	Staging   string `json:"staging"`
	Ripe      string `json:"ripe"`
	Unripe    string `json:"unripe"`
	Maps      string `json:"maps"`
	Chain     string `json:"chain"`
}

func GetProgress(chain string) (SimpleProgress, error) {
	indexPath := config.GetPathToIndex(chain)
	var ret SimpleProgress
	ret.Finalized = folderProgress(filepath.Join(indexPath, "finalized"))
	ret.Blooms = folderProgress(filepath.Join(indexPath, "blooms"))
	ret.Staging = folderProgress(filepath.Join(indexPath, "staging"))
	ret.Ripe = folderProgress(filepath.Join(indexPath, "ripe"))
	ret.Unripe = folderProgress(filepath.Join(indexPath, "unripe"))
	ret.Maps = folderProgress(filepath.Join(indexPath, "maps"))
	ret.Chain = chain
	return ret, nil
}

func folderProgress(path string) string {
	count := file.NFilesInFolder(path)
	if count == 0 {
		return "0"
	}
	early, _ := file.EarliestFileInFolder(path)
	first, _ := cache.BnsFromFilename(early)
	late, _ := file.LatestFileInFolder(path)
	last, _ := cache.BnsFromFilename(late)
	return fmt.Sprintf("%09d-%09d %d %d", first, last, (last - first + 1), count)
}
