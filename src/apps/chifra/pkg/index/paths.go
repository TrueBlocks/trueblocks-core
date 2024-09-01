package index

import (
	"path/filepath"
	"strings"
)

func isCacheType(path string, folder, extension string) bool {
	if !strings.Contains(path, folder) {
		return false
	}
	if !strings.HasSuffix(path, extension) {
		return false
	}
	return true
}

// ToBloomPath returns a path pointing to the bloom filter given either a path to itself or its associated index data
func ToBloomPath(pathIn string) string {
	if isCacheType(pathIn, "blooms", "bloom") {
		return pathIn
	}
	ret := strings.Replace(filepath.Clean(pathIn), ".bin", ".bloom", -1)
	ret = strings.Replace(ret, ".txt", ".bloom", -1)
	ret = strings.Replace(ret, "finalized", "blooms", -1)
	ret = strings.Replace(ret, "staging", "blooms", -1)
	return ret
}

// ToIndexPath returns a path pointing to the index portion
func ToIndexPath(pathIn string) string {
	if isCacheType(pathIn, "finalized", "bin") {
		return pathIn
	}

	ret := strings.Replace(filepath.Clean(pathIn), ".bloom", ".bin", -1)
	ret = strings.Replace(ret, ".txt", ".bin", -1)
	ret = strings.Replace(ret, "blooms", "finalized", -1)
	ret = strings.Replace(ret, "staging", "finalized", -1)
	return ret
}

// ToStagingPath returns a path pointing to the staging folder given either a neighboring path
func ToStagingPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".txt") {
		return pathIn
	}

	ret := strings.Replace(filepath.Clean(pathIn), ".bin", ".txt", -1)
	ret = strings.Replace(ret, ".bloom", ".txt", -1)
	ret = strings.Replace(ret, "finalized", "staging", -1)
	ret = strings.Replace(ret, "blooms", "staging", -1)
	return ret
}
