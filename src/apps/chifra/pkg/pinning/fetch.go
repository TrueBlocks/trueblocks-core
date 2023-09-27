package pinning

import (
	"context"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"path/filepath"
	"strconv"
)

// FetchResult type make it easier to return both download content and
// download size information (for validation purposes)
type FetchResult struct {
	Body       io.ReadCloser
	ContentLen int64 // download size in bytes
}

// FetchFromGateway downloads a chunk from an IPFS gateway using HTTP
func FetchFromGateway(ctx context.Context, gateway, hash string) (*FetchResult, error) {
	url, _ := url.Parse(gateway)
	url.Path = filepath.Join(url.Path, hash)
	request, err := http.NewRequestWithContext(ctx, "GET", url.String(), nil)
	if err != nil {
		// logger.Fatalln("NewRequestWithContext failed in FetFromGateway with", url)
		return nil, err
	}
	response, err := http.DefaultClient.Do(request)
	if err != nil {
		// logger.Fatalln("DefaultClient.Do failed in FetFromGateway with", url)
		return nil, err
	}
	if response.StatusCode != 200 {
		// logger.Fatalln("DefaultClient.Do returned StatusCode not equal to 200 in FetFromGateway with", url)
		return nil, fmt.Errorf("fetch to %s returned status code: %d", url, response.StatusCode)
	}
	body := response.Body

	contentLen, err := strconv.ParseInt(response.Header.Get("Content-Length"), 10, 64)
	if err != nil {
		// logger.Fatalln("Could not parse Content-Length in FetFromGateway with", url)
		return nil, err
	}

	return &FetchResult{
		Body:       body,
		ContentLen: contentLen,
	}, nil
}
