package ledger10

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ReconcilerOptions struct {
	Connection   *rpc.Connection
	AccountFor   base.Address
	FirstBlock   base.Blknum
	LastBlock    base.Blknum
	AsEther      bool
	TestMode     bool
	UseTraces    bool
	Reversed     bool
	AssetFilters []base.Address
	AppFilters   *types.AppearanceFilter
}

type AssetTransfer struct {
	types.Transfer
	AmountIn         base.Wei
	AmountOut        base.Wei
	BegBal           base.Wei
	EndBal           base.Wei
	CorrectingReason string
	CorrectionId     base.Value `json:"correctionId"`
	StatementId      base.Value `json:"statementId"`
}

func (s *AssetTransfer) EndBalCalc() *base.Wei {
	return new(base.Wei).Add(&s.BegBal, s.AmountNet())
}

func (s *AssetTransfer) AmountNet() *base.Wei {
	return new(base.Wei).Sub(&s.AmountIn, &s.AmountOut)
}

func NewAssetTransfer(t types.Transfer) AssetTransfer {
	return AssetTransfer{Transfer: t}
}

func (s *AssetTransfer) Model(chain, format string, verbose bool, extraOpts map[string]any) types.Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	_, _, _, _ = chain, format, verbose, extraOpts
	check1, check2, reconciles, byCheckpoint := s.Reconciled2()
	calc := s.EndBalCalc()
	fmt.Printf("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%t\t%t\n",
		s.Asset.Hex(),
		s.Holder.Hex(),
		s.BlockNumber,
		s.TransactionIndex,
		s.LogIndex,
		s.StatementId,
		s.CorrectionId,
		s.CorrectingReason,
		s.BegBal.Text(10),
		s.AmountNet().Text(10),
		calc.Text(10),
		s.EndBal.Text(10),
		check1.Text(10),
		check2.Text(10),
		reconciles,
		byCheckpoint,
	)
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func AssetOfInterest(filters []base.Address, needle base.Address) bool {
	if len(filters) == 0 {
		return true
	}

	for _, asset := range filters {
		if asset.Hex() == needle.Hex() {
			return true
		}
	}

	return false
}

// ---------------------------------------------------------
func (p *AssetTransfer) Reconciled2() (base.Wei, base.Wei, bool, bool) {
	calc := p.EndBalCalc()
	checkVal := *new(base.Wei).Add(&p.BegBal, p.AmountNet())
	tentativeDiff := *new(base.Wei).Sub(&checkVal, calc)
	checkpointDiff := *new(base.Wei).Sub(&checkVal, &p.EndBal)

	checkpointEqual := checkVal.Equal(&p.EndBal)
	if checkpointEqual {
		return tentativeDiff, checkpointDiff, true, true
	}

	tentativeEqual := checkVal.Equal(calc)
	return tentativeDiff, checkpointDiff, tentativeEqual, false
}

// ---------------------------------------------------------
func PrintHeader() {
	fmt.Println("asset\tholder\tblockNumber\ttransactionIndex\tlogIndex\trowIndex\tcorrectionIndex\tcorrectionReason\tbegBal\tamountNet\tendBalCalc\tendBal\tcheck1\tcheck2\treconciled\tcheckpoint")
}
