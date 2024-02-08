package rpc

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	key := config.GetKey("trueblocks").ApiKey
	if key == "" {
		return []types.SimpleSlurp{}, 0, errors.New("cannot read API key")
	}

	url := "https://trueblocks.io/api/rpc"
	headers := map[string]string{
		"Authorization": "Bearer " + key,
	}

	if file.FileExists("./.key") {
		lines := file.AsciiFileToLines(".key")
		if len(lines) != 6 {
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

	type keyParam struct {
		Address  string `json:"address"`
		Page     int    `json:"page"`
		PageSize int    `json:"pageSize"`
	}

	type Shit struct {
		BlockNumber      int `json:"blockNumber"`
		TransactionIndex int `json:"transactionId"`
	}

	method := "tb_getAppearances"
	params := query.Params{keyParam{
		Address:  addr,
		Page:     paginator.Page,
		PageSize: paginator.PerPage,
	}}

	if apps, err := query.QueryWithHeaders[[]Shit](url, headers, method, params); err != nil {
		return []types.SimpleSlurp{}, 0, err
	} else {
		v := make([]types.SimpleSlurp, 0, len(*apps))
		for _, a := range *apps {
			v = append(v, types.SimpleSlurp{
				BlockNumber:      uint64(a.BlockNumber),
				TransactionIndex: uint64(a.TransactionIndex),
			})
		}
		return v, len(*apps), nil
	}
}

func (conn *Connection) getTxCountByAddressKey(chain, addr string, paginator *Paginator) (int, error) {
	_, cnt, err := conn.getTxsByAddressKey(chain, addr, paginator)
	return cnt, err
}
