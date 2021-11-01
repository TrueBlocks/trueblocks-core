package output

import (
	"fmt"
	"os"
	"strings"
	"text/tabwriter"
)

type Table struct {
	writer *tabwriter.Writer
	target *os.File
}

func (t *Table) New() {
	t.writer = tabwriter.NewWriter(os.Stdout, 0, 0, 1, ' ', 0)
	t.target = os.Stdout
}

func (t *Table) Header(header []string) {
	fmt.Fprintln(t.writer, strings.Join(header, "\t"))
}

func (t *Table) Row(cells []string) {
	for _, cell := range cells {
		fmt.Fprint(t.writer, cell, "\t")
	}
	fmt.Fprint(t.writer, "\n")
}

func (t *Table) Print() error {
	return t.writer.Flush()
}
