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
				Address: crud.CrudField{
					Value:   "0x199d5ed7f45f4ee35960cf22eade2076e95b253f",
					Updated: true,
				},
				Name: crud.CrudField{
					Value:   "some name 1",
					Updated: true,
				},
				IsCustom: crud.CrudField{
					Value:   "true",
					Updated: true,
				},
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
			gotData, err := crud.NewNameCrud(len(opts.Autoname) > 0, tt.args.r)
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
		if data.Address.Value != "" {
			os.Setenv("TB_NAME_ADDRESS", data.Address.Value)
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

	// valid envs
	expected = &crud.NameCrud{
		Address: crud.CrudField{
			Value:   "0x199d5ed7f45f4ee35960cf22eade2076e95b253f",
			Updated: true,
		},
		Name: crud.CrudField{
			Value:   "some name",
			Updated: true,
		},
		IsCustom: crud.CrudField{
			Value:   "true",
			Updated: true,
		},
	}
	setEnvs(expected)
	result, err = crud.NewNameCrud(true /* nameRequired */, nil /* request */)
	if err != nil {
		t.Fatal(err)
	}
	if !reflect.DeepEqual(result, expected) {
		t.Fatal("wrong value:", fmt.Sprintf("%+v", result))
	}
	os.Clearenv()

	// invalid envs
	expected = &crud.NameCrud{
		Address: crud.CrudField{
			Value:   base.ZeroAddr.Hex(),
			Updated: true,
		},
		Name: crud.CrudField{
			Value:   "some name",
			Updated: true,
		},
	}
	setEnvs(expected)
	_, err = crud.NewNameCrud(true /* nameRequired */, nil /* request */)
	if err == nil {
		t.Fatal("error expected")
	}
	os.Clearenv()
}
