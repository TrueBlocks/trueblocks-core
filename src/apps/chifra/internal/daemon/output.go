// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package daemonPkg

// EXISTING_CODE
import (
	"fmt"
	"net/http"
	"os"
	"os/exec"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunDaemon handles the daemon command for the command line. Returns error only as per cobra.
func RunDaemon(cmd *cobra.Command, args []string) (err error) {
	opts := daemonFinishParse(args)
	outputHelpers.SetWriterForCommand("daemon", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.DaemonInternal()
	return
}

// ServeDaemon handles the daemon command for the API. Returns error and a bool if handled
func ServeDaemon(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := daemonFinishParseApi(w, r)
	outputHelpers.InitJsonWriterApi("daemon", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.DaemonInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("daemon", err, &opts.Globals)
	return
}

// DaemonInternal handles the internal workings of the daemon command.  Returns error and a bool if handled
func (opts *DaemonOptions) DaemonInternal() (err error, handled bool) {
	err = opts.validateDaemon()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	var processes OurProcesses

	var wg sync.WaitGroup
	api, err := startApi(&wg)
	if err != nil {
		return err, true
	}

	processes = append(processes, api)
	for _, proc := range processes {
		fmt.Println(proc)
	}
	fmt.Println(wg)
	wg.Wait()

	// EXISTING_CODE

	return
}

// EXISTING_CODE
func startApi(wg *sync.WaitGroup) (OurProcess, error) {
	cmd := exec.Command("chifra", "serve")
	cmd.Stderr = os.Stderr
	cmd.Stdout = os.Stdout
	err := cmd.Start()
	if err != nil {
		return OurProcess{}, err
	}
	pid := cmd.Process.Pid
	wg.Add(1)
	go func() {
		defer wg.Done()
		err = cmd.Wait()
		if err != nil {
			fmt.Printf("Api command finished with error: %v\n", err)
		} else {
			fmt.Println("Api command finished")
		}
	}()

	fmt.Println(colors.BrightYellow, "Spawned api server to pid", pid, colors.Off)
	proc := OurProcess{Name: "chifra serve", Pid: pid}
	return proc, nil
}

type OurProcess struct {
	Name string
	Pid  int
}
type OurProcesses []OurProcess

// EXISTING_CODE
