package main

import (
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type helper struct {
	path  string
	order int
	on    bool
}

var routeMap = map[string]helper{
	"slurp":        {"tools", 0, os.Getenv("TEST_SLURP") == "true"},
	"names":        {"tools", 1, true},
	"blocks":       {"tools", 2, true},
	"logs":         {"tools", 3, true},
	"receipts":     {"tools", 4, true},
	"state":        {"tools", 5, true},
	"tokens":       {"tools", 6, true},
	"traces":       {"tools", 7, true},
	"transactions": {"tools", 8, true},
	"abis":         {"tools", 9, true},
	"when":         {"tools", 10, true},
	"list":         {"apps", 11, true},
	"monitors":     {"apps", 12, true},
	"export":       {"apps", 13, true},
	"scrape":       {"apps", 14, true},
	"status":       {"apps", 15, true},
	"chifra":       {"apps", 16, true},
	"chunks":       {"apps", 17, true},
	"config":       {"apps", 18, true},
	"daemon":       {"apps", 19, true},
	"explore":      {"apps", 20, true},
	"init":         {"apps", 21, true},
}

var modeMap = map[string]helper{
	"sdk": {"mode", 1, true},
	"api": {"mode", 2, true},
	"cmd": {"mode", 3, true},
}

func isValidEnvironment() error {
	if os.Getenv("TEST_MODE") != "true" {
		return fmt.Errorf("you must run this program with TEST_MODE=true")
	}

	filter := os.Getenv("TB_TEST_FILTER")
	if len(filter) == 0 && len(os.Args) > 1 {
		filter = os.Args[1]
		os.Args = []string{os.Args[0]}
	}
	if len(filter) == 0 {
		return nil
	}
	if !strings.Contains(filter, ":") {
		filter += ":"
	}
	parts := strings.Split(filter, ":")
	if len(parts) != 2 {
		return fmt.Errorf("invalid filter (%s). Need <routeList>:<modeList>", filter)
	}

	return nil
}

func getRoutesAndModes() ([]string, []string) {
	filter := os.Getenv("TB_TEST_FILTER")
	if len(filter) == 0 && len(os.Args) > 1 {
		filter = os.Args[1]
		os.Args = []string{os.Args[0]}
	}
	if len(filter) > 0 {
		if !strings.Contains(filter, ":") {
			filter += ":"
		}
		parts := strings.Split(filter, ":")
		if len(parts) != 2 {
			logger.Error("filter must be in the form of 'route:mode'")
			os.Exit(1)
		}

		userRoutes, userModes := parts[0], parts[1]
		if len(userRoutes) > 0 {
			for key, value := range routeMap {
				routeMap[key] = helper{value.path, value.order, false}
			}
			nOn := 0
			routes := strings.Split(userRoutes, ",")
			for _, route := range routes {
				if route == "tools" || route == "apps" {
					for key, value := range routeMap {
						if value.path == route {
							on := true
							if key == "slurp" {
								on = os.Getenv("TEST_SLURP") == "true"
							}
							if key == "status" {
								on = false
								if strings.Contains(filter, "status") {
									fmt.Println("Tests for the status route may not be run separately.")
									os.Exit(1)
								}
							}
							routeMap[key] = helper{routeMap[key].path, routeMap[key].order, on}
						}
					}
				} else {
					if len(routeMap[route].path) > 0 {
						routeMap[route] = helper{routeMap[route].path, nOn, true}
						nOn++
					}
				}
			}
		}

		if len(userModes) > 0 {
			for key, value := range modeMap {
				modeMap[key] = helper{value.path, value.order, false}
			}
			nOn := 0
			modes := strings.Split(userModes, ",")
			for _, mode := range modes {
				if len(modeMap[mode].path) > 0 {
					modeMap[mode] = helper{modeMap[mode].path, nOn, true}
					nOn++
				}
			}
		}
	}

	// sort the routes and prepare for return...
	sorted := []helper{}
	for key, value := range routeMap {
		if value.on {
			sorted = append(sorted, helper{key, value.order, true})
		}
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].order < sorted[j].order
	})
	order := []string{}
	for _, item := range sorted {
		order = append(order, item.path)
	}

	// sort the modes and prepare for return...
	sorted = []helper{}
	for key, value := range modeMap {
		if value.on {
			sorted = append(sorted, helper{key, value.order, true})
		}
	}
	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].order < sorted[j].order
	})
	modes := []string{}
	for _, item := range sorted {
		modes = append(modes, item.path)
	}

	if len(order) == 0 || len(modes) == 0 {
		logger.Error("No routes or modes are enabled")
		os.Exit(1)
	}

	if len(filter) > 0 {
		fmt.Println("Order:", order)
		fmt.Println("Modes:", modes)
	}

	return order, modes
}
