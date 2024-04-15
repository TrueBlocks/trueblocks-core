package provider

import "errors"

var ErrPaginatorNoIdForPage = errors.New("no id for requested page")

type PageIdPaginator struct {
	initialized bool

	firstPage string
	page      string
	perPage   int
	Done      bool

	nextPage     string
	previousPage string
}

func NewPageIdPaginator(page string, firstPage string, perPage int) *PageIdPaginator {
	return &PageIdPaginator{
		firstPage: firstPage,
		page:      page,
		perPage:   perPage,
	}
}

func (p *PageIdPaginator) Page() any {
	p.init()
	return p.page
}

func (p *PageIdPaginator) PerPage() int {
	return p.perPage
}

func (p *PageIdPaginator) NextPage() error {
	if p.init() {
		return nil
	}
	if p.Done {
		return ErrPaginatorDone
	}
	if p.nextPage == "" {
		return ErrPaginatorNoIdForPage
	}
	p.previousPage = p.page
	p.page = p.nextPage
	p.nextPage = ""
	return nil
}

func (p *PageIdPaginator) PreviousPage() error {
	if p.init() {
		return nil
	}
	if p.Done {
		return ErrPaginatorDone
	}
	if p.previousPage == "" {
		return ErrPaginatorNoIdForPage
	}
	p.nextPage = p.page
	p.page = p.previousPage
	p.previousPage = ""
	return nil
}

func (p *PageIdPaginator) SetPage(newPage any) error {
	s, err := p.castPageId(newPage)
	if err != nil {
		return err
	}
	p.page = s
	return nil
}

func (p *PageIdPaginator) SetNextPage(newPage any) error {
	s, err := p.castPageId(newPage)
	if err != nil {
		return err
	}
	p.nextPage = s
	return nil
}

func (p *PageIdPaginator) SetPreviousPage(newPage any) error {
	s, err := p.castPageId(newPage)
	if err != nil {
		return err
	}
	p.previousPage = s
	return nil
}

func (p *PageIdPaginator) castPageId(input any) (string, error) {
	s, ok := input.(string)
	if !ok {
		return "", errors.New("page id expected")
	}
	return s, nil
}

func (p *PageIdPaginator) init() bool {
	if p.initialized {
		return false
	}

	if p.page == "" {
		p.page = p.firstPage
	}
	p.initialized = true
	return true
}
