package types

import (
	"os"

	"github.com/gocarina/gocsv"
)

type Validater interface {
	Validate() bool
}

func LoadCsv[T Validater, D any](thePath string, callBack func(*T, *D) (bool, error), data *D) ([]T, error) {
	records := make([]T, 0)
	callbackFunc := func(record T) error {
		ok, err := callBack(&record, data)
		if err != nil {
			return err
		} else if !ok {
			return nil
		}
		if record.Validate() {
			records = append(records, record)
		}
		return nil
	}

	if theFile, err := os.OpenFile(thePath, os.O_RDWR, os.ModePerm); err != nil {
		return []T{}, err

	} else {
		defer theFile.Close()
		gocsv.TagName = "json"
		if err := gocsv.UnmarshalToCallback(theFile, callbackFunc); err != nil {
			return []T{}, err
		}
	}

	return records, nil
}
