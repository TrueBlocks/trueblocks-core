package types

type Facet struct {
	Name  string `json:"name"`
	Store string `json:"store"`
}

func (f *Facet) SingleStore() string {
	if f == nil {
		return ""
	}
	return Singular(f.Store)
}
