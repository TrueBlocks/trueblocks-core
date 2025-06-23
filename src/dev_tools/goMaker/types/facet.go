package types

type Facet struct {
	Name  string `json:"name"`
	Store string `json:"store"`
}

type FacetList []Facet

func (f *Facet) SingleStore() string {
	if f == nil {
		return ""
	}
	return Singular(f.Store)
}
