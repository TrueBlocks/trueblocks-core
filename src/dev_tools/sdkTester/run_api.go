package main

import (
	"io/ioutil"
	"log"
	"net/http"
)

func (t *TestCase) ApiTest() (string, error) {
	url := "http://" + getApiUrl() + "/" + t.Route + "?" + t.ApiOptions
	if response, err := http.Get(url); err != nil {
		log.Printf("Failed to query URL %s: %v", url, err)
	} else {
		defer response.Body.Close()
		if body, err := ioutil.ReadAll(response.Body); err != nil {
			log.Printf("Failed to read response from URL %s: %v", url, err)
		} else {
			return string(body), nil
		}
	}
	return "", nil
}
