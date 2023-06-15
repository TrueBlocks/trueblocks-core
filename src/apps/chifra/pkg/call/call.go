package call

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

type ContractCall struct {
	encoded string

	Address     base.Address
	Method      *types.SimpleFunction
	Arguments   []any
	BlockNumber uint64
}

type ContractMethodResult struct {
	BlockNumber base.Blknum
	Address     base.Address
	Name        string
	Encoding    string
	Signature   string
	Outputs     map[string]string
}

func (c *ContractMethodResult) Raw() *any {
	return nil
}

func (s *ContractMethodResult) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	callResult := map[string]any{
		"name":      s.Name,
		"signature": s.Signature,
		"encoding":  s.Encoding,
		"outputs":   s.Outputs,
	}
	var model = map[string]any{
		"blockNumber": s.BlockNumber,
		"address":     s.Address.Hex(),
		"encoding":    s.Encoding,
		"callResult":  callResult,
	}

	if format == "json" {
		return types.Model{
			Data: model,
		}
	}

	model["signature"] = s.Signature
	model["compressedResult"] = types.MakeCompressed(s.Outputs)
	order := []string{
		"blockNumber",
		"address",
		"signature",
		"encoding",
		"compressedResult",
	}

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func (c *ContractCall) ForceEncoding(encoding string) {
	c.encoded = encoding
}

func CallContract(chain string, call *ContractCall) (results *ContractMethodResult, err error) {
	blockNumberHex := "0x" + strconv.FormatUint(call.BlockNumber, 16)
	if err != nil {
		return
	}

	var packed []byte
	if call.encoded != "" {
		packed = common.Hex2Bytes(call.encoded[2:])
	} else {
		packed, err = call.Method.Pack(call.Arguments)
		if err != nil {
			return nil, err
		}
	}

	rawReturn, err := rpc.Query[string](chain, "eth_call", rpc.Params{
		map[string]any{
			"to":   call.Address.Hex(),
			"data": "0x" + common.Bytes2Hex(packed),
		},
		blockNumberHex,
	})

	if err != nil {
		return nil, err
	}

	rr := *rawReturn
	function := call.Method.Clone()
	if len(rr) > 2 {
		err = articulate.ArticulateFunction(function, "", rr[2:])
		if err != nil {
			return nil, err
		}
	}

	results = &ContractMethodResult{
		BlockNumber: call.BlockNumber,
		Address:     call.Address,
		Name:        call.Method.Name,
		Encoding:    call.Method.Encoding,
		Signature:   call.Method.Signature,
	}
	results.Outputs = make(map[string]string)

	for index, output := range function.Outputs {
		results.Outputs[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}

	return results, nil
}
