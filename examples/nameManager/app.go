package main

import (
	"io"
	"log/slog"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
)

// App is the main structure carrying the command line options, loggers, and application state
type App struct {
	logger   *slog.Logger
	database names.DatabaseType
	action   Action
	dryrun   bool
}

// NewApp returns a new application ready to be used.
func NewApp() *App {
	logger.SetLoggerWriter(io.Discard) // we never want core to log anything
	logLevel := slog.LevelInfo
	if ll, ok := os.LookupEnv("TB_LOGLEVEL"); ok {
		switch strings.ToLower(ll) {
		case "debug":
			logLevel = slog.LevelDebug
		case "info":
			logLevel = slog.LevelInfo
		case "warn":
			logLevel = slog.LevelWarn
		case "error":
			logLevel = slog.LevelError
		}
	}
	opts := slog.HandlerOptions{
		Level: logLevel,
		ReplaceAttr: func(groups []string, a slog.Attr) slog.Attr {
			if a.Key == slog.TimeKey {
				a.Value = slog.StringValue(a.Value.Time().Format("15:04:05"))
			}
			return a
		},
	}

	app := App{
		logger:   slog.New(slog.NewTextHandler(os.Stderr, &opts)),
		database: names.DatabaseCustom,
	}

	if os.Getenv("TB_NAMEMANAGER_REGULAR") == "true" {
		app.database = names.DatabaseRegular
	}
	if os.Getenv("TB_NAMEMANAGER_DRYRUN") == "true" {
		app.dryrun = true
	}

	return &app
}

func (a *App) IsRegular() bool {
	return a.database == names.DatabaseRegular
}
