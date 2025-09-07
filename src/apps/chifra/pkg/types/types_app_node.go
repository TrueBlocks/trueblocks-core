package types

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type NodeAppearance struct {
	Type             string
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	LogIndex         base.Lognum
}

type AppNode[T any] struct {
	prev    *AppNode[T]
	current *NodeAppearance
	next    *AppNode[T]
	index   int
	data    *T
}

func NewAppNode[T any](appearance *NodeAppearance, data *T) *AppNode[T] {
	return &AppNode[T]{
		prev:    nil,
		current: appearance,
		next:    nil,
		index:   0,
		data:    data,
	}
}

func (a *AppNode[T]) String() string {
	return fmt.Sprintf("AppNode: %d.%d %d.%d %d.%d %t %t %t %t",
		a.PrevBlock(), a.PrevTxId(),
		a.CurBlock(), a.CurTxId(),
		a.NextBlock(), a.NextTxId(),
		a.IsHead(), a.IsTail(),
		a.IsFirstInBlock(), a.IsLastInBlock(),
	)
}

func (a *AppNode[T]) Data() *T {
	return a.data
}

func (a *AppNode[T]) SetData(data *T) {
	a.data = data
}

func (a *AppNode[T]) Index() int {
	return a.index
}

func (a *AppNode[T]) IsHead() bool {
	return a.prev == nil
}

func (a *AppNode[T]) IsTail() bool {
	return a.next == nil
}

func (a *AppNode[T]) Prev() *AppNode[T] {
	return a.prev
}

func (a *AppNode[T]) PrevBlock() base.Blknum {
	if a.IsHead() {
		return base.Blknum(max(int(a.CurBlock()), 1) - 1)
	}
	return base.Blknum(a.prev.current.BlockNumber)
}

func (a *AppNode[T]) PrevTxId() base.Txnum {
	if a.IsHead() {
		return 0
	}
	return base.Txnum(a.prev.current.TransactionIndex)
}

func (a *AppNode[T]) Cur() *NodeAppearance {
	return a.current
}

func (a *AppNode[T]) CurBlock() base.Blknum {
	if a.current != nil {
		return base.Blknum(a.current.BlockNumber)
	}
	logger.Panic("should never happen: AppNode.current is nil")
	return 0
}

func (a *AppNode[T]) CurTxId() base.Txnum {
	if a.current != nil {
		return base.Txnum(a.current.TransactionIndex)
	}
	logger.Panic("should never happen: AppNode.current is nil")
	return 0
}

func (a *AppNode[T]) Next() *AppNode[T] {
	return a.next
}

func (a *AppNode[T]) NextBlock() base.Blknum {
	if a.next != nil {
		return base.Blknum(a.next.current.BlockNumber)
	}
	return base.Blknum(a.current.BlockNumber + 1)
}

func (a *AppNode[T]) NextTxId() base.Txnum {
	if a.next != nil {
		return base.Txnum(a.next.current.TransactionIndex)
	}
	return 0
}

func (a *AppNode[T]) IsFirstInBlock() bool {
	return a.PrevBlock() != a.CurBlock()
}

func (a *AppNode[T]) IsLastInBlock() bool {
	return a.CurBlock() != a.NextBlock()
}

type AppList[T any] struct {
	Head *AppNode[T]
	Tail *AppNode[T]
}

func NewAppList[T any](apps []NodeAppearance, dataItems []*T) (*AppList[T], error) {
	if len(apps) == 0 {
		return &AppList[T]{}, nil
	}

	list := &AppList[T]{}
	var prevNode *AppNode[T]

	for i, app := range apps {
		var data *T
		if dataItems != nil && i < len(dataItems) {
			data = dataItems[i]
		}
		node := NewAppNode(&app, data)
		node.index = i

		if list.Head == nil {
			list.Head = node
		} else if prevNode != nil {
			node.prev = prevNode
			prevNode.next = node
		}
		prevNode = node
		list.Tail = node
	}

	return list, nil
}

func NewAppListFromApps[T any](apps []Appearance, dataItems []*T) (*AppList[T], error) {
	nodeApps := make([]NodeAppearance, 0, len(apps))
	for _, app := range apps {
		nodeApps = append(nodeApps, NodeAppearance{
			// Type:             app.Type,
			BlockNumber:      base.Blknum(app.BlockNumber),
			TransactionIndex: base.Txnum(app.TransactionIndex),
			// LogIndex:         app.LogIndex,
		})
	}
	return NewAppList(nodeApps, dataItems)
}

func (l *AppList[T]) Count() int {
	count := 0
	current := l.Head
	for current != nil {
		count++
		current = current.Next()
	}
	return count
}
