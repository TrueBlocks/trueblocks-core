package main

import (
	"context"
	"fmt"
	"log"
	"math/big"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/keystore"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/ethclient"
	"github.com/ethereum/go-ethereum/rpc"
	"github.com/joho/godotenv"
)

func main() {
	pwd, _ := os.Getwd()
	logger.InfoBB("pwd:", pwd)

	err := godotenv.Load(".env")
	if err != nil {
		log.Fatalf("Error loading .env file: %v", err)
	}

	password := os.Getenv("TB_KEYSTORE_PW")
	rpcURL := os.Getenv("TB_KEYSTORE_RPC")

	logger.InfoBB("password:", password)
	logger.InfoBB("rpcURL:", rpcURL)

	client, err := ethclient.Dial(rpcURL)
	if err != nil {
		log.Fatal(err)
	}

	rpcClient, err := rpc.Dial(rpcURL)
	if err != nil {
		log.Fatal(err)
	}

	files, err := os.ReadDir("./keystore")
	if err != nil || len(files) == 0 {
		log.Fatal("Keystore not found or empty")
	}

	filePath := "./keystore/" + files[0].Name()
	keystoreJSON, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	key, err := keystore.DecryptKey(keystoreJSON, password)
	if err != nil {
		log.Fatal(err)
	}

	fromAddress := key.Address
	toAddress := common.HexToAddress("0x00a3819199113fc6a6e6ba1298afde7377e2009b")
	nonce, err := client.PendingNonceAt(context.Background(), fromAddress)
	if err != nil {
		log.Fatal(err)
	}

	baseFee, err := client.SuggestGasPrice(context.Background())
	if err != nil {
		log.Fatal(err)
	}

	var chainIDHex string
	err = rpcClient.CallContext(context.Background(), &chainIDHex, "eth_chainId")
	if err != nil {
		log.Fatalf("Failed to fetch chain ID: %v", err)
	}

	chainID := new(big.Int)
	_, ok := chainID.SetString(chainIDHex[2:], 16) // Remove "0x" and parse as hex
	if !ok {
		log.Fatal("Failed to parse chainID as big.Int")
	}

	var gasTipCapHex string
	err = rpcClient.CallContext(context.Background(), &gasTipCapHex, "eth_maxPriorityFeePerGas")
	if err != nil {
		log.Fatal(err)
	}

	gasTipCap := new(big.Int)
	_, ok = gasTipCap.SetString(gasTipCapHex[2:], 16) // Remove "0x" and parse as hex
	if !ok {
		log.Fatal("Failed to parse gas tip cap as big.Int")
	}

	value := big.NewInt(1000)

	gasFeeCap := new(big.Int).Add(baseFee, gasTipCap)

	gasLimit, err := client.EstimateGas(context.Background(), ethereum.CallMsg{
		From:  fromAddress,
		To:    &toAddress,
		Value: value,
		Data:  nil,
	})
	if err != nil {
		log.Fatal("Failed to estimate gas limit: ", err)
	}

	tx := types.NewTx(&types.DynamicFeeTx{
		ChainID:   chainID,
		Nonce:     nonce,
		GasFeeCap: gasFeeCap,
		GasTipCap: gasTipCap,
		Gas:       gasLimit,
		To:        &toAddress,
		Value:     value,
		Data:      nil,
	})

	logger.InfoBG("chainID:   ", chainID)
	logger.InfoBG("nonce:     ", nonce)
	logger.InfoBG("from:      ", fromAddress)
	logger.InfoBG("to:        ", toAddress)
	logger.InfoBW("baseFee:   ", baseFee)
	logger.InfoBW("gasTipCap: ", gasTipCap)
	logger.InfoBW("gasFeeCap: ", gasFeeCap)
	logger.InfoBW("gasLimit:  ", gasLimit)
	logger.InfoBW("gasLimit:  ", tx.Hash().Hex())

	signedTx, err := types.SignTx(tx, types.LatestSignerForChainID(chainID), key.PrivateKey)
	if err != nil {
		log.Fatal(err)
	}

	err = client.SendTransaction(context.Background(), signedTx)
	if err != nil {
		log.Fatal(err)
	}

	now := time.Now()
	msg := fmt.Sprintf("%s,%s,%s,%s\n", now.Format("2024-02-01|15:04:05.000"), fromAddress.Hex(), toAddress.Hex(), signedTx.Hash().Hex())
	fmt.Print(msg)
	file.AppendToAsciiFile("transactions.csv", msg)
}

/*

package main

import (
	"bufio"
	"context"
	"errors"
	"flag"
	"fmt"
	"log"
	"math/big"
	"os"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/keystore"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/crypto"
	"github.com/ethereum/go-ethereum/ethclient"
)

var (
	createOption bool
	showOption   bool
	sendOption   bool
	sendData     string
	toAddressStr string
	keystoreDir  = "./keystore"
)

func getOptions() {
	if _, err := os.Stat(".env"); err == nil {
		file, err := os.Open(".env")
		if err != nil {
			log.Fatalf("Failed to open .env file: %v", err)
		}
		defer file.Close()

		scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			line := strings.TrimSpace(scanner.Text())
			if line == "" || strings.HasPrefix(line, "#") {
				continue
			}
			parts := strings.SplitN(line, "=", 2)
			if len(parts) == 2 {
				os.Setenv(strings.TrimSpace(parts[0]), strings.TrimSpace(parts[1]))
			}
		}
		if err := scanner.Err(); err != nil {
			log.Fatalf("Error reading .env file: %v", err)
		}
	} else {
		log.Println(".env file not found. This won't work.")
	}

	flag.BoolVar(&createOption, "create", false, "Create a new keystore")
	flag.BoolVar(&showOption, "show", false, "Show keystore contents")
	flag.BoolVar(&sendOption, "send", false, "Send a transaction with optional data")
	flag.StringVar(&sendData, "data", "", "Optional data to include in the transaction")
	flag.StringVar(&toAddressStr, "to", "", "Recipient address for the transaction (required with --send)")
	flag.Parse()

	if !createOption && !showOption && !sendOption {
		fmt.Println("Usage: program --create, --show, or --send [--data=<text>] [--to=<address>]")
		os.Exit(1)
	}

	if sendOption {
		if toAddressStr == "" {
			fmt.Println("Error: --to is required with --send")
			os.Exit(1)
		}
		if os.Getenv("TB_KEYSTORE_PW") == "" {
			fmt.Println("Error: TB_KEYSTORE_PW environment variable is not set")
			os.Exit(1)
		}
		if os.Getenv("TB_KEYSTORE_RPC") == "" {
			fmt.Println("Error: TB_KEYSTORE_RPC environment variable is not set")
			os.Exit(1)
		}
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
	var index int = 0
	// fmt.Scan(&index)

	if index < 0 || index >= len(files) {
		log.Fatal("Invalid index")
	}

	filePath := fmt.Sprintf("%s/%s", keystoreDir, files[index].Name())
	// password := promptPassword("Enter the password to unlock the keystore: ")
	password := os.Getenv("TB_KEYSTORE_PW") // promptPassword("Enter the password to unlock the keystore: ")

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
	fmt.Printf("Address: %s\nPrivate Key: %s\n", key.Address.Hex(), hash.Hex())
}

func getBalanceAt(address common.Address) *big.Int {
	// Fake balance for demonstration purposes
	return big.NewInt(1000000000000000000) // 1 Ether in Wei
}

func sendTransaction() {
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
	var index int = 0
	// fmt.Scan(&index)

	if index < 0 || index >= len(files) {
		log.Fatal("Invalid index")
	}

	filePath := fmt.Sprintf("%s/%s", keystoreDir, files[index].Name())
	password := os.Getenv("TB_KEYSTORE_PW") // promptPassword("Enter the password to unlock the keystore: ")

	keystoreJSON, err := os.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	key, err := keystore.DecryptKey(keystoreJSON, password)
	if err != nil {
		log.Fatal("Failed to unlock the keystore: ", err)
	}

	client, err := ethclient.Dial(os.Getenv("TB_KEYSTORE_RPC"))
	if err != nil {
		log.Fatal("Failed to connect to Ethereum node: ", err)
	}

	fromAddress := key.Address
	toAddress := common.HexToAddress(toAddressStr)

	nonce, err := client.PendingNonceAt(context.Background(), fromAddress)
	if err != nil {
		log.Fatal("Failed to get nonce: ", err)
	}
	// nonce = 0

	gasPrice, err := client.SuggestGasPrice(context.Background())
	if err != nil {
		log.Fatal("Failed to get gas price: ", err)
	}
	logger.InfoBY("gasPrice:", gasPrice)
	gasPrice = new(big.Int).Mul(new(big.Int).SetUint64(4), gasPrice)
	logger.InfoBY("gasPrice:", gasPrice)
	// gasPrice.SetString("308a0", 16)
	// logger.InfoBY("gasPrice:", gasPrice)

	// 0xc6df26c678
	gasLimit := uint64(4300000) // uint64(84000) // 21000) // Default gas limit for a basic transaction
	requiredGas := new(big.Int).Mul(new(big.Int).SetUint64(gasLimit), gasPrice)
	logger.InfoBY("requiredGas:", requiredGas)

	value := big.NewInt(0)
	// value.SetString("3e8", 16)

	var data []byte
	if sendData != "" {
		data = []byte(sendData)
	}

	// Create a call message to estimate gas
	msg := ethereum.CallMsg{
		From:     fromAddress,
		To:       &toAddress,
		GasPrice: gasPrice,
		Value:    value,
		Data:     data,
	}

	// Estimate gas usage
	gasLimit, err = client.EstimateGas(context.Background(), msg)
	if err != nil {
		log.Fatalf("Failed to estimate gas: %v", err)
	}
	gasLimit *= 4 // = new(big.Int).Mul(new(big.Int).SetUint64(2), gasLimit)
	fmt.Printf("Estimated Gas Limit: %d\n", gasLimit)

	tx := types.NewTransaction(nonce, toAddress, value, gasLimit, gasPrice, data)

	chainID, err := client.NetworkID(context.Background())
	if err != nil {
		log.Fatal("Failed to get network ID: ", err)
	}

	signedTx, err := types.SignTx(tx, types.NewEIP155Signer(chainID), key.PrivateKey)
	if err != nil {
		log.Fatal("Failed to sign transaction: ", err)
	}

	0.00071222
	   SepoliaETH
	   🦍 Aggressive

	   ~15 sec

	   ⚠ Max fee:

	   0.00674423
	   SepoliaETH

	   logger.InfoBY("keystoreDir:", keystoreDir)
	logger.InfoBY("files:", files)
	logger.InfoBY("filePath:", filePath)
	logger.InfoBY("password:", strings.Repeat("*", len(password)))
	// logger.InfoBY("keystoreJSON:", string(keystoreJSON))
	logger.InfoBY("key.Id", key.Id)
	logger.InfoBY("key.Address", key.Address)
	pk := crypto.FromECDSA(key.PrivateKey)
	hash := base.Hash{}
	hash.SetBytes(pk)
	pks := hash.Hex()
	pks = pks[:10] + "..." + pks[len(pks)-10:]
	fmt.Printf("Address: %s\nPrivate Key: %s\n", key.Address.Hex(), pks)
	bn, _ := client.BlockNumber(context.Background())
	logger.InfoBY("client.BlockNumber:", bn)
	p, _ := client.PeerCount(context.Background())
	logger.InfoBY("client.PeerCount:", p)
	s, _ := client.SyncProgress(context.Background())
	balance, _ := client.BalanceAt(context.Background(), fromAddress, base.BiFromBn(base.Value(bn)))
	okay := true
	if balance.Cmp(requiredGas) < 0 {
		logger.InfoBR("Insufficient balance to cover gas fees:")
		logger.InfoBR("\t", balance)
		logger.InfoBR("\t", requiredGas)
		okay = false
	}

	logger.InfoBY("client.SyncProgress:", s)
	logger.InfoBY("fromAddress:", fromAddress)
	logger.InfoBY("toAddress:", toAddress)
	logger.InfoBY("balance:", balance)
	logger.InfoBY("requiredGas:", requiredGas)
	logger.InfoBY("balance.Cmp(requiredGas):", balance.Cmp(requiredGas) > 0)
	logger.InfoBY("value:", value)
	logger.InfoBY("data:", data)
	logger.InfoBY("tx:", tx.Hash().Hex())
	logger.InfoBY("chainID:", chainID)
	logger.InfoBY("signedTx:", signedTx.Hash().Hex())
	cnt, _ := client.PendingTransactionCount(context.Background())
	logger.InfoBY("PendingTransactionCount:", cnt)
	logger.InfoBY("nonce:", nonce)
	logger.InfoBY("gasPrice:    ", gasPrice)
	logger.InfoBY("balance:     ", balance)
	logger.InfoBY("gasLimit:    ", gasLimit)
	logger.InfoBY("requiredGas: ", requiredGas)
	if okay {
		// usage.QueryUser("Press Enter to continue", "Continuing...")

		err = client.SendTransaction(context.Background(), signedTx)
		if err != nil {
			log.Fatal("Failed to send transaction: ", err)
		}

		receipt, err := WaitForTransactionReceipt(client, signedTx.Hash())
		if err != nil {
			log.Fatal("Failed to get transaction receipt: ", err)
		}
		logger.InfoBB("Transaction receipt:", receipt)
	}
}

// WaitForTransactionReceipt polls for the transaction receipt until it is available or a timeout occurs.
func WaitForTransactionReceipt(client *ethclient.Client, txHash common.Hash) (*types.Receipt, error) {
	timeout := time.Minute * 5 // Define a timeout for the polling
	start := time.Now()

	for {
		// Check if the timeout has been exceeded
		if time.Since(start) > timeout {
			logger.InfoBG("Timed out...")
			return nil, errors.New("transaction receipt not found within the timeout period")
		}

		// Attempt to fetch the transaction receipt
		receipt, err := client.TransactionReceipt(context.Background(), txHash)
		if err == nil {
			// Receipt found, return it
			logger.InfoBG("Got it...")
			return receipt, nil
		}

		_, isPending, err2 := client.TransactionByHash(context.Background(), txHash)
		if err2 != ethereum.NotFound {
			log.Fatalf("Error fetching transaction: %v", err)
		}
		if isPending {
			logger.InfoBR("Transaction is pending...")
		} else {
			logger.InfoBR("Transaction not pending...")
		}

		// If the receipt is not found, continue polling
		// The error is expected until the transaction is mined
		if err == ethereum.NotFound {
			// Wait before retrying
			logger.InfoBG("Transaction receipt not found, retrying...", receipt)
			time.Sleep(time.Second * 2)
			continue
		}

		// If an unexpected error occurs, return it
		logger.InfoBG("Unexpected error...")
		return nil, err
	}
}

func main() {
	getOptions()

	if createOption {
		createKeystore()
	} else if showOption {
		showKeystore()
	} else if sendOption {
		sendTransaction()
	}
}

*/
