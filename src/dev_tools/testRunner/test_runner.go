package main

import (
	"encoding/json"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
	if !t.ShouldRun(tr.Mode) {
		return nil
	}

	tr.AppendLog(t)

	wasTested := false
	passedTest := false

	os.Setenv("TEST_MODE", "true")
	logger.SetTestMode(true)

	workFn, goldFn, envFn, _ := t.GetOutputPaths(tr.Mode)
	workFile, _ := os.Create(workFn)
	logger.SetLoggerWriter(workFile)
	logger.ToggleDecoration()

	defer func() {
		logger.ToggleDecoration()
		logger.SetLoggerWriter(os.Stderr)
		tr.ReportOneTest(t, wasTested && !passedTest)
	}()

	if file.FileExists(envFn) {
		lines := file.AsciiFileToLines(envFn)
		for _, line := range lines {
			if !strings.HasPrefix(line, "#") {
				logger.Info("Env: " + line)
			}
		}
	}

	msg := t.Route + "?"
	if tr.Mode == "cmd" {
		msg = "chifra "
		if t.Route != "chifra" && t.Tool != "chifra" {
			msg += t.Route + "  "
		}
	}
	logger.Info(msg + t.OptionsForMode(tr.Mode))

	wasTested = true
	if results, err := t.InnerTest(tr.Mode); err != nil {
		type E struct {
			Errors []string `json:"errors"`
		}
		e := E{Errors: []string{err.Error()}}
		bytes, _ := json.MarshalIndent(e, "", "  ")
		results = string(bytes)
		logger.Info(results)
	} else {
		if len(results) > 0 {
			results = strings.ReplaceAll(results, "3735928559", "\"0xdeadbeef\"") // mildly hacky cleaning
			results = strings.ReplaceAll(results, "\\u0026", "&")
			results = strings.ReplaceAll(results, "\\u003c", "<")
			results = strings.ReplaceAll(results, "\\u003d", "=")
			results = strings.ReplaceAll(results, "\\u003e", ">")
			logger.Info(results)
		}
	}

	if workFile != nil {
		workFile.Close()
		workContents := file.AsciiFileToString(workFn)
		goldContents := file.AsciiFileToString(goldFn)
		passedTest = workContents == goldContents
	}

	if wasTested {
		tr.NTested++
	}

	if passedTest {
		tr.NPassed++
	}

	return nil
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

func (tr *Runner) AppendLog(t *TestCase) {
	// s := fmt.Sprintf("%s\t%s.txt\t%s", t.Route, t.Filename, t.OptionsForMode(tr.Mode))
	// tr.Logs[tr.Mode] = append(tr.Logs[tr.Mode], s)
}

var summaryTmpl = `  {{padRight .NameAndMode 25 " "}} ==> {{padRight .Result 8 " "}} {{.NPassed}} of {{.NTested}} passed, {{.Failed}} failed.`

func (tr *Runner) ReportOneTest(t *TestCase, failed bool) {
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

func (tr *Runner) ReportOneMode() {
	// file.AppendToAsciiFile(getLogFile(tr.Mode), strings.Join(tr.Logs[tr.Mode], "\n")+"\n")
	if tr.NTested > 0 {
		colors.ColorsOn()
		fmt.Println(executeTemplate(colors.Yellow, "summary", summaryTmpl, &tr))
		colors.ColorsOff()
	}
}

func (tr *Runner) ReportFinal() {
	colors.ColorsOn()
	tr.Route = "final"
	tr.Mode = "final"
	fmt.Println(executeTemplate(colors.Yellow, "summary", summaryTmpl, &tr))
	fmt.Println("nFails:", len(tr.Fails))
	// for _, fail := range tr.Fails {
	// 	fmt.Printf("%s%s%s\n", colors.Red, fail, colors.Off)
	// }
	colors.ColorsOff()
}

func countOf(testMap map[string][]TestCase, source, mode string) int {
	count := 0
	for _, testCase := range testMap[source] {
		if testCase.ShouldRun(mode) {
			count++
		}
	}
	return count
}
