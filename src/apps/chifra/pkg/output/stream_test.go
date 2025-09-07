package output

import (
	"bytes"
	"encoding/csv"
	"encoding/json"
	"fmt"
	"testing"
	"text/template"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var input = types.Receipt{
	BlockHash:         base.HexToHash("0x123"),
	BlockNumber:       123,
	ContractAddress:   base.HexToAddress("0xdafea492d9c6733ae3d56b7ed1adb60692c98bc5"),
	CumulativeGasUsed: 500,
	From:              base.HexToAddress("0xfd4536dd5a81ecfd1a4b30111a01d129e7567ff8"),
	GasUsed:           500,
	EffectiveGasPrice: 500,
	Logs: []types.Log{
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

func helperStreamFormats(w csv.Writer, outputBuffer *bytes.Buffer, chain, format string, expectKeys bool) error {
	buffer := new(bytes.Buffer)
	StreamModel(buffer, input.Model(chain, format, false, nil), OutputOptions{
		Format:   format,
		NoHeader: !expectKeys,
	})
	result := buffer.String()

	var expectedItems []string
	data := input.Model(chain, format, false, nil).Data
	for _, key := range input.Model(chain, format, false, nil).Order {
		expectedItems = append(expectedItems, fmt.Sprint(data[key]))
	}
	if expectKeys {
		w.Write(input.Model(chain, format, false, nil).Order)
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
	chain := utils.GetTestChain()
	outputBuffer := &bytes.Buffer{}
	csvWriter := csv.NewWriter(outputBuffer)

	err := helperStreamFormats(*csvWriter, outputBuffer, chain, "csv", false)
	if err != nil {
		t.Fatal(err)
	}

	csvWriter.Comma = '\t'
	outputBuffer.Reset()
	err = helperStreamFormats(*csvWriter, outputBuffer, chain, "txt", false)
	if err != nil {
		t.Fatal(err)
	}

	csvWriter.Comma = ':'
	outputBuffer.Reset()
	err = helperStreamFormats(*csvWriter, outputBuffer, chain, ":", false)
	if err != nil {
		t.Fatal(err)
	}
}

func TestStreamJson(t *testing.T) {
	chain := utils.GetTestChain()
	outputBuffer := &bytes.Buffer{}
	w := NewJsonWriter(outputBuffer)
	w.DefaultField = DefaultField{Key: "Data", FieldType: FieldArray}
	StreamModel(w, input.Model(chain, "json", false, nil), OutputOptions{
		Format:     "json",
		JsonIndent: "  ",
	})
	w.Close()
	result := outputBuffer.String()
	expected, err := json.MarshalIndent(struct {
		Data []interface{}
	}{
		Data: []interface{}{input.Model(chain, "json", false, nil).Data},
	}, "", "  ")

	if err != nil {
		t.Fatal(err)
	}
	if result != string(expected) {
		t.Fatal("mismatch in JSON format")
	}
}

func TestStreamPrintKeys(t *testing.T) {
	chain := utils.GetTestChain()
	outputBuffer := &bytes.Buffer{}
	csvWriter := csv.NewWriter(outputBuffer)

	err := helperStreamFormats(*csvWriter, outputBuffer, chain, "csv", true)
	if err != nil {
		t.Fatal(err)
	}
}

func TestStreamTemplate(t *testing.T) {
	chain := utils.GetTestChain()
	tmpl := template.Must(template.New("").Parse("{{.blockNumber}} used {{.gasUsed}}{{if .Nonexistent}}111{{else}}.{{end}}"))
	outputBuffer := &bytes.Buffer{}

	err := StreamWithTemplate(outputBuffer, input.Model(chain, "", false, nil), tmpl)
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

	renderData := func(modelChan chan types.Modeler, errorChan chan error) {
		modelChan <- &types.Receipt{
			BlockNumber:      123,
			TransactionIndex: 1,
			TransactionHash:  base.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}

		modelChan <- &types.Receipt{
			BlockNumber:      124,
			TransactionIndex: 5,
			TransactionHash:  base.HexToHash("0xdeadbeef2"),
			GasUsed:          200,
			Status:           1,
			IsError:          false,
		}
	}

	// Print the values and try to re-parse them to check if
	// we get the same data
	rCtx := NewRenderContext()
	StreamMany(rCtx, renderData, OutputOptions{
		Writer: jw,
		Format: "json",
	})
	jw.Close()

	type R = struct {
		Data []types.Receipt `json:"data"`
	}
	var result R
	err := json.Unmarshal(buffer.Bytes(), &result)
	if err != nil {
		t.Error(buffer.String())
		t.Fatal(err)
	}

	if result.Data[0].BlockNumber != 123 {
		t.Fatal("mismatched data")
	}
	if result.Data[1].BlockNumber != 124 {
		t.Fatal("mismatched data")
	}
}

func TestApiFormat(t *testing.T) {
	outputBuffer := &bytes.Buffer{}
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		modelChan <- &types.Receipt{
			BlockNumber:      123,
			TransactionIndex: 1,
			TransactionHash:  base.HexToHash("0xdeadbeef"),
			GasUsed:          100,
			Status:           1,
			IsError:          false,
		}
	}
	rCtx := NewRenderContext()
	err := StreamMany(rCtx, fetchData, OutputOptions{
		Writer: outputBuffer,
		Format: "api",
	})
	if err == nil {
		t.Fatal("Api format is no longer allow. Should error here.")
	}
}
