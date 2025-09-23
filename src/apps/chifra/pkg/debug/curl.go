package debug

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var devDebug = false
var devDebugMethod = ""

func init() {
	devDebugMethod = os.Getenv("TB_DEBUG_CURL")
	devDebug = len(devDebugMethod) > 0
}

type Debuggable interface {
	Url() string
	Body() string
	Headers() string
	Method() string
	Payload() string
}

func shouldDebugCurl(method string) bool {
	if !devDebug {
		return false

	} else {
		if len(devDebugMethod) == 0 {
			logger.ShouldNotHappen("devDeb is true but devDebugMethod is empty")
		}
		switch devDebugMethod {
		case "file":
			return true
		case "true":
			return true
		case "testing":
			return true
		default:
			// allows for specifying which method to debug (or none)
			return method == devDebugMethod
		}
	}
}

type strDebug string

func (c strDebug) Url() string {
	return string(c)
}

func (c strDebug) Body() string {
	return `curl "[{url}]"`
}

func (c strDebug) Headers() string {
	return ``
}

func (c strDebug) Method() string {
	return ""
}

func (c strDebug) Payload() string {
	return ""
}

func DebugCurlStr(url string) {
	DebugCurl(strDebug(url))
}

func DebugCurl(debuggable Debuggable) {
	if !shouldDebugCurl(debuggable.Method()) {
		return
	}

	url := "--url--"
	payload := "--payload--"
	headers := "--headers--"
	if devDebugMethod != "testing" {
		url = debuggable.Url()
		payload = debuggable.Payload()
		headers = debuggable.Headers()
	}

	var curlCmd = debuggable.Body()
	curlCmd = strings.ReplaceAll(curlCmd, "[{url}]", url)
	curlCmd = strings.ReplaceAll(curlCmd, "[{headers}]", headers)
	curlCmd = strings.ReplaceAll(curlCmd, "[{payload}]", payload)
	if devDebugMethod == "file" {
		_ = file.AppendToAsciiFile("./curl.log", curlCmd+"\n")
	} else {
		logger.ToggleDecoration()
		logger.Info(curlCmd)
		logger.ToggleDecoration()
	}
}

func CloseDebugger() {
	if !devDebug {
		return
	}
	logger.Info("Closing curl debugger")
}
