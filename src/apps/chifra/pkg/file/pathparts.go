package file

import (
	"path/filepath"
	"strings"
)

func GetPathParts(fullPath string) (folder string, filename string, ext string) {
	folder = filepath.Dir(fullPath)
	filename = filepath.Base(fullPath)
	ext = filepath.Ext(fullPath)
	filename = strings.ReplaceAll(filename, ext, "")
	ext = strings.ReplaceAll(ext, ".", "")
	return
}
