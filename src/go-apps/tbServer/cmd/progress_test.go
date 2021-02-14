package trueblocks

import (
	"testing"
	"io"
	"fmt"
)

func TestProgress(t *testing.T) {
	output := ":some command 10 of 100"
	slices := MatchProgress(output)
	slicesLen := len(slices)

	if slicesLen != 4 {
		t.Errorf("Progress regexp should have 4 matches, but got %d", slicesLen)
	}

	if slices[1] != "some command" {
		t.Errorf("First match should be the command name, but got %s", slices[1])
	}

	if slices[2] != "10" {
		t.Errorf("Second match should be the 'done' number, but got %s", slices[2])
	}

	if slices[3] != "100" {
		t.Errorf("Third match should be total number, but got  %s", slices[3])
	}
}

func TestScanForProgress(t *testing.T) {
	reader, writer := io.Pipe()

	go func() {
		fmt.Fprint(writer, ":command one 20 of 100")
		writer.Close()
	}()

	ScanForProgress(reader, func(cp *CommandProgress) {
		if cp.Done != 20 {
			t.Errorf("Done should be 20, but got %d", cp.Done);
		}

		if cp.Total != 100 {
			t.Errorf("Total should be 20, but got %d", cp.Total);
		}

		if cp.Finished != false {
			t.Errorf("Finished should be false, but got %t", cp.Finished);
		}
	});
}
