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
	ret := strings.ReplaceAll(filepath.Clean(pathIn), ".bin", ".bloom")
	ret = strings.ReplaceAll(ret, ".txt", ".bloom")
	ret = strings.ReplaceAll(ret, "finalized", "blooms")
	ret = strings.ReplaceAll(ret, "staging", "blooms")
	return ret
}

// ToIndexPath returns a path pointing to the index portion
func ToIndexPath(pathIn string) string {
	if isCacheType(pathIn, "finalized", "bin") {
		return pathIn
	}

	ret := strings.ReplaceAll(filepath.Clean(pathIn), ".bloom", ".bin")
	ret = strings.ReplaceAll(ret, ".txt", ".bin")
	ret = strings.ReplaceAll(ret, "blooms", "finalized")
	ret = strings.ReplaceAll(ret, "staging", "finalized")
	return ret
}

// ToStagingPath returns a path pointing to the staging folder given either a neighboring path
func ToStagingPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".txt") {
		return pathIn
	}

	ret := strings.ReplaceAll(filepath.Clean(pathIn), ".bin", ".txt")
	ret = strings.ReplaceAll(ret, ".bloom", ".txt")
	ret = strings.ReplaceAll(ret, "finalized", "staging")
	ret = strings.ReplaceAll(ret, "blooms", "staging")
	return ret
}
