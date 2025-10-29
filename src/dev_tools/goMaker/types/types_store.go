package types

import (
	"sort"
	"strings"
)

type Store struct {
	Source string     `json:"source,omitempty" toml:"source"`
	Name   string     `json:"name,omitempty" toml:"name"`
	sPtr   *Structure `json:"-" toml:"-"`
}

func NewStore(s *Structure, name string) Store {
	ret := Store{
		Source: "sdk",
		Name:   name,
		sPtr:   s,
	}
	parts := strings.Split(name, ".")
	if len(parts) > 1 {
		ret.Source = parts[0]
		ret.Name = parts[1]
	}
	return ret
}

// sdk.{{$val}}Options
func (s *Store) CountOptions() string {
	sing := Singular(s.Name)
	return s.Source + "." + "Count" + sing + "Options"
}

func (s *Store) Members() []Member {
	sing := Lower(Singular(s.Name))
	for _, st := range s.sPtr.cbPtr.Structures {
		sName := Lower(Singular(st.Class))
		if strings.EqualFold(sing, sName) {
			ret := st.Members
			sort.Slice(ret, func(i, j int) bool {
				return ret[i].DocOrder < ret[j].DocOrder
			})
			return ret
		}
	}
	return nil
}

func (s *Store) NMembers() int {
	members := s.Members()
	return len(members)
}

func (s *Structure) Stores() []Store {
	stores := make(map[string]bool)
	for _, f := range s.Facets {
		stores[f.StoreName] = true
	}
	sorted := []string{}
	for store := range stores {
		sorted = append(sorted, store)
	}
	sort.Strings(sorted)
	ret := []Store{}
	for _, storeName := range sorted {
		ret = append(ret, NewStore(s, storeName))
	}
	return ret
}

func (f *Facet) SortFunc() string {
	return "sdk.Sort" + f.Name
}

func (s *Store) NeedsBuckets() bool {
	st := s.sPtr
	for _, f := range st.Facets {
		if f.StoreName == s.Name && (f.PanelChart || f.FacetChart) {
			return true
		}
	}
	return false
}
