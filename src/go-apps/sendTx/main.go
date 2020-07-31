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

	contract, err := NewUnchainedIndex(common.HexToAddress("0x50c8f8c71828e594a513cc540176822a57e597c2"), conn)
	if err != nil {
		log.Fatalf("Could not connect to contract: %v", err)
	}

	value, _ := contract.IndexHash(nil)
	fmt.Println(value)
}
