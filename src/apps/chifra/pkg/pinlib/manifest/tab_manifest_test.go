/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package manifest

import (
	"strings"
	"testing"
)

var tabManifestSource = `
013326058-013328910	QmeyKQKiEdUpcPaEUTP6vsT3H4QdAuzWhd6gpTa3mrMotF	QmTDFSbcPbtQXtPRs8YoVvqvCHZQaTkP1dZKSAXKW5CWt1
013323255-013326057	QmYBqaeLTq1JYEsGkWtZmNh2ZaaC9cjKJqe9pmuQtDhbcc	Qmdnn9mUEZSBps8K8kkAEJ7HiM4qFwRppyLpkJTddce2MU
013320355-013323254	QmYLcU9qUpLhETffC4X2MjtKkFZRDn85XXBgGTMT2sWhv1	QmZqZ2ZUZ3s8tHBm34dGWyXpr5ShZgkHpTKg1n79NpehSJ
013317545-013320354	QmRxGboQht2ST1NMwkWdytsF1nL5FVrgfEhZ4ozBU1UZQU	QmQP9oKDZrqBDXMXCJS5UjWhtp8viDuAJqy3t2ERgFs6X7
013314548-013317544	QmXFzVi2TE2iR2ALeJVfGe7ZFgeDgjW3UeHiAussgFvQoX	QmZbT4zLcp8WTM6MH33D1nVP4eRCDwGTk2L37rud2c1bgK
013311678-013314547	QmaBmWyq4KdiuMrrErxgniduejueqQi497DzVfvzdR3Ft8	QmP72KErK2Z6rbpQ8tHjTtTqUrMEEimaBtk9vzxunKErJc
013308630-013311677	QmYHyegTZYecms9P9gCuNUp1vBZ7XHQohswt8iqmdmNhCe	QmP81iaNY88aHYaFrJPST1cT8gD76q3ZtnCHBk1YJA4dM6
`

func TestReadTabManifest(t *testing.T) {
	m, err := ReadTabManifest(strings.NewReader(tabManifestSource))
	if err != nil {
		t.Error(err)
	}

	if len(m.NewPins) != 7 {
		t.Errorf("Wrong length: %d", len(m.NewPins))
	}

	if m.NewPins[0].FileName != "013326058-013328910" {
		t.Errorf("Wrong FileName: %s", m.NewPins[0].FileName)
	}

	if m.NewPins[3].BloomHash != "QmRxGboQht2ST1NMwkWdytsF1nL5FVrgfEhZ4ozBU1UZQU" {
		t.Errorf("Wrong BloomHash: %s", m.NewPins[0].BloomHash)
	}

	if m.NewPins[6].IndexHash != "QmP81iaNY88aHYaFrJPST1cT8gD76q3ZtnCHBk1YJA4dM6" {
		t.Errorf("Wrong IndexHash: %s", m.NewPins[0].IndexHash)
	}
}
