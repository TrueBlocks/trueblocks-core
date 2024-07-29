package file

import "path/filepath"

func GetPathParts(fullPath string) (folder string, filename string, ext string) {
	folder = filepath.Dir(fullPath)
	filename = filepath.Base(fullPath)
	ext = filepath.Ext(fullPath)
	return
}
