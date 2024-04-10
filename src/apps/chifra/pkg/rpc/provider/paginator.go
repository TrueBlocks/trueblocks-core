package provider

import "errors"

var ErrPaginatorDone = errors.New("calling paginator that is in done state")

type Paginator interface {
	Page() any
	PerPage() int
	NextPage() error
	PreviousPage() error
	UpdateDone(predicate func() bool)
	SetPage(page any) error
	SetNextPage(page any) error
	SetPreviousPage(page any) error
}
