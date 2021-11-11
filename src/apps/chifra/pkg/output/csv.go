package output

import (
	"fmt"
	"strconv"
	"strings"
)

// This type is used to carry CSV layout information
type CsvFormatted struct {
	Header  []string
	Content [][]string
}

// AsCsv turns a type into CSV string. It uses custom code instead of
// Go's encoding/csv to maintain compatibility with C++ output, which
// quotes each item. encoding/csv would double-quote a quoted string...
func AsCsv(data *CsvFormatted) string {
	quoteItems := func(items []string) string {
		quoted := []string{}
		for _, item := range items {
			quoted = append(quoted, strconv.Quote(item))
		}

		return fmt.Sprint(strings.Join(quoted, ","), "\n")
	}

	result := quoteItems(data.Header)

	for _, line := range data.Content {
		result += quoteItems(line)
	}

	return result
}
