package types

import (
	"bytes"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestBlockEncode(t *testing.T) {
	expected := &SimpleBlock[SimpleTransaction]{
		BlockNumber: 57831,
		Hash:        base.HexToHash("0x65a1119526538c12891a644ff12362710cb27cacbc95cf03001d5dd2d268140c"),
		Transactions: []SimpleTransaction{
			{
				Hash: base.HexToHash("0x682767553fcdc49de838b63cb2730a90a0c92b36040828b751e646b7cde4da27"),
			},
			{
				Hash: base.HexToHash("0xfc7e6a17103a9aac9bde235a27ba9a5f4fdea1d027e2764ed407a38cbccc48a6"),
			},
			{
				Hash: base.HexToHash("0x2787860f83df0f7938c4987656d7c6f58f936a8a97860f7057dce2e52ccb911b"),
			},
		},
	}
	buffer := new(bytes.Buffer)
	_, err := expected.WriteTo(buffer)
	if err != nil {
		t.Fatal("writing:", err)
	}

	result := &SimpleBlock[SimpleTransaction]{}
	_, err = result.ReadFrom(buffer)
	if err != nil {
		t.Fatal("reading:", err)
	}

	if !reflect.DeepEqual(result, expected) {
		t.Errorf("SimpleBlock.Encode() = %+v, want %+v", result, expected)
	}
}
