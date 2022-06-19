package unchained

import (
	"context"
	"fmt"
	"log"
	"math/big"
	"testing"
	"time"

	"errors"

	"github.com/ethereum/go-ethereum/accounts/abi/bind"
	"github.com/ethereum/go-ethereum/accounts/abi/bind/backends"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/core"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/crypto"
)

// simulateClient setups a simulated blockchain and deploys the contract
func simulateClient() (*backends.SimulatedBackend, *bind.TransactOpts, *Unchained) {
	privateKey, err := crypto.GenerateKey()
	if err != nil {
		log.Fatal(err)
	}

	auth, _ := bind.NewKeyedTransactorWithChainID(privateKey, big.NewInt(1337))
	balance := new(big.Int)
	balance.SetString("10000000000000000000", 10) // 10 eth in wei

	address := auth.From
	genesisAlloc := map[common.Address]core.GenesisAccount{
		address: {
			Balance: balance,
		},
	}
	blockGasLimit := uint64(4712388)
	client := backends.NewSimulatedBackend(genesisAlloc, blockGasLimit)

	nonce, err := client.PendingNonceAt(context.Background(), address)
	if err != nil {
		log.Fatal(err)
	}

	gasLimit := uint64(4712388) // in units
	gasPrice, err := client.SuggestGasPrice(context.Background())
	if err != nil {
		log.Fatal(err)
	}

	auth.Nonce = big.NewInt(int64(nonce))
	auth.Value = big.NewInt(0) // value
	auth.GasLimit = gasLimit
	auth.GasPrice = gasPrice

	_, tx, instance, err := DeployUnchained(auth, client)
	if err != nil {
		log.Fatal(err)
	}
	mined := make(chan error)
	go func() {
		_, err = bind.WaitDeployed(context.Background(), client, tx)
		mined <- err
		close(mined)
	}()
	client.Commit()

	select {
	case err = <-mined:
		if err != nil {
			log.Fatal("transaction error:", err)
		}
	case <-time.After(2 * time.Second):
		log.Fatal("timeout")
	}

	return client, auth, instance
}

// waitForTransaction will call `doTransaction` which performs the transaction, commit it on the client and wait until it is mined
func waitForTransaction(client *backends.SimulatedBackend, auth *bind.TransactOpts, doTransaction func() (*types.Transaction, error)) (*types.Transaction, error) {
	nonce, err := client.PendingNonceAt(context.Background(), auth.From)
	if err != nil {
		log.Fatal(err)
	}

	gasPrice, err := client.SuggestGasPrice(context.Background())
	if err != nil {
		log.Fatal(err)
	}

	auth.Nonce = big.NewInt(int64(nonce))
	auth.GasLimit = uint64(300000) // in units
	auth.GasPrice = gasPrice

	publishHashTx, err := doTransaction()
	if err != nil {
		return publishHashTx, err
	}

	mined2 := make(chan error)
	go func() {
		_, err = bind.WaitMined(context.Background(), client, publishHashTx)
		mined2 <- err
		close(mined2)
	}()
	client.Commit()

	select {
	case err = <-mined2:
		if err != nil {
			return publishHashTx, fmt.Errorf("transaction error: %w", err)
		}
	case <-time.After(2 * time.Second):
		return publishHashTx, errors.New("timeout")
	}

	return publishHashTx, nil
}

func TestNewUnchained(t *testing.T) {
	client, _, instance := simulateClient()
	defer client.Close()

	defaultValue, err := instance.ChainNameToHash(nil, "mainnet")
	if err != nil {
		t.Error(err)
	}
	if defaultValue != "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm" {
		t.Error("Wrong default value", defaultValue)
	}
}

func TestPublishHash(t *testing.T) {
	client, auth, instance := simulateClient()
	_, err := waitForTransaction(
		client,
		auth,
		func() (*types.Transaction, error) {
			return instance.PublishHash(auth, "gnosis", "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm")
		},
	)
	if err != nil {
		t.Error(err)
	}

	hashValue, err := instance.ChainNameToHash(nil, "gnosis")
	if err != nil {
		t.Error(err)
	}
	if hashValue != "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm" {
		t.Error("Wrong default value", hashValue)
	}
}

func TestDonate(t *testing.T) {
	client, auth, instance := simulateClient()
	_, err := waitForTransaction(
		client,
		auth,
		func() (*types.Transaction, error) {
			return instance.Donate(auth)
		},
	)
	if err != nil {
		t.Error(err)
	}
}
