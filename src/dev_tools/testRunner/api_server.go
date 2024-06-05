package main

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func getApiUrl() string {
	port := strings.ReplaceAll(os.Getenv("TB_TEST_API_SERVER"), ":", "")
	if port == "" {
		port = "8080"
	}
	return "localhost:" + port
}

func startApiServer() error {
	ready := make(chan bool)
	go sdk.NewDaemon(getApiUrl()).Start(ready)
	<-ready
	logger.Info(colors.Yellow + "Server started..." + colors.Off)
	return nil
}

// func getFreePort() (int, error) {
// 	addr, err := net.ResolveTCPAddr("tcp", "localhost:0")
// 	if err != nil {
// 		return 0, err
// 	}

// 	l, err := net.ListenTCP("tcp", addr)
// 	if err != nil {
// 		return 0, err
// 	}
// 	defer l.Close()
// 	return l.Addr().(*net.TCPAddr).Port, nil
// }

// func startServer(port int) {
// 	server := &http.Server{Addr: fmt.Sprintf(":%d", port), Handler: nil} // Example: No specific handler

// 	listener, err := net.Listen("tcp", server.Addr)
// 	if err != nil {
// 		log.Fatalf("Failed to open port %d: %v", port, err)
// 	}

// 	fmt.Printf("Server starting on port %d\n", port)
// 	if err := server.Serve(listener); err != nil {
// 		log.Fatalf("Failed to start server: %v", err)
// 	}
// }
