package chunksPkg

func (opts *ChunksOptions) HandlePinManifest(blockNums []uint64) error {

	// defer opts.Globals.RenderFooter()
	// err := opts.Globals.RenderHeader(types.SimpleManifest{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	// if err != nil {
	// 	return err
	// }

	// count := int(0)
	// ctx := WalkContext{
	// 	VisitFunc: opts.pinFile,
	// 	Data:      &count,
	// }

	// // if err := output.OutputHeader(types.SimpleManifest{}, opts.Globals.Writer, opts.Globals.Format, "chunks"); err != nil {
	// // 	return err
	// // }
	// if err := opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums); err != nil {
	// 	return err
	// }
	// // opts.Globals.RenderFooter()

	return nil
}
