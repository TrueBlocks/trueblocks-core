package rpc

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func getUrlAndHeaders() (string, map[string]string, error) {
	key := config.GetKey("trueblocks").ApiKey
	if key == "" {
		return "", map[string]string{}, errors.New("cannot read API key")
	}

	url := "https://trueblocks.io/api/rpc"
	headers := map[string]string{
		"Authorization": "Bearer " + key,
	}

	if file.FileExists("./.key") {
		// TODO: This can be removed at some point
		lines := file.AsciiFileToLines(".key")
		if len(lines) < 6 {
			myUrl := lines[0]
			myHeaders := map[string]string{}
			for i := 1; i < len(lines); i++ {
				parts := strings.Split(lines[i], "=")
				if len(parts) != 2 {
					continue
				}
				myHeaders[parts[0]] = parts[1]
			}
			url = myUrl
			headers = myHeaders
		}
	}

	return url, headers, nil
}

type keyParam struct {
	Address string `json:"address"`
	Page    int    `json:"page"`
	PerPage int    `json:"perPage"`
}

type Meta struct {
	LastIndexedBlock int    `json:"lastIndexedBlock,omitempty"`
	Address          string `json:"address,omitempty"`
}

type Data interface{ int | []types.SimpleSlurp }
type keyResponse[D Data] struct {
	Data D    `json:"data"`
	Meta Meta `json:"meta"`
}

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	url, headers, err := getUrlAndHeaders()
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	method := "tb_getAppearances"
	params := query.Params{keyParam{
		Address: addr,
		Page:    paginator.Page,
		PerPage: paginator.PerPage,
	}}

	if response, err := query.QueryWithHeaders[keyResponse[[]types.SimpleSlurp]](url, headers, method, params); err != nil {
		return []types.SimpleSlurp{}, 0, err
	} else {
		v := make([]types.SimpleSlurp, 0, len(response.Data))
		for _, a := range response.Data {
			s := types.SimpleSlurp{
				BlockNumber:      a.BlockNumber,
				TransactionIndex: a.TransactionIndex,
			}
			v = append(v, s)
		}
		return v, len(v), nil
	}
}

func (conn *Connection) getTxCountByAddressKey(chain, addr string) (int, error) {
	url, headers, err := getUrlAndHeaders()
	if err != nil {
		return 0, err
	}

	method := "tb_getAppearanceCount"
	params := query.Params{keyParam{
		Address: addr,
	}}

	if response, err := query.QueryWithHeaders[keyResponse[int]](url, headers, method, params); err != nil {
		return 0, err
	} else {
		return response.Data, err
	}
}
