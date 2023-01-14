package types

import "github.com/ethereum/go-ethereum/common"

type SimpleTraceResult struct {
	NewContract common.Address `json:"newContract"`
	Code        string         `json:"code"`
	GasUsed     Gas            `json:"gasUsed"`
	Output      string         `json:"output"`
}
