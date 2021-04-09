package main

import (
	"fmt"
	"log"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/ethclient"
)

func main() {
	conn, err := ethclient.Dial("http://localhost:36963")
	if err != nil {
		log.Fatalf("Failed to connect: %v", err)
	}

	contract, err := NewUnchainedIndex(common.HexToAddress("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"), conn)
	if err != nil {
		log.Fatalf("Could not connect to contract: %v", err)
	}

    // contract.PublishHash(nil, "0x12")
	value1, _ := contract.Owner(nil)
	fmt.Printf("Owner: 0x%x\n", value1)

	value, _ := contract.ManifestHash(nil)
	fmt.Printf("Latest Hash: %s\n", value)
}
