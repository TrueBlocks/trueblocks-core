package namesPkg

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Test_cleanName(t *testing.T) {
	type args struct {
		name *types.SimpleName
	}
	tests := []struct {
		name         string
		args         args
		wantModified bool
	}{
		{
			name: "Tag lower than 8",
			args: args{
				name: &types.SimpleName{
					Tags: "79999",
				},
			},
			wantModified: false,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotModified := cleanName(tt.args.name); gotModified != tt.wantModified {
				t.Errorf("cleanName() = %v, want %v", gotModified, tt.wantModified)
			}
		})
	}
}
