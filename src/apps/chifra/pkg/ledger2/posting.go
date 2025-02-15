package ledger2

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// Posting is a single line item that records a material asset movement against the AccountedFor address.
type Posting struct {
	BlockNumber       base.Blknum
	TransactionIndex  base.Txnum
	Index             string
	TransferIn        base.Wei
	MiningRewardIn    base.Wei
	UncleRewardIn     base.Wei
	WithdrawalIn      base.Wei
	SelfDestructIn    base.Wei
	PrefundIn         base.Wei
	CorrectingIn      base.Wei
	TransferOut       base.Wei
	GasOut            base.Wei
	InternalTxFeesOut base.Wei
	CorrectingOut     base.Wei
	CorrectingReason  string
	RunningBalance    base.Wei
	Timestamp         base.Timestamp
	From              base.Address
	To                base.Address
}

// NewPosting creates a Posting with the essential fields set.
func NewPosting(blockNumber base.Blknum, txIndex base.Txnum, index string, timestamp base.Timestamp) Posting {
	return Posting{
		BlockNumber:      blockNumber,
		TransactionIndex: txIndex,
		Index:            index,
		Timestamp:        timestamp,
	}
}

// String returns a human-readable summary of the Posting.
func (p *Posting) String() string {
	totIn := p.TotalIn()
	totOut := p.TotalOut()
	net := p.NetValue()
	return fmt.Sprintf(
		"Posting(Block=%d Tx=%d Index=%s In=%s Out=%s Net=%s Time=%d)",
		p.BlockNumber,
		p.TransactionIndex,
		p.Index,
		totIn.String(),
		totOut.String(),
		net.String(),
		p.Timestamp,
	)
}

// TotalIn returns the sum of all incoming fields in the Posting.
func (p *Posting) TotalIn() base.Wei {
	total := base.NewWei(0)
	total = total.Add(total, &p.TransferIn)
	total = total.Add(total, &p.MiningRewardIn)
	total = total.Add(total, &p.UncleRewardIn)
	total = total.Add(total, &p.WithdrawalIn)
	total = total.Add(total, &p.SelfDestructIn)
	total = total.Add(total, &p.PrefundIn)
	total = total.Add(total, &p.CorrectingIn)
	return *total
}

// TotalOut returns the sum of all outgoing fields in the Posting.
func (p *Posting) TotalOut() base.Wei {
	total := base.NewWei(0)
	total = total.Add(total, &p.TransferOut)
	total = total.Add(total, &p.GasOut)
	total = total.Add(total, &p.InternalTxFeesOut)
	total = total.Add(total, &p.CorrectingOut)
	return *total
}

// NetValue is the difference between total incoming and total outgoing fields in the Posting.
func (p *Posting) NetValue() base.Wei {
	in := p.TotalIn()
	out := p.TotalOut()
	net := base.NewWei(0)
	net = net.Sub(&in, &out)
	return *net
}
