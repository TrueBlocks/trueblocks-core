package main

import (
	"fmt"
	"io"
	"net/http"
)

func (t *TestCase) ApiTest() (string, error) {
	url := "http://" + getApiUrl() + "/" + t.Route + "?" + t.ApiOptions
	request, err := http.NewRequest("GET", url, nil) // Create a new request
	if err != nil {
		fmt.Println("Error creating request:", err)
		return "", err
	}

	// Set the User-Agent header
	request.Header.Set("User-Agent", "testRunner")

	// Create an HTTP client and send the request
	client := &http.Client{}
	response, err := client.Do(request)
	if err != nil {
		fmt.Println("Error making GET request:", err)
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
