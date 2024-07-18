package main

import (
	"fmt"
	"reflect"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func TestStreamAbis() {
	opts := sdk.AbisOptions{
		Addrs: []string{
			"0x6b175474e89094c44da98b954eedeac495271d0f", // MakerDAO
			"0x5d3a536E4D6DbD6114cc1Ead35777bAB948E3643", // Compound
			"0x514910771af9ca656af840dff83e8264ecf986ca", // Chainlink
			"0x6b3595068778dd592e39a122f4f5a5cf09c90fe2", // SushiSwap
			"0xC011A72400E58ecD99Ee497CF89E3775d4bd732F", // Synthetix
			"0xba100000625a3754423978a60c9317c58a424e3d", // Balancer
			"0xd533a949740bb3306d119cc777fa900ba034cd52", // Curve Finance
		},
		RenderCtx: output.NewStreamingContext(),
	}

	fmt.Println("fnName,nInputs,nOutputs")
	go func() {
		for {
			select {
			case model := <-opts.RenderCtx.ModelChan:
				fmt.Println("Type:", reflect.TypeOf(model))
				if a, ok := model.(*types.Abi); ok {
					fmt.Println("Abi:", a.Address, namesMap[a.Address].Name)
					for _, f := range a.Functions {
						fmt.Printf("\t%s,%d,%d\n", f.Name, len(f.Inputs), len(f.Outputs))
						for _, i := range f.Inputs {
							fmt.Printf("\t\tinput: %s,%s\n", i.Name, i.ParameterType)
						}
						for _, o := range f.Outputs {
							fmt.Printf("\t\toutput: %s,%s\n", o.Name, o.ParameterType)
						}
					}
				} else {
					fmt.Println("Not a function")
				}
			case err := <-opts.RenderCtx.ErrorChan:
				fmt.Println("Error returned by fetchData:", err)
				// os.Exit(1)
			}
		}
	}()

	if _, _, err := opts.Abis(); err != nil {
		fmt.Println(err.Error())
	}
}
