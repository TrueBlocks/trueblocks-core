package types

import (
	"sort"
	"strings"
)

type Store struct {
	Name   string     `json:"name,omitempty" toml:"name"`
	Source string     `json:"source,omitempty" toml:"source"`
	sPtr   *Structure `json:"-" toml:"-"`
}

func NewStore(s *Structure, name string) Store {
	ret := Store{
		Name:   name,
		Source: "sdk",
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

func (s *Structure) FindStore(store string) *Store {
	for _, st := range s.Stores() {
		if st.Name == store {
			return &st
		}
	}
	return nil
}

func (s *Structure) Stores() []Store {
	stores := make(map[string]bool)
	for _, f := range s.Facets {
		stores[f.Store] = true
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

func (f *Facet) StoreName() string {
	if f == nil || f.stPtr == nil {
		return ""
	}
	if store := f.stPtr.FindStore(f.Store); store != nil {
		return store.Name + " // " + store.Source
	}
	return ""
}
