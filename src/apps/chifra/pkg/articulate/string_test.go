package articulate

import (
	"testing"

	"github.com/ethereum/go-ethereum/common"
)

func Test_toPrintable(t *testing.T) {
	type args struct {
		hex string
	}
	tests := []struct {
		name        string
		args        args
		wantResult  string
		wantSuccess bool
	}{
		{
			name: "CID-like",
			args: args{
				hex: "0x516d51344e3338384c736e77384e486e4a72526f59327561676d5071736d3353",
			},
			wantResult:  "QmQ4N388Lsnw8NHnJrRoY2uagmPqsm3S",
			wantSuccess: true,
		},
		{
			name: "not a string",
			args: args{
				hex: "0xf31b5730bd294639c67fa143fca1dad2c7aa05644f8d8b0e104aad5922d2f866",
			},
			wantResult:  "",
			wantSuccess: false,
		},
		{
			name: "string with newline",
			args: args{
				hex: "0x" + common.Bytes2Hex([]byte("Line1\nLine2")),
			},
			wantResult:  "Line1[n]Line2",
			wantSuccess: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotResult, gotPureStr := ArticulateString(tt.args.hex)
			if gotResult != tt.wantResult {
				t.Errorf("ArticulateString() gotResult = %v, want %v", gotResult, tt.wantResult)
			}
			if gotPureStr != tt.wantSuccess {
				t.Errorf("ArticulateString() gotSuccess = %v, want %v", gotPureStr, tt.wantSuccess)
			}
		})
	}
}
