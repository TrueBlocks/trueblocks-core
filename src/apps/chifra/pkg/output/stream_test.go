package output

import (
	"bytes"
	"context"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"testing"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var input = types.SimpleReceipt{
	BlockHash:         base.HexToHash("0x123"),
	BlockNumber:       123,
	ContractAddress:   base.HexToAddress("0xdafea492d9c6733ae3d56b7ed1adb60692c98bc5"),
	CumulativeGasUsed: "500",
	From:              base.HexToAddress("0xfd4536dd5a81ecfd1a4b30111a01d129e7567ff8"),
	GasUsed:           500,
	EffectiveGasPrice: 500,
	Logs: []types.SimpleLog{
		{
			Address:          base.HexToAddress("0x02ef66278c3c88ff929a5c84c46fbfb83614382e"),
			LogIndex:         0,
			BlockNumber:      124,
			TransactionIndex: 1,
		},
	},
	Status:           1,
	IsError:          false,
	To:               base.HexToAddress("0x917e5e52ac55098d8caa1709e33178aadd9d4901"),
	TransactionHash:  base.HexToHash("0x124"),
	TransactionIndex: 1,
}

func helperStreamFormats(w csv.Writer, outputBuffer *bytes.Buffer, format string, expectKeys bool) error {
	buffer := new(bytes.Buffer)
	StreamModel(buffer, input.Model(false, format, nil), OutputOptions{
		Format:   format,
		NoHeader: !expectKeys,
	})
	result := buffer.String()

	var expectedItems []string
	data := input.Model(false, format, nil).Data
	for _, key := range input.Model(false, format, nil).Order {
		expectedItems = append(expectedItems, fmt.Sprint(data[key]))
	}
	if expectKeys {
		w.Write(input.Model(false, format, nil).Order)
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
	w := NewJsonWriter(outputBuffer)
	w.DefaultField = DefaultField{Key: "Data", FieldType: FieldArray}
	StreamModel(w, input.Model(false, "json", nil), OutputOptions{
		Format:     "json",
		JsonIndent: "  ",
	})
	w.Close()
	result := outputBuffer.String()
	expected, err := json.MarshalIndent(struct {
		Data []interface{}
	}{
		Data: []interface{}{input.Model(false, "json", nil).Data},
	}, "", "  ")

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

	err := StreamWithTemplate(outputBuffer, input.Model(false, "", nil), tmpl)
	if err != nil {
		t.Fatal(err)
	}
	if outputBuffer.String() != "123 used 500." {
		t.Fatal("mismatched template text:", outputBuffer.String())
	}
}

func TestStreamMany(t *testing.T) {
	buffer := &bytes.Buffer{}
	jw := NewJsonWriter(buffer)
	jw.DefaultField = DefaultField{
		Key:       "data",
		FieldType: FieldArray,
	}

	renderData := func(models chan types.Modeler[types.RawReceipt], errorChan chan error) {
		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(123),
			TransactionIndex: 1,
			TransactionHash:  base.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}

		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(124),
			TransactionIndex: 5,
			TransactionHash:  base.HexToHash("0xdeadbeef2"),
			GasUsed:          200,
			Status:           1,
			IsError:          false,
		}
	}

	// Print the values and try to re-parse them to check if
	// we get the same data
	StreamMany(context.Background(), renderData, OutputOptions{
		Writer: jw,
		Format: "json",
	})
	jw.Close()

	type R = struct {
		Data []types.SimpleReceipt `json:"data"`
	}
	var result R
	err := json.Unmarshal(buffer.Bytes(), &result)
	if err != nil {
		logger.Error(buffer.String())
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
	renderData := func(models chan types.Modeler[types.RawReceipt], errorChan chan error) {
		models <- &types.SimpleReceipt{
			BlockNumber:      uint64(123),
			TransactionIndex: 1,
			TransactionHash:  base.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}
	}
	err := StreamMany(context.Background(), renderData, OutputOptions{
		Writer: outputBuffer,
		Format: "api",
	})
	if err == nil {
		t.Fatal("Api format is no longer allow. Should error here.")
	}
}
