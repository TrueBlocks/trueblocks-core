// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"net/url"
	"os"
	"reflect"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func TestNamesOptions_getCrudDataHttp(t *testing.T) {
	type fields struct {
		Autoname string
	}
	type args struct {
		r *http.Request
	}
	tests := []struct {
		name     string
		fields   fields
		args     args
		wantData *crud.NameCrud
		wantErr  bool
	}{
		{
			name: "read from valid HTTP form",
			args: args{
				r: (func() (r *http.Request) {
					data := url.Values{}
					data.Set("address", "0x199d5ed7f45f4ee35960cf22eade2076e95b253f")
					data.Set("name", "some name 1")
					fmt.Println("encoded", data.Encode())
					reader := strings.NewReader(data.Encode())
					r = httptest.NewRequest("POST", "/names", reader)
					r.Header.Set("Content-Type", "application/x-www-form-urlencoded")
					return
				})(),
			},
			wantData: &crud.NameCrud{
				Address: crud.Field[base.Address]{
					Value:   base.HexToAddress("0x199d5ed7f45f4ee35960cf22eade2076e95b253f"),
					Updated: true,
				},
				Name: crud.Field[string]{
					Value:   "some name 1",
					Updated: true,
				},
				// Tags:         "",
				// Source:      "",
				// Symbol:      "",
				// Decimals:    "",
				// Description: "",
			},
		},
		{
			name: "read from invalid HTTP form",
			args: args{
				r: (func() (r *http.Request) {
					data := url.Values{}

					data.Set("name", "some name 2")
					fmt.Println("encoded", data.Encode())
					reader := strings.NewReader(data.Encode())
					r = httptest.NewRequest("POST", "/names", reader)
					r.Header.Set("Content-Type", "application/x-www-form-urlencoded")
					return
				})(),
			},
			wantErr: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			opts := &NamesOptions{
				Autoname: tt.fields.Autoname,
			}
			gotData, err := opts.getCrudDataHttp(tt.args.r)
			if (err != nil) != tt.wantErr {
				t.Errorf("NamesOptions.getCrudDataHttp() error = %v, wantErr %v", err, tt.wantErr)
				return
			}
			if !reflect.DeepEqual(gotData, tt.wantData) {
				t.Errorf("NamesOptions.getCrudDataHttp() = %v, want %v", gotData, tt.wantData)
			}
		})
	}
}

func TestNamesOptions_getCrudDataEnv(t *testing.T) {
	setEnvs := func(data *crud.NameCrud) {
		if data.Address.Value.Hex() != "" {
			os.Setenv("TB_NAME_ADDRESS", data.Address.Value.Hex())
		}
		if data.Name.Value != "" {
			os.Setenv("TB_NAME_NAME", data.Name.Value)
		}
		if data.Tags.Value != "" {
			os.Setenv("TB_NAME_TAGS", data.Tags.Value)
		}
		if data.Source.Value != "" {
			os.Setenv("TB_NAME_SOURCE", data.Source.Value)
		}
		if data.Symbol.Value != "" {
			os.Setenv("TB_NAME_SYMBOL", data.Symbol.Value)
		}
		if data.Decimals.Value != "" {
			os.Setenv("TB_NAME_DECIMALS", data.Decimals.Value)
		}
	}
	var expected *crud.NameCrud
	var result *crud.NameCrud
	var err error
	var opts *NamesOptions

	// valid envs
	expected = &crud.NameCrud{
		Address: crud.Field[base.Address]{
			Value:   base.HexToAddress("0x199d5ed7f45f4ee35960cf22eade2076e95b253f"),
			Updated: true,
		},
		Name: crud.Field[string]{
			Value:   "some name",
			Updated: true,
		},
	}
	setEnvs(expected)
	opts = &NamesOptions{}
	chain := utils.GetTestChain()
	opts.Conn = rpc.TempConnection(chain)
	result, err = opts.getCrudDataEnv()
	if err != nil {
		t.Fatal(err)
	}
	if !reflect.DeepEqual(result, expected) {
		t.Fatal("wrong value:", fmt.Sprintf("%+v", result))
	}
	os.Clearenv()

	// invalid envs
	expected = &crud.NameCrud{
		Address: crud.Field[base.Address]{
			Value:   base.ZeroAddr,
			Updated: true,
		},
		Name: crud.Field[string]{
			Value:   "some name",
			Updated: true,
		},
	}
	setEnvs(expected)
	opts = &NamesOptions{}
	opts.Conn = rpc.TempConnection(chain)
	_, err = opts.getCrudDataEnv()
	if err == nil {
		t.Fatal("error expected")
	}
	os.Clearenv()
}
