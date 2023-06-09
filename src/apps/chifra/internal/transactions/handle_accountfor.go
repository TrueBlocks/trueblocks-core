package transactionsPkg

func (opts *TransactionsOptions) HandleAccountFor() (err error) {
	if opts.Globals.IsApiMode() {
		return nil
	}
	return opts.Globals.PassItOn("getTrans", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
}
