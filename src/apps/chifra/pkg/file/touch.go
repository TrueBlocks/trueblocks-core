// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"fmt"
	"os"
	"time"
)

func Touch(filename string) bool {
	_, err := os.Stat(filename)
	if os.IsNotExist(err) {
		file, err := os.Create(filename)
		if err != nil {
			fmt.Println(err.Error())
			return false
		}
		defer file.Close()
	} else {
		currentTime := time.Now().Local()
		err = os.Chtimes(filename, currentTime, currentTime)
		if err != nil {
			fmt.Println(err.Error())
			return false
		}
	}
	return true
}
