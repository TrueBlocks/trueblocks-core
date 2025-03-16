package types

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"

type AppNode struct {
	Prev    *AppNode
	Current *Appearance
	Next    *AppNode
}

func (a *AppNode) IsSamePrev(reason string) bool {
	if reason == "token" {
		return a.PrevBlock() == a.CurBlock() && a.PrevTxId() == a.CurTxId()
	} else {
		return a.PrevBlock() == a.CurBlock()
	}
}

func (a *AppNode) IsSameNext(reason string) bool {
	if reason == "token" {
		return a.CurBlock() == a.NextBlock() && a.CurTxId() == a.NextTxId()
	} else {
		return a.CurBlock() == a.NextBlock()
	}
}

func (a *AppNode) PrevBlock() base.Blknum {
	if a.Prev.Current == nil {
		if a.Current.BlockNumber == 0 {
			return 0
		}
		return base.Blknum(a.Current.BlockNumber - 1)
	}
	return base.Blknum(a.Prev.Current.BlockNumber)
}

func (a *AppNode) PrevTxId() base.Txnum {
	if a.Prev.Current == nil {
		return 0
	}
	return base.Txnum(a.Prev.Current.TransactionIndex)
}

func (a *AppNode) CurBlock() base.Blknum {
	return base.Blknum(a.Current.BlockNumber)
}

func (a *AppNode) CurTxId() base.Txnum {
	return base.Txnum(a.Current.TransactionIndex)
}

func (a *AppNode) NextBlock() base.Blknum {
	return base.Blknum(a.Next.Current.BlockNumber)
}

func (a *AppNode) NextTxId() base.Txnum {
	return base.Txnum(a.Next.Current.TransactionIndex)
}

func (a *AppNode) SetPrev(app *Appearance) {
	if a.Prev == nil {
		a.Prev = &AppNode{}
	}
	a.Prev.Current = app
}

func (a *AppNode) SetCur(app *Appearance) {
	a.Current = app
}

func (a *AppNode) SetNext(app *Appearance) {
	if a.Next == nil {
		a.Next = &AppNode{}
	}
	a.Next.Current = app
}
