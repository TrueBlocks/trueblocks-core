package index

import (
	"strings"
)

type CacheType uint

const (
	Cache_NotACache CacheType = iota
	Index_Bloom
	Index_Final
	Index_Staging
)

// cacheTypeToFolder is a map of cache types to the folder name (also, it acts as the mode in chifra status)
var cacheTypeToFolder = map[CacheType]string{
	Cache_NotACache: "unknown",
	Index_Bloom:     "blooms",
	Index_Final:     "finalized",
	Index_Staging:   "staging",
}

var cacheTypeToExt = map[CacheType]string{
	Cache_NotACache: "unknown",
	Index_Bloom:     "bloom",
	Index_Final:     "bin",
	Index_Staging:   "txt",
}

func IsCacheType(path string, cT CacheType, checkExt bool) bool {
	if !strings.Contains(path, cacheTypeToFolder[cT]) {
		return false
	}
	if checkExt && !strings.HasSuffix(path, cacheTypeToExt[cT]) {
		return false
	}
	return true
}

// ToBloomPath returns a path pointing to the bloom filter given either a path to itself or its associated index data
func ToBloomPath(pathIn string) string {
	if IsCacheType(pathIn, Index_Bloom, true /* checkExt */) {
		return pathIn
	}
	ret := strings.Replace(pathIn, ".bin", ".bloom", -1)
	ret = strings.Replace(ret, ".txt", ".bloom", -1)
	ret = strings.Replace(ret, "/finalized/", "/blooms/", -1)
	ret = strings.Replace(ret, "/staging/", "/blooms/", -1)
	return ret
}

// ToIndexPath returns a path pointing to the bloom filter
func ToIndexPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bin") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bloom", ".bin", -1)
	ret = strings.Replace(ret, ".txt", ".bin", -1)
	ret = strings.Replace(ret, "/blooms/", "/finalized/", -1)
	ret = strings.Replace(ret, "/staging/", "/finalized/", -1)
	return ret
}

// ToStagingPath returns a path pointing to the staging folder given either a neighboring path
func ToStagingPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".txt") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bin", ".txt", -1)
	ret = strings.Replace(ret, ".bloom", ".txt", -1)
	ret = strings.Replace(ret, "/finalized/", "/staging/", -1)
	ret = strings.Replace(ret, "/blooms/", "/staging/", -1)
	return ret
}
