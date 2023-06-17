package call

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	ShowLogs    bool
}

func (c *ContractCall) ForceEncoding(encoding string) {
	c.encoded = encoding
}

func CallContract(chain string, call *ContractCall) (results *types.SimpleCallResult, err error) {
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

	packedHex := "0x" + common.Bytes2Hex(packed)
	encodedArguments := ""
	if len(packedHex) > 10 {
		encodedArguments = packedHex[10:]
	}

	rawReturn, err := rpc.Query[string](chain, "eth_call", rpc.Params{
		map[string]any{
			"to":   call.Address.Hex(),
			"data": packedHex,
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

	results = &types.SimpleCallResult{
		BlockNumber:      call.BlockNumber,
		Address:          call.Address,
		Name:             call.Method.Name,
		Encoding:         call.Method.Encoding,
		Signature:        call.Method.Signature,
		EncodedArguments: encodedArguments,
	}
	results.Outputs = make(map[string]string)

	for index, output := range function.Outputs {
		results.Outputs[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}

	if call.ShowLogs {
		msg := fmt.Sprintf("call to %s at block %d at four-byte %s returned %v",
			call.Address.Hex(), call.BlockNumber, call.Method.Encoding, results.Outputs)
		logger.TestLog(true, msg)
	}

	return results, nil
}
