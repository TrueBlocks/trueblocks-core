package main

func main() {
	// Tests stream results from Export command. Includes time out cancel
	TestStreamExport()
	// Streams names database
	TestStreamNames()
	// Shows how to use a progress bar with streaming SDK
	TestProgressReporting()
}
