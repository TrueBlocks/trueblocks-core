package types

import (
	"sort"
	"strings"
)

type Store struct {
	sPtr *Structure `json:"-" toml:"-"`
	Name string     `json:"name,omitempty" toml:"name"`
}

func NewStore(s *Structure, name string) Store {
	return Store{
		sPtr: s,
		Name: name,
	}
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
