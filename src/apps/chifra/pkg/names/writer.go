package names

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

var defaultHeader = []string{
	"tags",
	"address",
	"name",
	"symbol",
	"source",
	"decimals",
	"deleted",
	"isCustom",
	"isPrefund",
	"isContract",
	"isErc20",
	"isErc721",
}

type nameWriterFormat int

const (
	NameWriterCsv nameWriterFormat = iota
	NameWriterTsv
)

type NameWriter struct {
	WriteHeader   bool
	Format        nameWriterFormat
	Header        []string
	headerWritten bool
	csvWriter     *csv.Writer
}

func NewNameWriter(destWriter io.Writer) *NameWriter {
	writer := &NameWriter{
		WriteHeader: true,
		Header:      defaultHeader,
		csvWriter:   csv.NewWriter(destWriter),
	}
	writer.setFormat(NameWriterTsv)

	return writer
}

func (w *NameWriter) Write(name *types.Name) (err error) {
	if w.WriteHeader && !w.headerWritten {
		if err = w.writeHeader(); err != nil {
			return
		}
	}
	var decimals string
	if name.Decimals > 0 {
		decimals = fmt.Sprint(name.Decimals)
	}

	err = w.csvWriter.Write([]string{
		name.Tags,
		name.Address.Hex(),
		name.Name,
		name.Symbol,
		name.Source,
		decimals,
		fmt.Sprint(name.Deleted),
		fmt.Sprint(name.IsCustom),
		fmt.Sprint(name.IsPrefund),
		fmt.Sprint(name.IsContract),
		fmt.Sprint(name.IsErc20),
		fmt.Sprint(name.IsErc721),
	})

	return
}

func (w *NameWriter) writeHeader() error {
	if w.headerWritten {
		return errors.New("header has been already written")
	}
	if err := w.csvWriter.Write(w.Header); err != nil {
		return err
	}
	w.headerWritten = true
	return nil
}

func (w *NameWriter) Flush() {
	w.csvWriter.Flush()
}

func (w *NameWriter) Error() error {
	return w.csvWriter.Error()
}

func (w *NameWriter) setFormat(format nameWriterFormat) {
	comma := ','
	if format == NameWriterTsv {
		comma = '\t'
	}
	w.csvWriter.Comma = comma
	w.Format = format
}
