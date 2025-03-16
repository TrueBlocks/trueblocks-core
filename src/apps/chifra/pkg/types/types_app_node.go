package types

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type AppNode struct {
	prev    *AppNode
	current *Appearance
	next    *AppNode
	index   int
}

func NewAppNode(appearance *Appearance) *AppNode {
	return &AppNode{
		prev:    nil,
		current: appearance,
		next:    nil,
		index:   0,
	}
}

func (a *AppNode) Index() int {
	return a.index
}

func (a *AppNode) Prev() *AppNode {
	return a.prev
}

func (a *AppNode) PrevBlock() base.Blknum {
	if a.prev != nil {
		return base.Blknum(a.prev.current.BlockNumber)
	}
	return 0
}

func (a *AppNode) PrevTxId() base.Txnum {
	if a.prev != nil {
		return base.Txnum(a.prev.current.TransactionIndex)
	}
	return 0
}

func (a *AppNode) Cur() *Appearance {
	return a.current
}

func (a *AppNode) CurBlock() base.Blknum {
	if a.current != nil {
		return base.Blknum(a.current.BlockNumber)
	}
	panic("should never happen: AppNode.current is nil")
}

func (a *AppNode) CurTxId() base.Txnum {
	if a.current != nil {
		return base.Txnum(a.current.TransactionIndex)
	}
	panic("should never happen: AppNode.current is nil")
}

func (a *AppNode) Next() *AppNode {
	return a.next
}

func (a *AppNode) NextBlock() base.Blknum {
	if a.next != nil {
		return base.Blknum(a.next.current.BlockNumber)
	}
	return base.Blknum(a.current.BlockNumber + 1)
}

func (a *AppNode) NextTxId() base.Txnum {
	if a.next != nil {
		return base.Txnum(a.next.current.TransactionIndex)
	}
	return 0
}

func (a *AppNode) IsSamePrev(reason string) bool {
	if reason == "token" {
		return a.PrevBlock() == a.CurBlock() && a.PrevTxId() == a.CurTxId()
	}
	return a.PrevBlock() == a.CurBlock()
}

func (a *AppNode) IsSameNext(reason string) bool {
	if reason == "token" {
		return a.CurBlock() == a.NextBlock() && a.CurTxId() == a.NextTxId()
	}
	return a.CurBlock() == a.NextBlock()
}

type AppList struct {
	Head *AppNode
	Tail *AppNode
}

func NewAppList(appearances []Appearance) (*AppList, error) {
	if len(appearances) == 0 {
		return &AppList{}, nil
	}

	list := &AppList{}
	var prevNode *AppNode

	for i, app := range appearances {
		current := &Appearance{
			BlockNumber:      app.BlockNumber,
			TransactionIndex: app.TransactionIndex,
		}
		node := NewAppNode(current)
		node.index = i

		if list.Head == nil {
			list.Head = node
			if app.BlockNumber == 0 {
				prev := &Appearance{BlockNumber: 0, TransactionIndex: 0}
				prevNode = NewAppNode(prev)
				node.prev = prevNode
			} else {
				prev := &Appearance{BlockNumber: app.BlockNumber - 1, TransactionIndex: 0}
				prevNode = NewAppNode(prev)
				node.prev = prevNode
			}
		}
		if prevNode != nil && node.prev == nil {
			node.prev = prevNode
			prevNode.next = node
		}
		prevNode = node
		list.Tail = node
	}

	return list, nil
}
