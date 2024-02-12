package rpc

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func getUrlAndHeaders() (string, map[string]string, bool, error) {
	key := config.GetKey("trueblocks").ApiKey
	if key == "" {
		return "", map[string]string{}, false, errors.New("cannot read API key")
	}

	url := "https://trueblocks.io/api/rpc"
	headers := map[string]string{
		"Authorization": "Bearer " + key,
	}

	isDev := false
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
			isDev = true
			url = myUrl
			headers = myHeaders
		}
	}

	return url, headers, isDev, nil
}

type keyParam struct {
	Address string `json:"address"`
	Page    int    `json:"page"`
	PerPage int    `json:"perPage"`
}

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	url, headers, isDev, err := getUrlAndHeaders()
	if err != nil {
		return []types.SimpleSlurp{}, 0, err
	}

	method := "tb_getAppearances"
	params := query.Params{keyParam{
		Address: addr,
		Page:    paginator.Page,
		PerPage: paginator.PerPage,
	}}

	if apps, err := query.QueryWithHeaders[[]types.SimpleSlurp](url, headers, method, params); err != nil {
		return []types.SimpleSlurp{}, 0, err
	} else {
		// TODO: This can be removed when we fix
		// TODO: https://github.com/TrueBlocks/trueblocks-key/issues/82
		v := make([]types.SimpleSlurp, 0, len(*apps))
		for _, a := range *apps {
			s := types.SimpleSlurp{
				BlockNumber:      a.BlockNumber,
				TransactionIndex: a.TransactionIndex,
			}
			if isDev {
				s.TransactionIndex = a.TempTransactionId
			}
			v = append(v, s)
		}
		return v, len(v), nil
	}
}

func (conn *Connection) getTxCountByAddressKey(chain, addr string) (int, error) {
	url, headers, _, err := getUrlAndHeaders()
	if err != nil {
		return 0, err
	}

	method := "tb_getAppearanceCount"
	params := query.Params{keyParam{
		Address: addr,
	}}

	if cntPtr, err := query.QueryWithHeaders[int](url, headers, method, params); err != nil {
		return 0, err
	} else {
		return *cntPtr, err
	}
}
