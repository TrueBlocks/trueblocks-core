package provider

import "errors"

type PageNumberPaginator struct {
	initialized bool

	firstPage int
	page      int
	perPage   int
	Done      bool
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
	if p.init() {
		return nil
	}
	if p.Done {
		return ErrPaginatorDone
	}
	p.page++
	return nil
}

func (p *PageNumberPaginator) PreviousPage() error {
	if p.init() {
		return nil
	}
	if p.Done {
		return ErrPaginatorDone
	}
	p.page--
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

func (p *PageNumberPaginator) SetPreviousPage(newPage any) error {
	return errors.New("set previous page not implemented for this paginator")
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
