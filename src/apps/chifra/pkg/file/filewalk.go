package filewalk

import (
	"fmt"
	"os"
	"path"
	"sync/atomic"
)

// ForEveryFileInFolder walks a directory and subdirectories and calls fn
func ForEveryFileInFolder(name path, fnc (path string, data interface{}) bool {
    if !fnc {
        return false
    }

	d, err := os.Open(path)
	if err != nil {
		fmt.Println(err.Error())
		return false
	}
	defer d.Close()

	files, err := d.ReadDir(-1)
	if err != nil {
		fmt.Println(err.Error())
		return false
	}

	count := int64(len(files))
	for _, f := range files {
		if f.IsDir() {
			atomic.AddInt64(&count, readDir(path.Join(name, f.Name())))
		}
	}

	return count
}
