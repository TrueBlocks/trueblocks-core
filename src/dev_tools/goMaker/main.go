package main

import (
	"fmt"
	"os"

	"github.com/gocarina/gocsv"
)

type Namer interface {
	Name() string
}

func LoadCsv[T Namer, D any](thePath string, callBack func(*T, *D) (bool, error), data *D) ([]T, error) {
	records := make([]T, 0)
	callbackFunc := func(record T) error {
		ok, err := callBack(&record, data)
		if err != nil {
			return err
		} else if !ok {
			return nil
		}
		if record.Name() != "" {
			records = append(records, record)
		}
		return nil
	}

	if theFile, err := os.OpenFile(thePath, os.O_RDWR, os.ModePerm); err != nil {
		return []T{}, err

	} else {
		defer theFile.Close()
		if err := gocsv.UnmarshalToCallback(theFile, callbackFunc); err != nil {
			return []T{}, err
		}
	}

	return records, nil
}

type CmdLineOption struct {
	Num            string `csv:"num"`
	Group          string `csv:"group"`
	Tags           string `csv:"tags"`
	ApiRoute       string `csv:"api_route"`
	Tool           string `csv:"tool"`
	LongName       string `csv:"longName"`
	HotKey         string `csv:"hotKey"`
	DefVal         string `csv:"def_val"`
	IsRequired     string `csv:"is_required"`
	IsCustomizable string `csv:"is_customizable"`
	IsVisible      string `csv:"is_visible"`
	IsVisibleDocs  string `csv:"is_visible_docs"`
	Generate       string `csv:"generate"`
	OptionType     string `csv:"option_type"`
	DataType       string `csv:"data_type"`
	Description    string `csv:"description"`
}

func (c CmdLineOption) Name() string { return c.LongName }

func ReadCmdOption(cmd *CmdLineOption, data *any) (bool, error) {
	return true, nil
}

type CmdLineEndpoint struct {
	Num           string `csv:"num"`
	Group         string `csv:"group"`
	IsVisible     string `csv:"is_visible"`
	IsVisibleDocs string `csv:"is_visible_docs"`
	ApiGroup      string `csv:"api_group"`
	ApiRoute      string `csv:"api_route"`
	Tool          string `csv:"tool"`
	Summary       string `csv:"summary"`
	Capabilities  string `csv:"capabilities"`
	Description   string `csv:"description"`
}

func (c CmdLineEndpoint) Name() string { return c.ApiRoute }

func ReadCmdEndpoint(cmd *CmdLineEndpoint, data *any) (bool, error) {
	return true, nil
}

func main() {
	thePath := "/Users/jrush/Development/trueblocks-core/src/other/data-models/cmd-line-options.csv"

	cmds, err := LoadCsv[CmdLineOption](thePath, ReadCmdOption, nil)
	if err != nil {
		fmt.Println(err)
		return
	}

	for _, cmd := range cmds {
		fmt.Println(cmd.Tool, cmd.LongName, cmd.OptionType)
	}

	thePath = "/Users/jrush/Development/trueblocks-core/src/other/data-models/cmd-line-endpoints.csv"
	eps, err := LoadCsv[CmdLineEndpoint](thePath, ReadCmdEndpoint, nil)
	if err != nil {
		fmt.Println(err)
		return
	}

	for _, ep := range eps {
		fmt.Println(ep.Tool, ep.ApiGroup, ep.ApiRoute)
	}
}
