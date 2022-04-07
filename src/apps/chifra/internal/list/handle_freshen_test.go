package listPkg

import (
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func Test_establishIndexChunkValidFilename(t *testing.T) {
	// Such a range should not be listed in manifest, so the function will fail
	// and we can investigate
	fileRange := cache.FileRange{
		First: 0,
		Last:  1,
	}

	_, err := establishIndexChunk("mainnet", cache.FilenameFromRange(fileRange, "bloom"))
	if err == nil {
		t.Fatal("error expected")
	}
	if strings.Contains(err.Error(), "000000000-000000001") == false {
		t.Fatal("error message should include the file name:", err)
	}
}
