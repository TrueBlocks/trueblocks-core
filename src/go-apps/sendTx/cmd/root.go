package cmd

import (
	"context"
	"fmt"
	"io/ioutil"
	"math/big"

	"github.com/ethereum/go-ethereum/accounts"
	"github.com/ethereum/go-ethereum/accounts/keystore"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/crypto"
	"github.com/ethereum/go-ethereum/ethclient"
)

const (
	KEYJSON_FILEDIR   = `/home/someuser/.privatechain/keystore/UTC--2017-03-22T21-46-08.966825919Z--547d093502fe2760b0ca88edac5bcfa42e8d9e51`
	SIGN_PASSPHRASE   = `test`
	KEYSTORE_DIR      = `/home/someuser/.privatechain/keystore`
	COINBASE_ADDR_HEX = `0x547d093502fe2760b0ca88edac5bcfa42e8d9e51`
	ALTER_ADDR_HEX    = `0x970e8128ab834e8eac17ab8e3812f010678cf791`
	CHAIN_ID          = 0 // From geth logs
)

func thing() {
	// Init a keystore
	ks := keystore.NewKeyStore(
		KEYSTORE_DIR,
		keystore.LightScryptN,
		keystore.LightScryptP)
	fmt.Println()

	// Create account definitions
	fromAccDef := accounts.Account{
		Address: common.HexToAddress(COINBASE_ADDR_HEX),
	}

	toAccDef := accounts.Account{
		Address: common.HexToAddress(ALTER_ADDR_HEX),
	}

	// Find the signing account
	signAcc, err := ks.Find(fromAccDef)
	if err != nil {
		fmt.Println("account keystore find error:")
		panic(err)
	}
	fmt.Printf("account found: signAcc.addr=%s; signAcc.url=%s\n", signAcc.Address.String(), signAcc.URL)
	fmt.Println()

	// Unlock the signing account
	errUnlock := ks.Unlock(signAcc, SIGN_PASSPHRASE)
	if errUnlock != nil {
		fmt.Println("account unlock error:")
		panic(err)
	}
	fmt.Printf("account unlocked: signAcc.addr=%s; signAcc.url=%s\n", signAcc.Address.String(), signAcc.URL)
	fmt.Println()

	// Construct the transaction
	tx := types.NewTransaction(
		0x0,
		toAccDef.Address,
		new(big.Int),
		0,
		new(big.Int),
		[]byte(`cooldatahere`))

	// Open the account key file
	keyJSON, readErr := ioutil.ReadFile(KEYJSON_FILEDIR)
	if readErr != nil {
		fmt.Println("key json read error:")
		panic(readErr)
	}

	// Get the private key
	keyWrapper, keyErr := keystore.DecryptKey(keyJSON, SIGN_PASSPHRASE)
	if keyErr != nil {
		fmt.Println("key decrypt error:")
		panic(keyErr)
	}
	fmt.Printf("key extracted: addr=%s", keyWrapper.Address.String())

	// Define signer and chain id
	// chainID := big.NewInt(CHAIN_ID)
	// signer := types.NewEIP155Signer(chainID)
	signer := types.HomesteadSigner{}

	// Sign the transaction signature with the private key
	signature, signatureErr := crypto.Sign(tx.SigHash(signer).Bytes(), keyWrapper.PrivateKey)
	if signatureErr != nil {
		fmt.Println("signature create error:")
		panic(signatureErr)
	}

	signedTx, signErr := tx.WithSignature(signer, signature)
	if signErr != nil {
		fmt.Println("signer with signature error:")
		panic(signErr)
	}

	// Connect the client
	client, err := ethclient.Dial("http://localhost:8000") // 8000=geth RPC port
	if err != nil {
		fmt.Println("client connection error:")
		panic(err)
	}
	fmt.Println("client connected")
	fmt.Println()

	// Send the transaction to the network
	txErr := client.SendTransaction(context.Background(), signedTx)
	if txErr != nil {
		fmt.Println("send tx error:")
		panic(txErr)
	}

	fmt.Printf("send success tx.hash=%s\n", signedTx.Hash().String())
}

// Execute What it is
func Execute() {
	fmt.Println("Hello world")
	thing()
}
