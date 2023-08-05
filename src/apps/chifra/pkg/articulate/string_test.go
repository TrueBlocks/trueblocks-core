package articulate

import (
	"testing"
)

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
