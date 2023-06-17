package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawCallResult struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleCallResult struct {
	Address          base.Address
	BlockNumber      base.Blknum
	EncodedArguments string
	Encoding         string
	Name             string
	Outputs          map[string]string
	Signature        string
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleCallResult) Raw() *RawCallResult {
	return nil
}

func (s *SimpleCallResult) Model(verbose bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
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
		"bytes":       s.EncodedArguments,
		"callResult":  callResult,
	}

	if format == "json" {
		return Model{
			Data: model,
		}
	}

	model["signature"] = s.Signature
	model["compressedResult"] = MakeCompressed(s.Outputs)
	order := []string{
		"blockNumber",
		"address",
		"signature",
		"encoding",
		"bytes",
		"compressedResult",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
