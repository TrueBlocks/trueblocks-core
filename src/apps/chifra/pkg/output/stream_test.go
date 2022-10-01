package output

import (
	"bytes"
	"context"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"testing"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

var input = types.SimpleReceipt{
	BlockHash:         common.Hash{1, 2, 3},
	BlockNumber:       123,
	ContractAddress:   common.Address{1, 5, 5},
	CumulativeGasUsed: "500",
	From:              common.Address{1, 5, 6},
	GasUsed:           500,
	EffectiveGasPrice: 500,
	Logs: []types.SimpleLog{
		{
			Address:          common.Address{1, 5, 7},
			LogIndex:         0,
			BlockNumber:      124,
			TransactionIndex: 1,
		},
	},
	Status:           1,
	IsError:          false,
	To:               common.Address{1, 5, 8},
	TransactionHash:  common.Hash{1, 2, 4},
	TransactionIndex: 1,
}

func helperStreamFormats(w csv.Writer, outputBuffer *bytes.Buffer, format string, expectKeys bool) error {
	buffer := new(bytes.Buffer)
	StreamModel(buffer, input.Model(false, format), OutputOptions{
		Format:   format,
		NoHeader: !expectKeys,
	})
	result := buffer.String()

	var expectedItems []string
	for _, key := range input.Model(false, format).Order {
		expectedItems = append(expectedItems, fmt.Sprint(input.Model(false, format).Data[key]))
	}
	if expectKeys {
		w.Write(input.Model(false, format).Order)
	}
	w.Write(expectedItems)
	w.Flush()
	expected := outputBuffer.String()

	if result != expected {
		return fmt.Errorf("mismatch for format '%s': <%s>", format, result)
	}
	return nil
}

func TestStreamFormats(t *testing.T) {
	outputBuffer := &bytes.Buffer{}
	csvWriter := csv.NewWriter(outputBuffer)

	err := helperStreamFormats(*csvWriter, outputBuffer, "csv", false)
	if err != nil {
		t.Fatal(err)
	}

	csvWriter.Comma = '\t'
	outputBuffer.Reset()
	err = helperStreamFormats(*csvWriter, outputBuffer, "txt", false)
	if err != nil {
		t.Fatal(err)
	}

	csvWriter.Comma = ':'
	outputBuffer.Reset()
	err = helperStreamFormats(*csvWriter, outputBuffer, ":", false)
	if err != nil {
		t.Fatal(err)
	}
}

func TestStreamJson(t *testing.T) {
	outputBuffer := &bytes.Buffer{}
	StreamModel(outputBuffer, input.Model(false, "json"), OutputOptions{
		Format:     "json",
		JsonIndent: "  ",
	})
	result := outputBuffer.String()

	expected, err := json.MarshalIndent(input.Model(false, "json").Data, "    ", "  ")
	if err != nil {
		t.Fatal(err)
	}
	if result != string(expected) {
		t.Fatal("mismatch in JSON format")
	}
}

func TestStreamPrintKeys(t *testing.T) {
	outputBuffer := &bytes.Buffer{}
	csvWriter := csv.NewWriter(outputBuffer)

	err := helperStreamFormats(*csvWriter, outputBuffer, "csv", true)
	if err != nil {
		t.Fatal(err)
	}
}

func TestStreamTemplate(t *testing.T) {
	tmpl := template.Must(template.New("").Parse("{{.blockNumber}} used {{.gasUsed}}{{if .Nonexistent}}111{{else}}.{{end}}"))
	outputBuffer := &bytes.Buffer{}

	err := StreamWithTemplate(outputBuffer, input.Model(false, ""), tmpl)
	if err != nil {
		t.Fatal(err)
	}
	if outputBuffer.String() != "123 used 500." {
		t.Fatal("mismatched template text:", outputBuffer.String())
	}
}

func TestStreamMany(t *testing.T) {
	buffer := &bytes.Buffer{}

	renderData := func(models chan types.Modeler[types.RawReceipt], errors chan error) {
		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(123),
			TransactionIndex: 1,
			TransactionHash:  common.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}

		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(124),
			TransactionIndex: 5,
			TransactionHash:  common.HexToHash("0xdeadbeef2"),
			GasUsed:          200,
			Status:           1,
			IsError:          false,
		}
	}

	// Print the values and try to re-parse them to check if
	// we get the same data
	StreamMany(context.Background(), renderData, OutputOptions{
		Writer: buffer,
		Format: "json",
	})

	type R = struct {
		Data []types.SimpleReceipt `json:"data"`
	}
	var result R
	err := json.Unmarshal(buffer.Bytes(), &result)
	if err != nil {
		t.Fatal(err)
	}

	if result.Data[0].BlockNumber != uint64(123) {
		t.Fatal("mismatched data")
	}
	if result.Data[1].BlockNumber != uint64(124) {
		t.Fatal("mismatched data")
	}
}

func TestApiFormat(t *testing.T) {
	outputBuffer := &bytes.Buffer{}
	renderData := func(models chan types.Modeler[types.RawReceipt], errors chan error) {
		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(123),
			TransactionIndex: 1,
			TransactionHash:  common.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}
	}
	err := StreamMany(context.Background(), renderData, OutputOptions{
		Writer: outputBuffer,
		Format: "api",
	})
	if err != nil {
		t.Fatal(err)
	}
	var result = map[string]interface{}{}
	err = json.Unmarshal(outputBuffer.Bytes(), &result)
	if err != nil {
		t.Fatal(err)
	}

	if _, ok := result["meta"]; !ok {
		t.Fatal("meta is missing")
	}
}
