package output

import (
	"bytes"
	"encoding/csv"
)

// AsTsv turns a type into tab-separated values
func AsTsv(data interface{}) ([]byte, error) {
	records, err := ToStringRecords(data, false)
	if err != nil {
		return nil, err
	}

	buf := &bytes.Buffer{}
	writer := csv.NewWriter(buf)
	writer.Comma = '\t'

	err = writer.WriteAll(records)
	if err != nil {
		return nil, err
	}

	err = writer.Error()
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}
