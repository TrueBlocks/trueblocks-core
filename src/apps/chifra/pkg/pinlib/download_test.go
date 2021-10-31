package pinlib

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"testing"
)

var manifestJSONSource = `
{
  "fileName": "pin-manifest.json",
  "indexFormat": "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2",
  "bloomFormat": "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD",
  "commitHash": "f29699d3281e41cb011ddfbe50b7f01bfe5e3c53",
  "prevHash": "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm",
  "newBlockRange": "000000000-000350277",
  "newPins": [
    {
      "fileName": "000000000-000000000",
      "bloomHash": "QmPQEgUm7nzQuW9HYyWp5Ff3aoUwg2rsxDngyuyddJTvrv",
      "indexHash": "QmZ5Atm8Z7aFLz2EycK4pVMHuH4y3PDGspuFejnE9fx2i5"
    },
    {
      "fileName": "000000001-000350277",
      "bloomHash": "QmZgrWAJLidkHJRLDVoZGCWAgmmcQEDCDM65XL5ZbAXxCM",
      "indexHash": "QmP1KvDPUJ1MqsCYcicJgTf5sxN7WjT7dZsrfBk2Jg3mSe"
    }
   ],
  "prevBlockRange": "",
  "prevPins": []
}
`

var manifestTabSource = `
013326058-013328910	QmeyKQKiEdUpcPaEUTP6vsT3H4QdAuzWhd6gpTa3mrMotF	QmTDFSbcPbtQXtPRs8YoVvqvCHZQaTkP1dZKSAXKW5CWt1
013323255-013326057	QmYBqaeLTq1JYEsGkWtZmNh2ZaaC9cjKJqe9pmuQtDhbcc	Qmdnn9mUEZSBps8K8kkAEJ7HiM4qFwRppyLpkJTddce2MU
013320355-013323254	QmYLcU9qUpLhETffC4X2MjtKkFZRDn85XXBgGTMT2sWhv1	QmZqZ2ZUZ3s8tHBm34dGWyXpr5ShZgkHpTKg1n79NpehSJ
013317545-013320354	QmRxGboQht2ST1NMwkWdytsF1nL5FVrgfEhZ4ozBU1UZQU	QmQP9oKDZrqBDXMXCJS5UjWhtp8viDuAJqy3t2ERgFs6X7
013314548-013317544	QmXFzVi2TE2iR2ALeJVfGe7ZFgeDgjW3UeHiAussgFvQoX	QmZbT4zLcp8WTM6MH33D1nVP4eRCDwGTk2L37rud2c1bgK
013311678-013314547	QmaBmWyq4KdiuMrrErxgniduejueqQi497DzVfvzdR3Ft8	QmP72KErK2Z6rbpQ8tHjTtTqUrMEEimaBtk9vzxunKErJc
013308630-013311677	QmYHyegTZYecms9P9gCuNUp1vBZ7XHQohswt8iqmdmNhCe	QmP81iaNY88aHYaFrJPST1cT8gD76q3ZtnCHBk1YJA4dM6
`

func TestDownloadJSON(t *testing.T) {
	ts := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		fmt.Fprintln(w, manifestJSONSource)
	}))

	defer ts.Close()

	manifest, err := DownloadManifest(ts.URL)
	if err != nil {
		t.Error(err)
	}

	if l := len(manifest.NewPins); l != 2 {
		t.Errorf("Wrong NewPins length: %d", l)
	}
}

func TestDownloadTab(t *testing.T) {
	ts := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/tab-separated-values")
		fmt.Fprintln(w, manifestTabSource)
	}))

	defer ts.Close()

	manifest, err := DownloadManifest(ts.URL)
	if err != nil {
		t.Error(err)
	}

	if l := len(manifest.NewPins); l != 7 {
		t.Errorf("Wrong NewPins length: %d", l)
	}
}
