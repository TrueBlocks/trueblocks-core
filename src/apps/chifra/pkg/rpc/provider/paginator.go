package provider

import "errors"

var ErrPaginatorDone = errors.New("calling paginator that is in done state")

// Paginator takes care of telling us either which page to fetch next or that we
// fetched all pages.
type Paginator interface {
	// Page returns current page
	Page() any

	// PerPage returns number of items we request per page
	PerPage() int
	NextPage() error

	// Done returns true if we fetched all pages
	Done() bool
	SetPage(page any) error
	SetNextPage(page any) error
	SetDone(bool)
}
