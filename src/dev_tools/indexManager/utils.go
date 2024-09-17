package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"strconv"
	"strings"
)

// copyToRemote copies a file to a remote server using SCP
func copyToRemote(localFile, remoteUser, remoteHost, remotePath string) (string, error) {
	scpCmd := fmt.Sprintf("scp -p %s %s@%s:%s", localFile, remoteUser, remoteHost, remotePath)
	cmd := exec.Command("sh", "-c", scpCmd)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Run()
	msg := ""
	if err == nil {
		msg = fmt.Sprintf("File copied to %s@%s:%s\n", remoteUser, remoteHost, remotePath)
	}
	return msg, err
}

// gatherDiskUsage connects to the given machine via SSH and runs the df -h | grep -v snap command
func gatherDiskUsage(machine string) (string, error) {
	cmd := exec.Command("ssh", machine, "df -h | grep -v snap")
	output, err := cmd.CombinedOutput()
	return string(output), err
}

// parseSize converts a size string (e.g., "7.3Ti", "500Mi") to an integer value in kilobytes for sorting
func parseSize(sizeStr string) int64 {
	multiplier := int64(1)
	switch {
	case strings.HasSuffix(sizeStr, "Ti"):
		multiplier = 1024 * 1024 * 1024
		sizeStr = strings.TrimSuffix(sizeStr, "Ti")
	case strings.HasSuffix(sizeStr, "Gi"):
		multiplier = 1024 * 1024
		sizeStr = strings.TrimSuffix(sizeStr, "Gi")
	case strings.HasSuffix(sizeStr, "Mi"):
		multiplier = 1024
		sizeStr = strings.TrimSuffix(sizeStr, "Mi")
	case strings.HasSuffix(sizeStr, "Ki"):
		sizeStr = strings.TrimSuffix(sizeStr, "Ki")
	case strings.HasSuffix(sizeStr, "T"):
		multiplier = 1024 * 1024 * 1024
		sizeStr = strings.TrimSuffix(sizeStr, "T")
	case strings.HasSuffix(sizeStr, "G"):
		multiplier = 1024 * 1024
		sizeStr = strings.TrimSuffix(sizeStr, "G")
	case strings.HasSuffix(sizeStr, "M"):
		multiplier = 1024
		sizeStr = strings.TrimSuffix(sizeStr, "M")
	case strings.HasSuffix(sizeStr, "K"):
		sizeStr = strings.TrimSuffix(sizeStr, "K")
	}
	value, err := strconv.ParseFloat(sizeStr, 64)
	if err != nil {
		log.Printf("Error parsing size: %v", err)
		return 0
	}
	return int64(value * float64(multiplier))
}

// parsePercentage converts a percentage string (e.g., "39%") to an integer value for sorting
func parsePercentage(pctStr string) int64 {
	// Check if the string ends with '%'
	if !strings.HasSuffix(pctStr, "%") {
		return 0
	}

	// Remove the '%' suffix and parse the number
	pctStr = strings.TrimSuffix(pctStr, "%")
	value, err := strconv.ParseInt(pctStr, 10, 64)
	if err != nil {
		return 0
	}
	return value
}
