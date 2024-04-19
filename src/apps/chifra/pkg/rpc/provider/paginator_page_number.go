package provider

import "errors"

// PageNumberPaginator is a paginator for providers that use page numbers
type PageNumberPaginator struct {
	initialized bool

	firstPage int
	page      int
	perPage   int
	done      bool
}

func NewPageNumberPaginator(page int, firstPage int, perPage int) *PageNumberPaginator {
	return &PageNumberPaginator{
		firstPage: firstPage,
		page:      page,
		perPage:   perPage,
	}
}

func (p *PageNumberPaginator) Page() any {
	p.init()
	return p.page
}

func (p *PageNumberPaginator) PerPage() int {
	return p.perPage
}

func (p *PageNumberPaginator) NextPage() error {
	if p.Done() {
		return ErrPaginatorDone
	}
	if p.init() {
		return nil
	}
	p.page++
	return nil
}

func (p *PageNumberPaginator) SetPage(newPage any) error {
	i, ok := newPage.(int)
	if !ok {
		return errors.New("page number expected")
	}
	p.page = i
	return nil
}

func (p *PageNumberPaginator) SetNextPage(newPage any) error {
	return errors.New("set next page not implemented for this paginator")
}

func (p *PageNumberPaginator) Done() bool {
	return p.done
}

func (p *PageNumberPaginator) SetDone(done bool) {
	p.done = done
}

func (p *PageNumberPaginator) init() bool {
	if p.initialized {
		return false
	}

	if p.firstPage > p.page {
		p.page = p.firstPage
	}
	p.initialized = true
	return true
}
