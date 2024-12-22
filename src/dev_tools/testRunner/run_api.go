package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
)

func (t *TestCase) ApiTest() (string, error) {
	url := "http://" + apiUrl + "/" + t.Route + "?" + t.ApiOptions
	request, err := http.NewRequest("GET", url, nil) // Default to GET request
	if err != nil {
		fmt.Println("Error creating request:", err)
		return "", err
	}

	// Set the User-Agent header
	request.Header.Set("User-Agent", "testRunner")

	// Check if config.file exists
	// TODO: NOTE THAT THIS DOES NOT WORK. IT DOES NOT
	// TODO: FIND THE RIGHT _curl.txt FILE FOR THIS
	// TODO: TEST CASE.
	if _, err := os.Stat("config.file"); err == nil {
		configData, err := os.ReadFile("config.file")
		if err != nil {
			fmt.Println("Error reading config file:", err)
			return "", err
		}

		lines := strings.Split(string(configData), "\n")
		for _, line := range lines {
			if strings.HasPrefix(line, "-X ") {
				verb := strings.TrimSpace(strings.TrimPrefix(line, "-X "))
				request.Method = verb
			}
			if strings.HasPrefix(line, "-H ") {
				header := strings.TrimSpace(strings.TrimPrefix(line, "-H "))
				headerParts := strings.SplitN(header, ": ", 2)
				if len(headerParts) == 2 {
					request.Header.Set(headerParts[0], headerParts[1])
				}
			}
			if strings.HasPrefix(line, "-d ") {
				data := strings.TrimSpace(strings.TrimPrefix(line, "-d "))
				request.Body = io.NopCloser(strings.NewReader(data))
				request.ContentLength = int64(len(data))
			}
		}
	}

	// Create an HTTP client and send the request
	client := &http.Client{}
	response, err := client.Do(request)
	if err != nil {
		fmt.Println("Error making request:", err)
		return "", err
	}
	defer response.Body.Close()

	body, err := io.ReadAll(response.Body)
	if err != nil {
		fmt.Println("Error reading response body:", err)
		return "", err
	}

	return string(body), nil
}
