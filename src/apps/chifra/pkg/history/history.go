package history

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
)

// FromHistoryBool retrieves a key/value pair from the history file
func FromHistoryBool(historyFile, key string) bool {
	return strings.ToLower(FromHistory(historyFile, key)) == "true"
}

// FromHistory retrieves a key/value pair from the history file
func FromHistory(historyFile, key string) string {
	lines := file.AsciiFileToLines(historyFile)
	for _, line := range lines {
		if strings.HasPrefix(line, key+"=") {
			return strings.ReplaceAll(line, key+"=", "")
		}
	}
	return ""
}

// ToHistory stores a key/value pair in the history file
func ToHistory(historyFile, key, value string) error {
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
