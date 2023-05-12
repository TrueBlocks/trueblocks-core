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

func TestArticulateEncodedString(t *testing.T) {
	type args struct {
		hex string
	}
	tests := []struct {
		name       string
		args       args
		wantResult string
		wantErr    bool
	}{
		{
			name: "valid string",
			args: args{
				hex: "0x0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000e44616920537461626c65636f696e000000000000000000000000000000000000",
			},
			wantResult: "Dai Stablecoin",
		},
		{
			name: "invalid string",
			args: args{
				// this is bytes32
				hex: "0x506f6f6c65642045746865720000000000000000000000000000000000000000",
			},
			wantErr: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotResult, err := ArticulateEncodedString(tt.args.hex)
			if (err != nil) != tt.wantErr {
				t.Errorf("ArticulateEncodedString() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotResult != tt.wantResult {
				t.Errorf("ArticulateEncodedString() = %v, want %v", gotResult, tt.wantResult)
			}
		})
	}
}

func TestArticulateBytes32String(t *testing.T) {
	type args struct {
		hex string
	}
	tests := []struct {
		name string
		args args
		want string
	}{
		{
			name: "Pooled Ether",
			args: args{
				hex: "0x506f6f6c65642045746865720000000000000000000000000000000000000000",
			},
			want: "Pooled Ether",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := ArticulateBytes32String(tt.args.hex); got != tt.want {
				t.Errorf("ArticulateBytes32String() = %v, want %v", got, tt.want)
			}
		})
	}
}

func TestArticulateEncodedStringOrBytes32(t *testing.T) {
	type args struct {
		hex string
	}
	tests := []struct {
		name    string
		args    args
		want    string
		wantErr bool
	}{
		{
			name: "bytes32",
			args: args{
				hex: "0x506f6f6c65642045746865720000000000000000000000000000000000000000",
			},
			want: "Pooled Ether",
		},
		{
			name: "string",
			args: args{
				hex: "0x0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000e44616920537461626c65636f696e000000000000000000000000000000000000",
			},
			want: "Dai Stablecoin",
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := ArticulateEncodedStringOrBytes32(tt.args.hex)
			if (err != nil) != tt.wantErr {
				t.Errorf("ArticulateEncodedStringOrBytes32() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if got != tt.want {
				t.Errorf("ArticulateEncodedStringOrBytes32() = %v, want %v", got, tt.want)
			}
		})
	}
}
