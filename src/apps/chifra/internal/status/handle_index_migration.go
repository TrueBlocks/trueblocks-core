package statusPkg

// Not needed, so comments, but might be needed later, so not removed
func (opts *StatusOptions) HandleIndexMigration() error {
	return nil
	// 	fileName := config.GetPathToIndex(opts.Globals.Chain) + "finalized/000000000-000000000.bin"
	// 	ok, _ := index.HasValidIndexHeader(opts.Globals.Chain, fileName)
	// 	if ok {
	// 		log.Println(colors.Yellow, "The index does not need to be migrated.", colors.Off)
	// 		return nil
	// 	}

	// 	opts.checkTrueBlocksFile()

	// 	log.Println(colors.Yellow + "Removing temporary folders (staging, unripe, ripe, maps)" + colors.Off)
	// 	index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), true)
	// 	done()

	// 	opts.replaceOldIndex()

	// 	log.Println("To complete the migration, you must remove all monitors from the cache")
	// 	done()

	// 	return nil
	// }

	// func (opts *StatusOptions) checkTrueBlocksFile() {
	// 	log.Println(colors.Yellow + "Checking " + config.GetPathToRootConfig() + "trueBlocks.toml" + colors.Off)
	// 	lines := file.AsciiFileToLines(config.GetPathToRootConfig() + "trueBlocks.toml")
	// 	for i, line := range lines {
	// 		if !strings.HasPrefix(line, "#") {
	// 			// fmt.Fprintf(os.Stderr, "%s\r", colors.Yellow+"Checking "+line+colors.Off)
	// 			if strings.Contains(line, "pinGateway") {
	// 				log.Printf("%s    Error at line %d: %sChange 'pinGateway' to 'ipfsGateway'.", colors.Red, i+1, colors.Off)
	// 				log.Fatal()
	// 			}
	// 			if strings.Contains(line, "unchainedindex.io/ipfs/") {
	// 				isMainnet := strings.Contains(line, "ipfs.unchainedindex.io/ipfs/")
	// 				isGnosis := strings.Contains(line, "gnosis.unchainedindex.io/ipfs/")
	// 				isSepolia := strings.Contains(line, "sepolia.unchainedindex.io/ipfs/")
	// 				if !isMainnet && !isGnosis && !isSepolia {
	// 					log.Printf("%s    Error at line %d: %sUnsupported endpoint: %s", colors.Red, i+1, colors.Off, line)
	// 					log.Fatal()
	// 				}
	// 			}
	// 		}
	// 	}
	// 	done()
	// }

	// func (opts *StatusOptions) replaceOldIndex() error {
	// 	isPartial := false

	// 	files, err := filepath.Glob(filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "blooms", "0*"))
	// 	if err != nil {
	// 		panic(err)
	// 	}
	// 	for _, ff := range files {
	// 		fmt.Println("===>", ff)
	// 		err := os.Remove(ff)
	// 		if err != nil {
	// 			log.Panic(err)
	// 		}
	// 		ff = config.ToIndexPath(ff)
	// 		if file.FileExists(ff) {
	// 			fmt.Println("===>", ff)
	// 			err := os.Remove(ff)
	// 			if err != nil {
	// 				log.Panic(err)
	// 			}
	// 		} else {
	// 			isPartial = true
	// 		}
	// 	}

	// 	initOpts := initPkg.InitOptions{
	// 		Globals: opts.Globals,
	// 	}
	// 	if !isPartial {
	// 		initOpts.All = true
	// 	}
	// 	err = initOpts.HandleInit()
	// 	if err != nil {
	// 		return err
	// 	}
	// 	done()
	// return nil
}

// func done() {
// 	log.Println(colors.Green + "Done." + colors.Off)
// }
