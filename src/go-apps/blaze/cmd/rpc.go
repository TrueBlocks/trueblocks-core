package cmd

//----------------------------------------------------------------------------
import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
)

// getBlockHeader Returns the block header for a given block.
// func getBlockHeader(blockNum int) ([]byte, error) {
// 	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "parity_getBlockHeaderByNumber", RPCParams{fmt.Sprintf("0x%x", blockNum)}, 2})
// 	if err != nil {
// 		fmt.Println(err)
// 		return nil, err
// 	}
// 
// 	body := bytes.NewReader(payloadBytes)
// 	req, err := http.NewRequest("POST", Options.rpcProvider, body)
// 	if err != nil {
// 		fmt.Println(err)
// 		return nil, err
// 	}
// 
// 	req.Header.Set("Content-Type", "application/json")
// 	resp, err := http.DefaultClient.Do(req)
// 	if err != nil {
// 		fmt.Println(err)
// 		return nil, err
// 	}
// 
// 	blockHeaderBody, err := ioutil.ReadAll(resp.Body)
// 	if err != nil {
// 		fmt.Println(err)
// 		return nil, err
// 	}
// 
// 	defer resp.Body.Close()
// 	return blockHeaderBody, nil
// }

// getTracesFromBlock Returns all traces for a given block.
func getTracesFromBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "trace_block", RPCParams{fmt.Sprintf("0x%x", blockNum)}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", Options.rpcProvider, body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	tracesBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	defer resp.Body.Close()
	return tracesBody, nil
}

// getLogsFromBlock Returns all logs for a given block.
func getLogsFromBlock(blockNum int) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getLogs", RPCParams{LogFilter{fmt.Sprintf("0x%x", blockNum), fmt.Sprintf("0x%x", blockNum)}}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", Options.rpcProvider, body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	logsBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	defer resp.Body.Close()
	return logsBody, nil
}

// getTransactionReceipt Returns recipt for a given transaction -- only used in errored contract creations
func getTransactionReceipt(hash string) ([]byte, error) {
	payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getTransactionReceipt", RPCParams{hash}, 2})
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	body := bytes.NewReader(payloadBytes)
	req, err := http.NewRequest("POST", Options.rpcProvider, body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	receiptBody, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println(err)
		return nil, err
	}

	defer resp.Body.Close()
	return receiptBody, nil
}
