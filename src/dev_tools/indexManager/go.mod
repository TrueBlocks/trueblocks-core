module github.com/TrueBlocks/trueblocks-core/indexManager

// Go Version
go 1.23.1

replace github.com/TrueBlocks/trueblocks-core/sdk => ../../../sdk

require github.com/TrueBlocks/trueblocks-core/src/apps/chifra v0.0.0-20250131141006-ca15858b0e7c

require (
	github.com/ethereum/go-ethereum v1.14.13 // indirect
	github.com/holiman/uint256 v1.3.1 // indirect
	github.com/inconshreveable/mousetrap v1.1.0 // indirect
	github.com/spf13/cobra v1.7.0 // indirect
	github.com/spf13/pflag v1.0.5 // indirect
	golang.org/x/sys v0.28.0 // indirect
	golang.org/x/term v0.27.0 // indirect
)
