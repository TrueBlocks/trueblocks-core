package utils

import (
	"os"
	"os/exec"
	"syscall"
)

func System(cmd string) int {
	c := exec.Command("sh", "-c", cmd)
	c.Stdin = os.Stdin
	c.Stdout = os.Stdout
	c.Stderr = os.Stderr
	if err := c.Run(); err == nil {
		return 0
	}

	// Figure out the exit code
	if ws, ok := c.ProcessState.Sys().(syscall.WaitStatus); ok {
		if ws.Exited() {
			return ws.ExitStatus()
		}

		if ws.Signaled() {
			return -int(ws.Signal())
		}
	}

	return -1
}

/*
func systemToString(cmd string) string {
	c := exec.Command("sh", "-c", cmd)
	c.Stdin = os.Stdin

	if ff, err := os.Create("/tmp/output.txt"); err != nil {
		return err.Error()

	} else {
		c.Stdout = ff
		c.Stderr = ff

		if err := c.Run(); err != nil {
			return err.Error()
		}

		ret := file.AsciiFileToString("/tmp/output.txt")
		os.Remove("/tmp/output.txt")
		return strings.Trim(ret, "\n\r\t ,")
	}
}
*/
