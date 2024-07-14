// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	tokens "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type tokensOptionsInternal struct {
	Addrs    []string    `json:"addrs,omitempty"`
	BlockIds []string    `json:"blocks,omitempty"`
	Parts    TokensParts `json:"parts,omitempty"`
	ByAcct   bool        `json:"byAcct,omitempty"`
	Changes  bool        `json:"changes,omitempty"`
	NoZero   bool        `json:"noZero,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *tokensOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// TokensBytes implements the chifra tokens command for the SDK.
func (opts *tokensOptionsInternal) TokensBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting tokens struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	return tokens.Tokens(rCtx, w, values)
}

// tokensParseFunc handles special cases such as structs and enums (if any).
func tokensParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*tokensOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(tokens): target is not of correct type")
	}

	if key == "parts" {
		var err error
		values := strings.Split(value, ",")
		if opts.Parts, err = enumFromTokensParts(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetTokensOptions returns a filled-in options instance given a string array of arguments.
func GetTokensOptions(args []string) (*tokensOptionsInternal, error) {
	var opts tokensOptionsInternal
	if err := assignValuesFromArgs(args, tokensParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type tokensGeneric interface {
	types.Token
}

func queryTokens[T tokensGeneric](opts *tokensOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.TokensBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		debugPrint(str, result, err)
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *TokensOptions) toInternal() *tokensOptionsInternal {
	return &tokensOptionsInternal{
		Addrs:    opts.Addrs,
		BlockIds: opts.BlockIds,
		Parts:    opts.Parts,
		ByAcct:   opts.ByAcct,
		Changes:  opts.Changes,
		NoZero:   opts.NoZero,
		Globals:  opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
