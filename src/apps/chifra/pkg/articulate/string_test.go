package articulate

import (
	"testing"
)

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
