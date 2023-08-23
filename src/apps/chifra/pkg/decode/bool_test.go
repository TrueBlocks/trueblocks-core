package decode

import "testing"

func TestDecodeBool(t *testing.T) {
	type args struct {
		hexStr string
	}
	tests := []struct {
		name       string
		args       args
		wantResult bool
		wantErr    bool
	}{
		{
			name: "true",
			args: args{
				hexStr: "0x0000000000000000000000000000000000000000000000000000000000000001",
			},
			wantResult: true,
		},
		{
			name: "false",
			args: args{
				hexStr: "0x0000000000000000000000000000000000000000000000000000000000000000",
			},
			wantResult: false,
		},
		{
			name: "too short",
			args: args{
				hexStr: "0x000000000001",
			},
			wantErr: true,
		},
		{
			name: "invalid hex",
			args: args{
				hexStr: "0x000z000000000000000000000000000000000000000000000000000000000001",
			},
			wantErr: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			gotResult, err := ArticulateBool(tt.args.hexStr)
			if (err != nil) != tt.wantErr {
				t.Errorf("ArticulateBool() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if gotResult != tt.wantResult {
				t.Errorf("ArticulateBool() = %v, want %v", gotResult, tt.wantResult)
			}
		})
	}
}
