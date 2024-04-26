package main

import (
	"bufio"
	"fmt"
	"os"
)

// SaveToFile writes the content of the slice to a file specified by fn.
func SaveToFile[T fmt.Stringer](fn string, slice []T) error {
	file, err := os.Create(fn)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	if _, err = writer.WriteString("[\n"); err != nil {
		return err
	}

	for i, item := range slice {
		if i > 0 {
			if _, err = writer.WriteString(","); err != nil {
				return err
			}
		}
		if writer.WriteString(fmt.Sprintf("%s\n", item.String())); err != nil {
			return err
		}
	}
	if _, err = writer.WriteString("]\n"); err != nil {
		return err
	}

	if writer.Flush(); err != nil {
		return err
	}

	return nil
}
