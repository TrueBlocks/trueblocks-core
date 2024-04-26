package main

import (
	"bufio"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
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

func SaveAndClean[T fmt.Stringer](fn string, slice []T, g sdk.Cacher, cleanFunc func() error) error {
	if err := SaveToFile(fn, slice); err != nil {
		return err
	}

	g.Caching(sdk.Decache)
	fmt.Println("Is it there?")
	usage.Wait()
	if err := cleanFunc(); err != nil {
		return err
	}
	fmt.Println("Is it gone?")
	usage.Wait()
	g.Caching(sdk.CacheOn)

	return nil
}
