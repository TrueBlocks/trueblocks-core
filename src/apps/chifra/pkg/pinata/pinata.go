package pinata

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"os"
	fp "path/filepath"
	"time"
)

const (
	PIN_FILE_URL = "https://api.pinata.cloud/pinning/pinFileToIPFS"
	PIN_HASH_URL = "https://api.pinata.cloud/pinning/pinByHash"
)

type Pinata struct {
	Apikey string
	Secret string
}

func (p *Pinata) PinFile(filepath string) (string, error) {
	file, err := os.Open(filepath)
	if err != nil {
		return "", err
	}
	defer file.Close()

	r, w := io.Pipe()
	m := multipart.NewWriter(w)

	go func() {
		defer w.Close()
		defer m.Close()

		part, err := m.CreateFormFile("file", fp.Base(file.Name()))
		if err != nil {
			return
		}

		if _, err = io.Copy(part, file); err != nil {
			return
		}
	}()

	req, err := http.NewRequest(http.MethodPost, PIN_FILE_URL, r)
	req.Header.Add("Content-Type", m.FormDataContentType())
	req.Header.Add("pinata_secret_api_key", p.Secret)
	req.Header.Add("pinata_api_key", p.Apikey)

	client := &http.Client{
		Timeout: 30 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	var dat map[string]interface{}
	if err := json.Unmarshal(data, &dat); err != nil {
		return "", err
	}

	if out, err := dat["error"].(string); err {
		return "", fmt.Errorf(out)
	}
	if hash, ok := dat["IpfsHash"].(string); ok {
		return hash, nil
	}

	return "", fmt.Errorf("Pin file to Pinata failure.")
}

func (p *Pinata) PinHash(hash string) (bool, error) {
	if hash == "" {
		return false, fmt.Errorf("HASH: %s is invalid.", hash)
	}

	jsonValue, _ := json.Marshal(map[string]string{"hashToPin": hash})

	req, err := http.NewRequest(http.MethodPost, PIN_HASH_URL, bytes.NewBuffer(jsonValue))
	req.Header.Set("Content-Type", "application/json")
	req.Header.Add("pinata_secret_api_key", p.Secret)
	req.Header.Add("pinata_api_key", p.Apikey)

	client := &http.Client{
		Timeout: 30 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil {
		return false, err
	}
	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return false, err
	}

	var dat map[string]interface{}
	if err := json.Unmarshal(data, &dat); err != nil {
		return false, err
	}

	if h, ok := dat["ipfsHash"].(string); ok {
		return h == hash, nil
	}

	return false, fmt.Errorf("Pin hash to Pinata failure.")
}

// TODO: BOGUS - OLD C++ CODE TO READ FROM PINATA
/*
//---------------------------------------------------------------------------
// Assumes the license is valid
bool pinlib_getPinList(const CPinApiLicense& lic, string& result) {
    result = "";
    result.clear();

    bool ret = true;  // assume success

    CURL* curl;
    curl = curl_easy_init();  // TODO(tjayrush): Creating and destroying curl context is slow
    if (!curl) {
        curl_easy_cleanup(curl);
        result = "Could not open curl context";
        ret = false;
    } else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        // TODO(tjayrush): This has pagination, I just didn't notice it
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.pinata.cloud/data/pinList?status=pinned");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("pinata_api_key: " + lic.apiKey).c_str());
        headers = curl_slist_append(headers, ("pinata_secret_api_key: " + lic.secretKey).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
        CURLcode res = curl_easy_perform(curl);
        if (res != 0) {
            curl_easy_cleanup(curl);
            result = curl_easy_strerror(res);
            ret = false;
        }
    }
    curl_easy_cleanup(curl);
    if (!ret)  // if we already have an error
        return ret;

    if (result.empty()) {
        result = "No result";
        return false;
    }

    return true;
}
*/
