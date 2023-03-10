package transactionsPkg

func (opts *TransactionsOptions) HandleAccountFor() (err error, disp bool) {
	if opts.Globals.IsApiMode() {
		return nil, false
	}
	return opts.Globals.PassItOn("getTrans", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr()), true
}
