package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func MarkdownTable(header []string, rows [][]string) string {
	wids := widths(header, rows)

	ret := []string{}
	ret = append(ret, markdownRow(header, wids))
	ret = append(ret, dividerRow(wids))
	for _, row := range rows {
		ret = append(ret, markdownRow(row, wids))
	}

	return strings.Join(ret, "\n")
}

func dividerRow(wids []int) string {
	ret := []string{}
	for _, w := range wids {
		ret = append(ret, strings.Repeat("-", w))
	}

	return "| " + strings.Join(ret, " | ") + " |"
}

func markdownRow(row []string, wids []int) string {
	if len(row) != len(wids) {
		logger.Fatal("values and wids have different lengths")
	}

	ret := []string{}
	for i := 0; i < len(row); i++ {
		field := row[i]
		ret = append(ret, Pad(field, wids[i]))
	}

	return "| " + strings.Join(ret, " | ") + " |"
}

func widths(header []string, rows [][]string) []int {
	nFields := len(header)
	wids := make([]int, nFields)
	for i := 0; i < nFields; i++ {
		wids[i] = base.Max(wids[i], len(header[i]))
		for j := 0; j < len(rows); j++ {
			if len(rows[j]) != nFields {
				logger.Fatal("fields[j] has the wrong number of fields")
			}
			wids[i] = base.Max(wids[i], len(rows[j][i]))
		}
	}

	return wids
}
