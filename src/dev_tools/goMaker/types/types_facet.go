package types

import "fmt"

type Facet struct {
	Name  string   `json:"name"`
	Store string   `json:"store"`
	Cruds []string `toml:"cruds" json:"cruds"`
}

var allowedCruds = map[string]bool{
	"delete":   true,
	"undelete": true,
	"remove":   true,
	"autoname": true,
	"update":   true,
	"clean":    true,
	"cleanOne": true,
}

func (f *Facet) SingleStore() string {
	if f == nil {
		return ""
	}
	return Singular(f.Store)
}

func (f *Facet) ValidateCruds() error {
	for _, crud := range f.Cruds {
		if !allowedCruds[crud] {
			return fmt.Errorf("invalid crud value: %s", crud)
		}
	}
	return nil
}

func (f *Facet) HasCruds() bool {
	if f == nil {
		return false
	}
	return len(f.Cruds) > 0
}
