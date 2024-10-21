package main

import (
	"fmt"
	"sort"
	"strings"
	"sync"
)

// DiskUsage represents the disk usage information for a single partition
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

// getDiscUsage gathers disk usage information from multiple machines concurrently
// It returns a slice of DiskUsage structs and an error if any occurred during the process
func getDiscUsage() ([]DiskUsage, error) {
	config, err := loadConfig()
	if err != nil {
		return nil, fmt.Errorf("error loading configuration: %w", err)
	}

	var mu sync.Mutex
	var errors []error
	var wg sync.WaitGroup
	resultChan := make(chan []DiskUsage, len(config.Machines))
	for _, machine := range config.Machines {
		wg.Add(1)
		go func(mach string) {
			defer wg.Done()
			data, err := gatherDiskUsage(mach)
			if err != nil {
				mu.Lock()
				errors = append(errors, fmt.Errorf("error gathering data from %s: %w", mach, err))
				mu.Unlock()
				return
			}
			resultChan <- processMachineData(mach, data, config.Aliases[mach])
		}(machine)
	}

	go func() {
		wg.Wait()
		close(resultChan)
	}()

	var allUsages []DiskUsage
	for result := range resultChan {
		allUsages = append(allUsages, result...)
	}

	sort.SliceStable(allUsages, func(i, j int) bool {
		if allUsages[i].Alias == allUsages[j].Alias {
			return allUsages[i].SizeValue < allUsages[j].SizeValue
		}
		return allUsages[i].Alias < allUsages[j].Alias
	})

	if len(errors) > 0 {
		return allUsages, fmt.Errorf("encountered %d errors while gathering disk usage: %v", len(errors), errors)
	}

	return allUsages, nil
}

// processMachineData parses the raw disk usage data for a single machine
// It returns a slice of DiskUsage structs for the given machine
func processMachineData(machine, data, alias string) []DiskUsage {
	var usages []DiskUsage
	uniqueRecords := make(map[string]DiskUsage)

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

	for _, usage := range uniqueRecords {
		usages = append(usages, usage)
	}

	return usages
}

type Config struct {
	Machines []string
	Aliases  map[string]string
}

// loadConfig returns a map of machine names to their aliases
func loadConfig() (Config, error) {
	var config Config
	config.Machines = []string{
		"wildmolasses.local",
		"linux.local",
		"laptop.local",
		"desktop.local",
		"unchainedindex.io",
		"dalledress.io",
	}
	config.Aliases = map[string]string{
		"laptop.local":       "1.0-Laptop",
		"desktop.local":      "2.0-Desktop",
		"wildmolasses.local": "4.0-WildMolasses",
		"linux.local":        "3.0-Linux",
		"unchainedindex.io":  "5.0-UnchainedIndex",
		"dalledress.io":      "6.0-Testing/DalleDress",
	}
	return config, nil
}
