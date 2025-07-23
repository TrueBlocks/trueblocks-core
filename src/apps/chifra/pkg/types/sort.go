package types

type SortOrder bool

const (
	Ascending  SortOrder = true
	Descending SortOrder = false
)

func (o SortOrder) String() string {
	if o == Ascending {
		return "ascending"
	}
	return "descending"
}
