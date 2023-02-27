package output

import (
	"context"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func Format(s *types.SimpleName, fmt string) error {
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
		modelChan <- s
	}

	return StreamMany(ctx, fetchData, OutputOptions{
		Writer:   os.Stdout,
		Chain:    "mainnet",
		TestMode: false,
		NoHeader: true,
		ShowRaw:  false,
		Verbose:  false,
		LogLevel: 0,
		Format:   fmt,
		OutputFn: "",
		Append:   false,
		Extra: map[string]interface{}{
			"expand": true,
		},
	})
}
