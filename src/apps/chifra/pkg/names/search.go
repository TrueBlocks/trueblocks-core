package names

import (
	"regexp"
)

func doSearch(name *Name, terms []string, parts Parts) bool {
	if len(terms) == 0 {
		return true
	}

	cnt := 0
	searchStr := name.Name + "\t" + name.Symbol + "\t" + name.Address + "\t" + name.Tags
	if parts&Expanded != 0 {
		searchStr += "\t" + name.Source + "\t" + name.Petname
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
