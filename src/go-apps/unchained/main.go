package main

import (
	"fmt"
	"log"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/ethclient"
)

func main() {
	conn, err := ethclient.Dial("https://mainnet.infura.io/v3/bd294519b16b4967b4d647071088f473")
	if err != nil {
		log.Fatalf("Failed to connect: %v", err)
	}

	contract, err := NewUnchainedIndex(common.HexToAddress("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"), conn)
	if err != nil {
		log.Fatalf("Could not connect to contract: %v", err)
	}

	value, _ := contract.ManifestHash(nil)
	fmt.Println(value)
}
