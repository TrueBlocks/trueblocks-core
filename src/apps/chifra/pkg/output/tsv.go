package output

import (
	"bytes"
	"encoding/csv"
)

// AsTsv turns a type into tab-separated values
func AsTsv(data *CsvFormatted) ([]byte, error) {
	buf := &bytes.Buffer{}
	writer := csv.NewWriter(buf)
	writer.Comma = '\t'

	err := writer.Write(data.Header)
	if err != nil {
		return nil, err
	}
	err = writer.WriteAll(data.Content)
	if err != nil {
		return nil, err
	}

	err = writer.Error()
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}
