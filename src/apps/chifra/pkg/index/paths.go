package index

import (
	"strings"
	"os"
)

const (
    blooms_str    = string(os.PathSeparator) + "blooms"    + string(os.PathSeparator)
    staging_str   = string(os.PathSeparator) + "staging"   + string(os.PathSeparator)
    finalized_str = string(os.PathSeparator) + "finalized" + string(os.PathSeparator)
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
	ret := strings.Replace(pathIn, ".bin", ".bloom", -1)
	ret = strings.Replace(ret, ".txt", ".bloom", -1)
	ret = strings.Replace(ret, finalized_str, blooms_str, -1)
	ret = strings.Replace(ret, staging_str, blooms_str, -1)
	return ret
}

// ToIndexPath returns a path pointing to the index portion
func ToIndexPath(pathIn string) string {
	if isCacheType(pathIn, "finalized", "bin") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bloom", ".bin", -1)
	ret = strings.Replace(ret, ".txt", ".bin", -1)
	ret = strings.Replace(ret, blooms_str, finalized_str, -1)
	ret = strings.Replace(ret, staging_str, finalized_str, -1)
	return ret
}

// ToStagingPath returns a path pointing to the staging folder given either a neighboring path
func ToStagingPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".txt") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bin", ".txt", -1)
	ret = strings.Replace(ret, ".bloom", ".txt", -1)
	ret = strings.Replace(ret, finalized_str, staging_str, -1)
	ret = strings.Replace(ret, blooms_str, staging_str, -1)
	return ret
}
