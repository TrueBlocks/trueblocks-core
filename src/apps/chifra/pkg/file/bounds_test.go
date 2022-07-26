package file

import (
	"fmt"
	"strings"
	"testing"
)

func Test_Latest(t *testing.T) {
	tmpPath := "./"
	files := []string{
		"000000000-000000000.bin",
		"zzzzzzzzz-555555555.bin",
		"110100101-110100101.bin",
	}
	for _, file := range files {
		Touch(file)
	}

	latest, err := LatestFileInFolder(tmpPath)
	if err != nil {
		t.Error(err)
	}

	earliest, err := EarliestFileInFolder(tmpPath)
	if err != nil {
		t.Error(err)
	}

	for _, file := range files {
		Remove(file)
	}

	if err != nil {
		t.Error("error fetching latest file")
	}

	if !strings.Contains(earliest, files[0]) {
		msg := fmt.Sprintf("Expected '%s', Got '%s'", files[0], earliest)
		t.Error(msg)
	}

	if !strings.Contains(latest, files[1]) {
		msg := fmt.Sprintf("Expected '%s', Got '%s'", files[1], latest)
		t.Error(msg)
	}
}
