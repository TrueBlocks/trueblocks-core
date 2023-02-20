package namesPkg

import "fmt"

func (opts *NamesOptions) HandleCrud() error {
	fmt.Println(opts)
	return nil
	// namesArray, err := names.LoadNamesArray(opts.Globals.Chain, opts.getType(), names.SortByAddress, opts.Terms)
	// if err != nil {
	// 	return err
	// }
	// if len(namesArray) == 0 {
	// 	logger.Log(logger.Warning, "No results for ", os.Args)
	// 	return nil
	// }

	// ctx := context.Background()

	// // Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	// fetchData := func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
	// 	for _, name := range namesArray {
	// 		name := name
	// 		modelChan <- &name
	// 	}
	// }

	// return output.StreamMany(ctx, fetchData, output.OutputOptions{
	// 	Writer:     opts.Globals.Writer,
	// 	Chain:      opts.Globals.Chain,
	// 	TestMode:   opts.Globals.TestMode,
	// 	NoHeader:   opts.Globals.NoHeader,
	// 	ShowRaw:    opts.Globals.ShowRaw,
	// 	Verbose:    opts.Globals.Verbose,
	// 	LogLevel:   opts.Globals.LogLevel,
	// 	Format:     opts.Globals.Format,
	// 	OutputFn:   opts.Globals.OutputFn,
	// 	Append:     opts.Globals.Append,
	// 	JsonIndent: "  ",
	// 	Extra: map[string]interface{}{
	// 		"expand":  opts.Expand,
	// 		"prefund": opts.Prefund,
	// 	},
	// })
}
