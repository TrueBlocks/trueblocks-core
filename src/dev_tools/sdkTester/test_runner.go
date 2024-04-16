package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type Runner struct {
	Route      string              `json:"route"`
	Mode       string              `json:"mode"`
	Msg        string              `json:"msg"`
	TotalTests int                 `json:"totalTests"`
	NFiltered  int                 `json:"nFiltered"`
	NTested    int                 `json:"nTested"`
	NPassed    int                 `json:"nPassed"`
	Logs       map[string][]string `json:"logs"`
	Fails      []string            `json:"fails"`
}

func NewSummary() *Runner {
	return &Runner{}
}

func NewRunner(testMap map[string][]TestCase, item, mode, source string) *Runner {
	tr := Runner{
		Route:     item,
		Mode:      mode,
		NFiltered: countOf(testMap, source, mode),
	}
	tr.Logs = make(map[string][]string)
	return &tr
}

func (tr *Runner) Run(t *TestCase) error {
	if !t.ShouldTest(tr.Mode) {
		return nil
	}

	var tested, passed bool
	var err error
	switch tr.Mode {
	case "sdk":
		tested, passed, err = tr.RunSdkTest(t)
	case "api":
		tested, passed, err = tr.RunApiTest(t)
	case "cmd":
		tested, passed, err = tr.RunCmdTest(t)
	}
	if tested {
		tr.NTested++
	}
	if passed {
		tr.NPassed++
	}
	return err
}

func (t *Runner) Result() string {
	if t.NPassed == t.NTested {
		return "ok"
	}
	return "X "
}

func (t *Runner) NameAndMode() string {
	return t.Route + " (" + t.Mode + " mode)"
}

func (t *Runner) Failed() string {
	return fmt.Sprintf("%d", t.NTested-t.NPassed)
}

func getLogFile(mode string) string {
	return "../src/dev_tools/sdkTester/generated/test_" + mode + ".log"
}

func (tr *Runner) AppendLog(t *TestCase) {
	if len(os.Getenv("TB_TEST_ROUTE")) > 0 {
		return
	}
	s := fmt.Sprintf("%s\t%s.txt\t%s", t.Route, t.Filename, t.OptionsForMode(tr.Mode))
	tr.Logs[tr.Mode] = append(tr.Logs[tr.Mode], s)
}

func (tr *Runner) Report() {
	file.AppendToAsciiFile(getLogFile(tr.Mode), strings.Join(tr.Logs[tr.Mode], "\n")+"\n")
	colors.ColorsOn()
	fmt.Println(executeTemplate(colors.Yellow, "summary", summaryTmpl, &tr))
	colors.ColorsOff()
}

func (tr *Runner) ReportFinal(t *TestCase, failed bool) {
	colors.ColorsOn()
	fmt.Println(executeTemplate(colors.Yellow, "summary", summaryTmpl, &tr))
	for _, fail := range tr.Fails {
		fmt.Printf("%s%s%s\n", colors.Red, fail, colors.Off)
	}
	colors.ColorsOff()
}

func (tr *Runner) ReportOne(t *TestCase, failed bool) {
	eol := "\r"
	if failed || os.Getenv("TB_REMOTE_TESTING") == "true" {
		eol = "\n"
	}

	mark := "[passed " + cm["greenCheck"] + "]"
	color := colors.White
	if failed {
		mark = "[failed " + cm["redX"] + "]"
		color = colors.Red
		tr.Fails = append(tr.Fails, t.Mode+"|"+t.Route+"|"+t.Filename)
	}

	colors.ColorsOn()
	skip := strings.Repeat(" ", utils.Max(0, 120-len(t.ApiOptions)-40))
	rPadded := padRight(t.Route, 15, false, ".")
	fPadded := padRight(t.Filename, 30, false, ".")
	tOpts := t.ApiOptions[:utils.Min(len(t.ApiOptions), 40)]
	fmt.Printf("%s    %s %d-%d %s %s%s%s%s%s%s", color, mark, tr.NTested, tr.NFiltered, tr.Mode, rPadded, fPadded, tOpts, skip, colors.Off, eol)
	colors.ColorsOff()
}

func countOf(testMap map[string][]TestCase, source, mode string) int {
	count := 0
	for _, testCase := range testMap[source] {
		if testCase.ShouldTest(mode) {
			count++
		}
	}
	return count
}
