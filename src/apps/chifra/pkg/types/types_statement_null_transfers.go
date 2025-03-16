package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type NullTest struct {
	NLogs int
	To    base.Address
}

func (s *Statement) IsNullTransfer(nLogs int, to base.Address) bool {
	lotsOfLogs := nLogs > 10
	mayBeAirdrop := s.Sender.IsZero() || s.Sender == to
	if !lotsOfLogs && !mayBeAirdrop {
		return false
	}

	hasBalanceChange := !s.IsMaterial() || !s.EndBal.Equal(&s.BegBal)
	if hasBalanceChange {
		return false
	}

	logger.TestLog(true, "A possible nullTransfer")
	logger.TestLog(true, "  nLogs:            ", nLogs)
	logger.TestLog(true, "    lotsOfLogs:      -->", lotsOfLogs)
	logger.TestLog(true, "  Sender.IsZero:    ", s.Sender, s.Sender.IsZero())
	logger.TestLog(true, "  or Sender == To:  ", s.Sender == to)
	logger.TestLog(true, "    mayBeAirdrop:    -->", mayBeAirdrop)
	logger.TestLog(true, "  EndBal-BegBal:    ", s.EndBal.Cmp(&s.BegBal))
	logger.TestLog(true, "  Material:         ", s.IsMaterial())
	logger.TestLog(true, "    hasBalanceChange: -->", hasBalanceChange)
	return true
}

func (s *Statement) CorrectForNullTransfer() bool {
	logger.TestLog(true, "Correcting token transfer for a null transfer")
	amt := s.TotalIn() // use totalIn since this is the amount that was faked
	s.AmountOut = *base.ZeroWei
	s.AmountIn = *base.ZeroWei
	s.CorrectAmountIn = *amt
	s.CorrectAmountOut = *amt
	s.CorrectingReasons = append(s.CorrectingReasons, "nullTransfer")
	return s.Reconciled()
}
