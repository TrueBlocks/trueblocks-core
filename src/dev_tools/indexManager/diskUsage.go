package main

import (
	"fmt"
	"sort"
	"strings"
)

type DiskUsage struct {
	Alias     string
	Machine12 string
	Partition string
	Size      string
	Used      string
	Available string
	Usage     string
	SizeValue int64
	UsedValue int64
}

func getDiskUsage() []DiskUsage {
	machines := []string{"wildmolasses.local", "linux.local", "laptop.local", "desktop.local", "unchainedindex.io", "192.34.63.136"}
	aliases := map[string]string{
		"laptop.local":       "1.0-Laptop",
		"desktop.local":      "2.0-Desktop",
		"wildmolasses.local": "4.0-WildMolasses",
		"linux.local":        "3.0-Linux",
		"unchainedindex.io":  "5.0-UnchainedIndex",
		"192.34.63.136":      "6.0-Testing",
	}

	// Create a map to store unique records
	uniqueRecords := make(map[string]DiskUsage)

	// Loop through each machine and gather disk usage data
	for _, machine := range machines {
		data, err := gatherDiskUsage(machine)
		if err != nil {
			fmt.Printf("Error gathering data from %s: %v\n", machine, err)
			continue
		}

		// Parse and store the gathered data
		lines := strings.Split(strings.TrimSpace(data), "\n")
		for _, line := range lines[1:] {
			fields := strings.Fields(line)
			if len(fields) < 5 {
				continue
			}
			usageValue := parsePercentage(fields[4])

			// Skip rows with invalid or zero usage
			if usageValue == 0 {
				continue
			}

			// Only process rows with more than 5% usage
			if usageValue > 5 {
				if len(fields) >= 6 && strings.Contains(fields[0], "/dev/") {
					key := fmt.Sprintf("%s|%s", machine, fields[1])
					usedValue := parseSize(fields[2]) // Used value in kilobytes
					alias := aliases[machine]
					if existing, found := uniqueRecords[key]; found {
						// Keep the record with the highest used value
						if usedValue > existing.UsedValue {
							uniqueRecords[key] = DiskUsage{
								Alias:     alias,
								Machine12: machine,
								Partition: fields[0],
								Size:      fields[1],
								Used:      fields[2],
								Available: fields[3],
								Usage:     fields[4],
								SizeValue: parseSize(fields[1]),
								UsedValue: usedValue,
							}
						}
					} else {
						uniqueRecords[key] = DiskUsage{
							Alias:     alias,
							Machine12: machine,
							Partition: fields[0],
							Size:      fields[1],
							Used:      fields[2],
							Available: fields[3],
							Usage:     fields[4],
							SizeValue: parseSize(fields[1]),
							UsedValue: usedValue,
						}
					}
				}
			}
		}
	}

	var allUsages []DiskUsage
	for _, usage := range uniqueRecords {
		allUsages = append(allUsages, usage)
	}

	sort.SliceStable(allUsages, func(i, j int) bool {
		if allUsages[i].Alias == allUsages[j].Alias {
			return allUsages[i].SizeValue < allUsages[j].SizeValue
		}
		return allUsages[i].Alias < allUsages[j].Alias
	})

	return allUsages
}
