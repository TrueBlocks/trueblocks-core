package rpc

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrPaginatorEmptyNextPage = errors.New("next page not set")
var ErrPaginatorEmptyPreviousPage = errors.New("previous page not set")

type Paginator interface {
	Page() any
	PerPage() int
	NextPage() error
	PreviousPage() error
	SetPage(page any) error
	SetNextPage(page any) error
	SetPreviousPage(page any) error
}

type PageNumberPaginator struct {
	page    int
	perPage int
}

func NewPageNumberPaginator(page int, perPage int) *PageNumberPaginator {
	return &PageNumberPaginator{
		page:    page,
		perPage: perPage,
	}
}

func (p *PageNumberPaginator) Page() any {
	return p.page
}

func (p *PageNumberPaginator) PerPage() int {
	return p.perPage
}

func (p *PageNumberPaginator) NextPage() error {
	p.page++
	return nil
}

func (p *PageNumberPaginator) PreviousPage() error {
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

type PageIdPaginator struct {
	page         string
	nextPage     string
	previousPage string
	perPage      int
}

func NewPageIdPaginator(page string, perPage int) *PageIdPaginator {
	return &PageIdPaginator{
		page:    page,
		perPage: perPage,
	}
}

func (p *PageIdPaginator) Page() any {
	return p.page
}

func (p *PageIdPaginator) PerPage() int {
	return p.perPage
}

func (p *PageIdPaginator) NextPage() error {
	if p.nextPage == "" {
		return ErrPaginatorEmptyNextPage
	}

	p.previousPage = p.page
	p.page = p.nextPage
	p.nextPage = ""
	return nil
}

func (p *PageIdPaginator) PreviousPage() error {
	if p.previousPage == "" {
		return ErrPaginatorEmptyPreviousPage
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

func (conn *Connection) SlurpTxsByAddress(chain, source, addr, requestType string, paginator Paginator) ([]types.SimpleSlurp, int, error) {
	switch source {
	case "key":
		return conn.getTxsByAddressKey(chain, addr, paginator)
	case "etherscan":
		fallthrough
	default:
		return conn.getTxsByAddressEs(chain, addr, requestType, paginator)
	}
}

func (conn *Connection) SlurpTxCountByAddress(chain, source, addr, requestType string, paginator Paginator) (int, error) {
	switch source {
	case "key":
		return conn.getTxCountByAddressKey(chain, addr)
	case "etherscan":
		fallthrough
	default:
		return conn.getTxCountByAddressEs(chain, addr, requestType, paginator)
	}
}
