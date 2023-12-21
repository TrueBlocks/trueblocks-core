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
	Method() string
	Payload() string
}

func shouldDebugCurl(method string) bool {
	if !devDebug {
		return false

	} else {
		if len(devDebugMethod) == 0 {
			logger.Fatal("Implementation error. Should not happen.")
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

type Basic string

func (c Basic) Url() string {
	return string(c)
}

func (c Basic) Body() string {
	return `curl "[{url}]"`
}

func (c Basic) Method() string {
	return ""
}

func (c Basic) Payload() string {
	return ""
}

func DebugCurl(debuggable Debuggable) {
	if !shouldDebugCurl(debuggable.Method()) {
		return
	}

	url := "--url--"
	payload := "--payload--"
	if devDebugMethod != "testing" {
		url = debuggable.Url()
		payload = debuggable.Payload()
	}

	var curlCmd = debuggable.Body()
	curlCmd = strings.Replace(curlCmd, "[{url}]", url, -1)
	curlCmd = strings.Replace(curlCmd, "[{payload}]", payload, -1)
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
