package sdk

import (
	"os"
)

var sdkTestMode = false

func init() {
	sdkTestMode = os.Getenv("TEST_MODE") == "true"
}
