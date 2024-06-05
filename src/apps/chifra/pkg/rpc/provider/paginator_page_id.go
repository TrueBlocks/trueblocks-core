package provider

import "errors"

var ErrPaginatorNoIdForPage = errors.New("no id for requested page")

// PageIdPaginator can be used for providers that use keyset pagination:
// i.e. page tokens instead of numbers
type PageIdPaginator struct {
	initialized bool

	firstPage string
	page      string
	perPage   int
	done      bool

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
	if p.Done() {
		return ErrPaginatorDone
	}
	if p.init() {
		return nil
	}
	if p.nextPage == "" {
		return ErrPaginatorNoIdForPage
	}
	p.previousPage = p.page
	p.page = p.nextPage
	p.nextPage = ""
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

func (p *PageIdPaginator) Done() bool {
	return p.done
}

func (p *PageIdPaginator) SetDone(done bool) {
	p.done = done
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
