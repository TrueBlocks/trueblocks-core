package types

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestAppNodeCreation(t *testing.T) {
	app := &Appearance{BlockNumber: 100, TransactionIndex: 1}
	node := NewAppNode(app)

	if node.current != app {
		t.Errorf("Expected current to be %v, got %v", app, node.current)
	}
	if node.prev != nil {
		t.Errorf("Expected prev to be nil, got %v", node.prev)
	}
	if node.next != nil {
		t.Errorf("Expected next to be nil, got %v", node.next)
	}
	if node.Index() != 0 {
		t.Errorf("Expected index to be 0, got %d", node.Index())
	}
	if node.Cur() != app {
		t.Errorf("Expected Cur() to be %v, got %v", app, node.Cur())
	}
	if node.Prev() != nil {
		t.Errorf("Expected Prev() to be nil, got %v", node.Prev())
	}
	if node.Next() != nil {
		t.Errorf("Expected Next() to be nil, got %v", node.Next())
	}
}

func TestAppNodeAccessors(t *testing.T) {
	app := &Appearance{BlockNumber: 100, TransactionIndex: 1}
	node := NewAppNode(app)

	tests := []struct {
		name     string
		got      base.Value
		expected base.Value
	}{
		{"CurBlock", node.CurBlock(), base.Blknum(100)},
		{"CurTxId", node.CurTxId(), base.Txnum(1)},
		{"PrevBlock", node.PrevBlock(), base.Blknum(0)},
		{"PrevTxId", node.PrevTxId(), base.Txnum(0)},
		{"NextBlock", node.NextBlock(), base.Blknum(101)},
		{"NextTxId", node.NextTxId(), base.Txnum(0)},
		{"Index", base.Value(node.Index()), base.Value(0)},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if tt.got != tt.expected {
				t.Errorf("%s: expected %d, got %d", tt.name, tt.expected, tt.got)
			}
		})
	}

	list, _ := NewAppList([]Appearance{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 101, TransactionIndex: 0},
	})

	head := list.Head
	middle := head.Next()
	tail := list.Tail

	if head.Prev() == nil {
		t.Errorf("Head.Prev() should not be nil for non-zero block")
	} else if head.Prev().CurBlock() != base.Blknum(99) || head.Prev().CurTxId() != 0 {
		t.Errorf("Head.Prev() should be {99, 0}, got %d:%d", head.Prev().CurBlock(), head.Prev().CurTxId())
	}
	if middle.Prev() != head {
		t.Errorf("Middle.Prev() should be head, got %v", middle.Prev())
	}
	if tail.Prev() != middle {
		t.Errorf("Tail.Prev() should be middle, got %v", tail.Prev())
	}

	if head.Cur().BlockNumber != 100 || head.Cur().TransactionIndex != 1 {
		t.Errorf("Head.Cur() = %v, want {100, 1}", head.Cur())
	}
	if middle.Cur().BlockNumber != 100 || middle.Cur().TransactionIndex != 2 {
		t.Errorf("Middle.Cur() = %v, want {100, 2}", middle.Cur())
	}
	if tail.Cur().BlockNumber != 101 || tail.Cur().TransactionIndex != 0 {
		t.Errorf("Tail.Cur() = %v, want {101, 0}", tail.Cur())
	}

	if head.Next() != middle {
		t.Errorf("Head.Next() should be middle, got %v", head.Next())
	}
	if middle.Next() != tail {
		t.Errorf("Middle.Next() should be tail, got %v", middle.Next())
	}
	if tail.Next() != nil {
		t.Errorf("Tail.Next() should be nil, got %v", tail.Next())
	}

	if head.Index() != 0 {
		t.Errorf("Head.Index() should be 0, got %d", head.Index())
	}
	if middle.Index() != 1 {
		t.Errorf("Middle.Index() should be 1, got %d", middle.Index())
	}
	if tail.Index() != 2 {
		t.Errorf("Tail.Index() should be 2, got %d", tail.Index())
	}

	zeroList, _ := NewAppList([]Appearance{{BlockNumber: 0, TransactionIndex: 1}})
	if zeroList.Head.Prev() == nil {
		t.Errorf("Zero block head should have prev, got nil")
	}
	if zeroList.Head.Prev().CurBlock() != 0 || zeroList.Head.Prev().CurTxId() != 0 {
		t.Errorf("Zero block prev should be {0, 0}, got %d:%d", zeroList.Head.Prev().CurBlock(), zeroList.Head.Prev().CurTxId())
	}
}

func TestAppNodeIsSame(t *testing.T) {
	list, _ := NewAppList([]Appearance{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 101, TransactionIndex: 0},
	})

	tests := []struct {
		name   string
		node   *AppNode
		reason string
		isPrev bool
		isNext bool
	}{
		{
			"HeadBlockOnly",
			list.Head,
			"block",
			false,
			true,
		},
		{
			"HeadToken",
			list.Head,
			"token",
			false,
			false,
		},
		{
			"MiddleBlockOnly",
			list.Head.next,
			"block",
			true,
			false,
		},
		{
			"TailToken",
			list.Tail,
			"token",
			false,
			false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := tt.node.IsSamePrev(tt.reason); got != tt.isPrev {
				t.Errorf("IsSamePrev: expected %v, got %v", tt.isPrev, got)
			}
			if got := tt.node.IsSameNext(tt.reason); got != tt.isNext {
				t.Errorf("IsSameNext: expected %v, got %v", tt.isNext, got)
			}
		})
	}
}

func TestNewAppList(t *testing.T) {
	tests := []struct {
		name        string
		appearances []Appearance
		wantHead    *Appearance
		wantTail    *Appearance
		wantErr     bool
	}{
		{
			"EmptyList",
			[]Appearance{},
			nil,
			nil,
			false,
		},
		{
			"SingleItem",
			[]Appearance{{BlockNumber: 100, TransactionIndex: 1}},
			&Appearance{BlockNumber: 100, TransactionIndex: 1},
			&Appearance{BlockNumber: 100, TransactionIndex: 1},
			false,
		},
		{
			"MultipleItems",
			[]Appearance{
				{BlockNumber: 100, TransactionIndex: 1},
				{BlockNumber: 100, TransactionIndex: 2},
				{BlockNumber: 101, TransactionIndex: 0},
			},
			&Appearance{BlockNumber: 100, TransactionIndex: 1},
			&Appearance{BlockNumber: 101, TransactionIndex: 0},
			false,
		},
		{
			"SingleMaxValues",
			[]Appearance{{BlockNumber: ^uint32(0), TransactionIndex: ^uint32(0)}},
			&Appearance{BlockNumber: ^uint32(0), TransactionIndex: ^uint32(0)},
			&Appearance{BlockNumber: ^uint32(0), TransactionIndex: ^uint32(0)},
			false,
		},
		{
			"SingleZeroBlock",
			[]Appearance{{BlockNumber: 0, TransactionIndex: 1}},
			&Appearance{BlockNumber: 0, TransactionIndex: 1},
			&Appearance{BlockNumber: 0, TransactionIndex: 1},
			false,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			list, err := NewAppList(tt.appearances)
			if (err != nil) != tt.wantErr {
				t.Errorf("NewAppList() error = %v, wantErr %v", err, tt.wantErr)
				return
			}

			if list.Head == nil && tt.wantHead != nil {
				t.Errorf("Expected head to be non-nil")
			} else if list.Head != nil && (list.Head.current.BlockNumber != tt.wantHead.BlockNumber ||
				list.Head.current.TransactionIndex != tt.wantHead.TransactionIndex) {
				t.Errorf("Head = %v, want %v", list.Head.current, tt.wantHead)
			}

			if list.Tail == nil && tt.wantTail != nil {
				t.Errorf("Expected tail to be non-nil")
			} else if list.Tail != nil && (list.Tail.current.BlockNumber != tt.wantTail.BlockNumber ||
				list.Tail.current.TransactionIndex != tt.wantTail.TransactionIndex) {
				t.Errorf("Tail = %v, want %v", list.Tail.current, tt.wantTail)
			}

			if list.Head != nil {
				if list.Head.Index() != 0 {
					t.Errorf("Head index should be 0, got %d", list.Head.Index())
				}
				if tt.name == "SingleZeroBlock" {
					if list.Head.Prev() == nil {
						t.Errorf("Zero block head should have prev, got nil")
					} else if list.Head.Prev().CurBlock() != 0 || list.Head.Prev().CurTxId() != 0 {
						t.Errorf("Zero block prev should be {0, 0}, got %d:%d", list.Head.Prev().CurBlock(), list.Head.Prev().CurTxId())
					}
				} else if list.Head.Prev() == nil {
					t.Errorf("Non-zero block head should have prev, got nil")
				} else if tt.name == "SingleItem" && (list.Head.Prev().CurBlock() != base.Blknum(99) || list.Head.Prev().CurTxId() != 0) {
					t.Errorf("SingleItem prev should be {99, 0}, got %d:%d", list.Head.Prev().CurBlock(), list.Head.Prev().CurTxId())
				}
				if list.Head.Cur() != list.Head.current {
					t.Errorf("Head Cur() should match current, got %v", list.Head.Cur())
				}
			}

			if list.Tail != nil {
				if list.Tail.Next() != nil {
					t.Errorf("Tail next should be nil, got %v", list.Tail.Next())
				}
				if list.Tail.Cur() != list.Tail.current {
					t.Errorf("Tail Cur() should match current, got %v", list.Tail.Cur())
				}
				if len(tt.appearances) > 0 && list.Tail.Index() != len(tt.appearances)-1 {
					t.Errorf("Tail index should be %d, got %d", len(tt.appearances)-1, list.Tail.Index())
				}
			}
		})
	}
}

func TestListLinking(t *testing.T) {
	list, _ := NewAppList([]Appearance{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 101, TransactionIndex: 0},
	})

	current := list.Head
	if current.Prev() == nil {
		t.Errorf("Head.prev should not be nil for non-zero block")
	}
	for i := 0; current.Next() != nil; i++ {
		if current.Next().Prev() != current {
			t.Errorf("Node %d next.prev link broken", i)
		}
		current = current.Next()
	}
	if current != list.Tail {
		t.Errorf("Last node should be tail")
	}

	current = list.Tail
	if current.Next() != nil {
		t.Errorf("Tail.next should be nil")
	}
	for i := 0; current.Prev() != nil; i++ {
		if current.Prev().Next() != current {
			t.Errorf("Node %d prev.next link broken", i)
		}
		current = current.Prev()
	}
	if current != list.Head {
		t.Errorf("First node should be head")
	}

	zeroList, _ := NewAppList([]Appearance{{BlockNumber: 0, TransactionIndex: 1}})
	if zeroList.Head.Prev() == nil {
		t.Errorf("Zero block head should have prev, got nil")
	}
}

func TestEdgeCaseAccessors(t *testing.T) {
	list, _ := NewAppList([]Appearance{
		{BlockNumber: 0, TransactionIndex: 0},
		{BlockNumber: ^uint32(0), TransactionIndex: ^uint32(0)},
	})

	head := list.Head
	if head.PrevBlock() != 0 || head.PrevTxId() != 0 {
		t.Errorf("Head prev accessors should return 0s, got %d:%d",
			head.PrevBlock(), head.PrevTxId())
	}
	if head.Index() != 0 {
		t.Errorf("Head index should be 0, got %d", head.Index())
	}
	if head.Prev() == nil {
		t.Errorf("Zero block head should have prev, got nil")
	}
	if head.Prev().CurBlock() != 0 || head.Prev().CurTxId() != 0 {
		t.Errorf("Zero block prev should be {0, 0}, got %d:%d", head.Prev().CurBlock(), head.Prev().CurTxId())
	}
	if head.Cur().BlockNumber != 0 || head.Cur().TransactionIndex != 0 {
		t.Errorf("Head Cur() should be {0, 0}, got %v", head.Cur())
	}

	tail := list.Tail
	expectedNext := base.Blknum(tail.current.BlockNumber + 1)
	if tail.NextBlock() != expectedNext {
		t.Errorf("Tail next block should be %d, got %d", expectedNext, tail.NextBlock())
	}
	if tail.Index() != 1 {
		t.Errorf("Tail index should be 1, got %d", tail.Index())
	}
	if tail.Next() != nil {
		t.Errorf("Tail Next() should be nil, got %v", tail.Next())
	}
	if tail.Cur().BlockNumber != ^uint32(0) || tail.Cur().TransactionIndex != ^uint32(0) {
		t.Errorf("Tail Cur() should be {%d, %d}, got %v", ^uint32(0), ^uint32(0), tail.Cur())
	}

	emptyList, _ := NewAppList([]Appearance{})
	if emptyList.Head != nil {
		t.Errorf("Empty list Head should be nil, got %v", emptyList.Head)
	}
	if emptyList.Tail != nil {
		t.Errorf("Empty list Tail should be nil, got %v", emptyList.Tail)
	}

	nonZeroList, _ := NewAppList([]Appearance{{BlockNumber: 100, TransactionIndex: 1}})
	if nonZeroList.Head.Prev() == nil {
		t.Errorf("Non-zero block head should have prev, got nil")
	} else if nonZeroList.Head.Prev().CurBlock() != base.Blknum(99) || nonZeroList.Head.Prev().CurTxId() != 0 {
		t.Errorf("Non-zero block prev should be {99, 0}, got %d:%d", nonZeroList.Head.Prev().CurBlock(), nonZeroList.Head.Prev().CurTxId())
	}
}

func TestListTraversal(t *testing.T) {
	list, _ := NewAppList([]Appearance{
		{BlockNumber: 100, TransactionIndex: 1},
		{BlockNumber: 100, TransactionIndex: 2},
		{BlockNumber: 101, TransactionIndex: 0},
	})

	expected := []struct {
		block base.Blknum
		txId  base.Txnum
		index int
	}{
		{base.Blknum(100), base.Txnum(1), 0},
		{base.Blknum(100), base.Txnum(2), 1},
		{base.Blknum(101), base.Txnum(0), 2},
	}

	i := 0
	for node := list.Head; node != nil; node = node.Next() {
		if i >= len(expected) {
			t.Errorf("More nodes than expected")
			break
		}
		if node.CurBlock() != expected[i].block {
			t.Errorf("Node %d: expected block %d, got %d", i, expected[i].block, node.CurBlock())
		}
		if node.CurTxId() != expected[i].txId {
			t.Errorf("Node %d: expected txId %d, got %d", i, expected[i].txId, node.CurTxId())
		}
		if node.Index() != expected[i].index {
			t.Errorf("Node %d: expected index %d, got %d", i, expected[i].index, node.Index())
		}
		i++
	}
	if i != len(expected) {
		t.Errorf("Expected %d nodes, traversed %d", len(expected), i)
	}

	i = len(expected) - 1
	for node := list.Tail; node != nil; node = node.Prev() {
		if i < 0 {
			t.Errorf("More nodes than expected in backward traversal")
			break
		}
		if node.CurBlock() != expected[i].block {
			t.Errorf("Node %d: expected block %d, got %d", i, expected[i].block, node.CurBlock())
		}
		if node.CurTxId() != expected[i].txId {
			t.Errorf("Node %d: expected txId %d, got %d", i, expected[i].txId, node.CurTxId())
		}
		if node.Index() != expected[i].index {
			t.Errorf("Node %d: expected index %d, got %d", i, expected[i].index, node.Index())
		}
		i--
	}
	if i != -1 {
		t.Errorf("Expected %d nodes, traversed %d in backward direction", len(expected), len(expected)+i+1)
	}

	i = len(expected) - 1
	for node := list.Tail; node != nil; node = node.Prev() {
		if i < 0 {
			t.Errorf("More nodes than expected in reverse traversal")
			break
		}
		if node.CurBlock() != expected[i].block {
			t.Errorf("Reverse Node %d: expected block %d, got %d", i, expected[i].block, node.CurBlock())
		}
		if node.CurTxId() != expected[i].txId {
			t.Errorf("Reverse Node %d: expected txId %d, got %d", i, expected[i].txId, node.CurTxId())
		}
		if node.Index() != expected[i].index {
			t.Errorf("Reverse Node %d: expected index %d, got %d", i, expected[i].index, node.Index())
		}
		i--
	}
	if i != -1 {
		t.Errorf("Expected %d nodes, traversed %d in reverse direction", len(expected), len(expected)+i+1)
	}

	zeroList, _ := NewAppList([]Appearance{{BlockNumber: 0, TransactionIndex: 1}})
	i = 0
	for node := zeroList.Head; node != nil; node = node.Next() {
		if i == 0 && node.Prev() == nil {
			t.Errorf("Zero block head should have prev, got nil")
		}
		i++
	}

	nonZeroList, _ := NewAppList([]Appearance{{BlockNumber: 100, TransactionIndex: 1}})
	i = 0
	for node := nonZeroList.Head; node != nil; node = node.Next() {
		if i == 0 && node.Prev() == nil {
			t.Errorf("Non-zero block head should have prev, got nil")
		} else if i == 0 && (node.Prev().CurBlock() != base.Blknum(99) || node.Prev().CurTxId() != 0) {
			t.Errorf("Non-zero block prev should be {99, 0}, got %d:%d", node.Prev().CurBlock(), node.Prev().CurTxId())
		}
		i++
	}
}
