/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
