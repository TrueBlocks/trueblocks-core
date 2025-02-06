//go:build integration
// +build integration

package ledger

import (
	"encoding/json"
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestGetStatementFromLog(t *testing.T) {
	bn := base.Blknum(9279453)
	txid := base.Txnum(208)
	log := types.Log{
		Address: base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f"),
		Topics: []base.Hash{
			transferTopic,
			base.HexToHash("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
			base.HexToHash("0x1212121212121212121212121212121212121212"),
		},
		Data:             "0xa",
		BlockNumber:      bn,
		TransactionIndex: txid,
	}
	conn := rpc.TempConnection(utils.GetTestChain())
	l := NewLedger(
		conn,
		base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		0,
		base.NOPOSN,
		true,
		false,
		false,
		false,
		false,
		nil,
	)
	tx := types.Transaction{
		BlockNumber:      bn,
		TransactionIndex: txid,
		Receipt:          &types.Receipt{},
	}
	l.theTx = &tx
	apps := make([]types.Appearance, 0, 100)
	apps = append(apps, types.Appearance{
		BlockNumber:      uint32(bn),
		TransactionIndex: uint32(txid),
	})
	l.SetContexts(apps)
	s, _ := l.getStatementsFromLog(&log)
	b, _ := json.MarshalIndent(s, "", "  ")
	fmt.Println(string(b))
	fmt.Println("reconciled:", s.Reconciled())
}
