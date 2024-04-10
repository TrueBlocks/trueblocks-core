package rpc

import (
	"encoding/json"
	"errors"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var ErrApiUrl = errors.New("cannot read API URL")

type keyParam struct {
	Address string `json:"address"`
	PageId  string `json:"pageId"`
	PerPage int    `json:"perPage"`
}

type Meta struct {
	LastIndexedBlock lastIndexedBlock `json:"lastIndexedBlock,omitempty"`
	Address          string           `json:"address,omitempty"`
	PreviousPageId   string           `json:"previousPageId"`
	NextPageId       string           `json:"nextPageId"`
}

// The only purpose of this type is to parse Key's lastIndexedBlock (string) into int
type lastIndexedBlock int

func (l *lastIndexedBlock) UnmarshalJSON(data []byte) (err error) {
	var value int64
	var unquoted string
	unquoted, err = strconv.Unquote(string(data))
	if err != nil {
		return
	}

	value, err = strconv.ParseInt(unquoted, 0, 64)
	*l = lastIndexedBlock(value)
	return
}

type Data interface {
	int | []types.SimpleSlurp | []KeyAppearance
}
type keyResponse[D Data] struct {
	Data D    `json:"data"`
	Meta Meta `json:"meta"`
}

type KeyAppearance struct {
	BlockNumber      string `json:"blockNumber"`
	TransactionIndex string `json:"transactionIndex"`
}

func (k *KeyAppearance) SimpleSlurp() (s types.SimpleSlurp, err error) {
	s = types.SimpleSlurp{}
	if err = json.Unmarshal([]byte(k.BlockNumber), &s.BlockNumber); err != nil {
		return
	}
	err = json.Unmarshal([]byte(k.TransactionIndex), &s.TransactionIndex)
	return
}

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator Paginator) ([]types.SimpleSlurp, int, error) {
	url := config.GetChain(chain).KeyEndpoint
	if url == "" {
		return []types.SimpleSlurp{}, 0, ErrApiUrl
	}

	pageId, ok := paginator.Page().(string)
	if !ok {
		return []types.SimpleSlurp{}, 0, errors.New("expected page id")
	}

	method := "tb_getAppearances"
	params := query.Params{keyParam{
		Address: addr,
		PageId:  pageId,
		PerPage: paginator.PerPage(),
	}}

	if response, err := query.QueryWithHeaders[keyResponse[[]KeyAppearance]](url, nil, method, params); err != nil {
		return []types.SimpleSlurp{}, 0, err
	} else {
		v := make([]types.SimpleSlurp, 0, len(response.Data))
		for _, a := range response.Data {
			slurp, err := a.SimpleSlurp()
			if err != nil {
				return []types.SimpleSlurp{}, 0, err
			}
			v = append(v, slurp)
		}
		// update paginator
		paginator.SetNextPage(response.Meta.NextPageId)
		paginator.SetPreviousPage(response.Meta.PreviousPageId)

		return v, len(v), nil
	}
}

func (conn *Connection) getTxCountByAddressKey(chain, addr string) (int, error) {
	url := config.GetChain(chain).KeyEndpoint
	if url == "" {
		return 0, ErrApiUrl
	}

	method := "tb_getAppearanceCount"
	params := query.Params{keyParam{
		Address: addr,
	}}

	if response, err := query.QueryWithHeaders[keyResponse[int]](url, nil, method, params); err != nil {
		return 0, err
	} else {
		return response.Data, err
	}
}
