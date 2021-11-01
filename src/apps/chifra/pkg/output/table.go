package output

import (
	"fmt"
	"os"
	"strings"
	"text/tabwriter"
)

// Table makes it easier to output tabular data to the console
type Table struct {
	writer *tabwriter.Writer
	target *os.File
}

// New sets up the default writer and target for a table
func (t *Table) New() {
	t.writer = tabwriter.NewWriter(os.Stdout, 0, 0, 1, ' ', 0)
	t.target = os.Stdout
}

// Header prints table header to the writer
func (t *Table) Header(header []string) {
	fmt.Fprintln(t.writer, strings.Join(header, "\t"))
}

// Row prints data as table cells in one row
func (t *Table) Row(cells []string) {
	for _, cell := range cells {
		fmt.Fprint(t.writer, cell, "\t")
	}
	fmt.Fprint(t.writer, "\n")
}

// Print flushes the writer, which will print the table
func (t *Table) Print() error {
	return t.writer.Flush()
}
