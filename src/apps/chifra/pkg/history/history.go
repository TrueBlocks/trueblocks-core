package history

import (
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func getFilename(chain string) string {
	return filepath.Join(config.PathToCache(chain), "tmp/history.txt")
}

// FromHistoryBool retrieves a key/value pair from the history file
func FromHistoryBool(chain, key string) bool {
	return strings.ToLower(FromHistory(chain, key)) == "true"
}

// FromHistory retrieves a key/value pair from the history file
func FromHistory(chain, key string) string {
	historyFile := getFilename(chain)
	lines := file.AsciiFileToLines(historyFile)
	for _, line := range lines {
		if strings.HasPrefix(line, key+"=") {
			return strings.Replace(line, key+"=", "", -1)
		}
	}
	return ""
}

// ToHistory stores a key/value pair in the history file
func ToHistory(chain, key, value string) error {
	historyFile := getFilename(chain)
	lines := file.AsciiFileToLines(historyFile)
	for i, line := range lines {
		if strings.HasPrefix(line, key+"=") {
			lines[i] = key + "=" + value
			return file.LinesToAsciiFile(historyFile, lines)
		}
	}
	lines = append(lines, key+"="+value)
	return file.LinesToAsciiFile(historyFile, lines)
}
