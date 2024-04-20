package names

import (
	"regexp"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func doSearch(name *types.Name, terms []string, parts Parts) bool {
	if len(terms) == 0 {
		return true
	}

	cnt := 0
	searchStr := name.Name + "\t" + name.Symbol + "\t" + name.Address.Hex() + "\t" + name.Tags
	if parts&Expanded != 0 {
		searchStr += "\t" + name.Source + "\t" + name.Petname
	}
	if parts&Tags != 0 {
		// only search tags
		searchStr = name.Tags
	}

	verb := "(?i)"
	if parts&MatchCase != 0 {
		verb = "(?)"
	}
	for _, term := range terms {
		if regexp.MustCompile(verb + term).MatchString(searchStr) {
			cnt++
		}
	}

	return len(terms) <= cnt
}
