package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/ethereum/go-ethereum/accounts/keystore"
	"github.com/ethereum/go-ethereum/crypto"
)

var (
	createOption bool
	showOption   bool
	keystoreDir  = "./keystore"
)

func getOptions() {
	flag.BoolVar(&createOption, "create", false, "Create a new keystore")
	flag.BoolVar(&showOption, "show", false, "Show keystore contents")
	flag.Parse()

	if !createOption && !showOption {
		fmt.Println("Usage: program --create or --show")
		os.Exit(1)
	}
}

func promptPassword(prompt string) string {
	fmt.Print(prompt)
	reader := bufio.NewReader(os.Stdin)
	password, _ := reader.ReadString('\n')
	return strings.TrimSpace(password)
}

func createKeystore() {
	privateKey, err := crypto.GenerateKey()
	if err != nil {
		log.Fatal(err)
	}

	if _, err := os.Stat(keystoreDir); os.IsNotExist(err) {
		if err := os.Mkdir(keystoreDir, 0700); err != nil {
			log.Fatal(err)
		}
	}

	password := promptPassword("Enter a password for the keystore: ")
	ks := keystore.NewKeyStore(keystoreDir, keystore.StandardScryptN, keystore.StandardScryptP)
	account, err := ks.ImportECDSA(privateKey, password)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("Keystore created for address: %s\n", account.Address.Hex())
}

func showKeystore() {
	files, err := os.ReadDir(keystoreDir)
	if err != nil {
		log.Fatal(err)
	}

	if len(files) == 0 {
		fmt.Println("No keystore files found.")
		return
	}

	fmt.Println("Available keystore files:")
	for i, file := range files {
		fmt.Printf("[%d] %s\n", i, file.Name())
	}

	fmt.Print("Choose a keystore file index: ")
	var index int
	fmt.Scan(&index)

	if index < 0 || index >= len(files) {
		log.Fatal("Invalid index")
	}

	filePath := fmt.Sprintf("%s/%s", keystoreDir, files[index].Name())
	password := promptPassword("Enter the password to unlock the keystore: ")

	keystoreJSON, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	key, err := keystore.DecryptKey(keystoreJSON, password)
	if err != nil {
		log.Fatal("Failed to unlock the keystore: ", err)
	}

	pk := crypto.FromECDSA(key.PrivateKey)
	hash := base.Hash{}
	hash.SetBytes(pk)
	pks := hash.Hex()
	pks = pks[:10] + "..." + pks[len(pks)-10:]
	fmt.Printf("Address: %s\nPrivate Key: %s\n", key.Address.Hex(), pks)
}

func main() {
	getOptions()

	if createOption {
		createKeystore()
	} else if showOption {
		showKeystore()
	}
}
