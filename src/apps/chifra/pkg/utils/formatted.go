// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package utils

import (
	"math/big"
	"strings"

	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/params"
)

func weiToEther(wei *big.Int) *big.Float {
	// Copied from https://github.com/ethereum/go-ethereum/issues/21221#issuecomment-805852059
	f := new(big.Float)
	f.SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	f.SetMode(big.ToNearestEven)
	fWei := new(big.Float)
	fWei.SetPrec(236) //  IEEE 754 octuple-precision binary floating-point format: binary256
	fWei.SetMode(big.ToNearestEven)
	return f.Quo(fWei.SetInt(wei), big.NewFloat(params.Ether))
}

func FormattedValue(in big.Int, asEther bool, decimals int) string {
	if asEther {
		return weiToEther(&in).Text('f', -1*decimals)
	}
	return in.Text(10)
}

func FormattedDate(ts int64) string {
	return gostradamus.FromUnixTimestamp(ts).Format("2006-01-02 15:04:05 UTC")
}

func FormattedCode(verbose bool, code string) string {
	codeLen := len(code)
	if verbose || codeLen <= 128 {
		return code
	}

	return strings.Join(
		[]string{
			code[:15],
			code[codeLen-15:],
		},
		"...",
	)
}

func FormattedHash(verbose bool, code string) string {
	codeLen := len(code)
	if verbose || codeLen <= 11 {
		return code
	}

	return strings.Join(
		[]string{
			code[:6],
			code[codeLen-4:],
		},
		"...",
	)
}
