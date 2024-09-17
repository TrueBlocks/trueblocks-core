package main

import (
	"fmt"
	"strings"
	"time"
)

func getTitle() string {
	fmtStr := `<html><head></head><body><center><table width="50%"><tr><td><h1>Disk Usage Report</h1>

Report generated on %s
<p><p>

`
	return fmt.Sprintf(fmtStr, time.Now().Format(time.RFC1123))
}

func headerRow() string {
	return `  <table width="100%">
  <tr>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Machine</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>System</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Partition</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Size</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Used</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Available</th>
    <th style="border-bottom: 1px solid black" valign="bottom" nowrap>Pct</th>
  </tr>
`
}

// cnt := 0

func dataRow(usage *DiskUsage, lastMachine string) (string, string) {
	var ret string

	machineName := "&nbsp;"
	systemName := "&nbsp;"
	if usage.Alias != lastMachine {
		parts := strings.Split(usage.Alias, "-")
		machineName = fmt.Sprintf("<b>%s</b>", parts[1])
		systemName = usage.Machine12
		if lastMachine != "" {
			ret += blankRow()
		}
		lastMachine = usage.Alias
	}

	usageValue := parsePercentage(usage.Usage)
	usageColor := ""
	if usageValue < 50 {
		usageColor = "background-color: #ccffcc;" // Light green
	} else if usageValue > 85 {
		usageColor = "background-color: #ffcccc;" // Light red (pink)
	} else {
		usageColor = "background-color: #ffffcc;" // Pale yellow
	}

	fmtStr := `  <tr>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top'>%s</td>
    <td nowrap valign='top' style='%s'>%s</td>
  </tr>
`

	ret += fmt.Sprintf(fmtStr,
		machineName,
		systemName,
		usage.Partition,
		usage.Size, usage.Used,
		usage.Available,
		usageColor,
		usage.Usage)

	return ret, lastMachine
}

func blankRow() string {
	return `  <tr style='background-color: #f0f0f0; height: 0.5em; line-height: 0.5em;'>
    <td valign='top' colspan='7' style='padding: 0;'>&nbsp;</td>
  </tr>
`
}

func legendRow() string {
	return `  </table>
  <div>
    <span style='display: inline-block; width: 15px; height: 15px; background-color: #ccffcc; border: 1px solid #99cc99;'></span> <span>50% disc usage or less</span>
    <span style='display: inline-block; width: 15px; height: 15px; background-color: #ffffcc; border: 1px solid #cccc99; margin-left: 20px;'></span> <span>okay</span>
    <span style='display: inline-block; width: 15px; height: 15px; background-color: #ffcccc; border: 1px solid #cc9999; margin-left: 20px;'></span> <span>85% disc usage or more</span>
  </div></td></tr></table></center></body></html>
`
}
