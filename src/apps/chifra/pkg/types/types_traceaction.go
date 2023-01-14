package types

import "github.com/ethereum/go-ethereum/common"

type SimpleTraceAction struct {
	SelfDestructed common.Address `json:"selfDestructed"`
	Balance        Wei            `json:"balance"`
	CallType       string         `json:"callType"`
	From           common.Address `json:"from"`
	Gas            Gas            `json:"gas"`
	Init           string         `json:"init"`
	Input          string         `json:"input"`
	RefundAddress  common.Address `json:"refundAddress"`
	To             common.Address `json:"to"`
	Value          Wei            `json:"value"`
}
