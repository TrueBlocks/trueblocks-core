package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type Posting struct {
	Statement
}

func NewPosting(bn base.Blknum, txId base.Txnum, logId base.Lognum, ts base.Timestamp) Posting {
	p := Posting{
		Statement: Statement{
			BlockNumber:      bn,
			TransactionIndex: txId,
			LogIndex:         logId,
			Timestamp:        ts,
		},
	}
	return p
}
