package utils

import (
	"path/filepath"
	"regexp"
)

func ToValidPath(path string) string {
	re := regexp.MustCompile(`[^a-zA-Z0-9_\-\.\/\\]`)
	resolved := ResolvePath(path)
	transformed := re.ReplaceAllString(resolved, "_")
	transformed = filepath.Clean(transformed)
	if len(transformed) == 0 {
		return "_"
	}
	return transformed
}
