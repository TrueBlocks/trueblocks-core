package logger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/colors"
)

func toColored(s severity, color string, v ...any) {
	if firstElement, ok := v[0].(string); ok {
		v[0] = color + firstElement
	} else {
		v = append([]interface{}{color}, v...)
	}
	v = append(v, colors.Off)
	toLog(s, v...)
}

func InfoR(v ...any) {
	toColored(info, colors.Red, v...)
}

func InfoG(v ...any) {
	toColored(info, colors.Green, v...)
}

func InfoY(v ...any) {
	toColored(info, colors.Yellow, v...)
}

func InfoB(v ...any) {
	toColored(info, colors.Blue, v...)
}

func InfoM(v ...any) {
	toColored(info, colors.Magenta, v...)
}

func InfoC(v ...any) {
	toColored(info, colors.Cyan, v...)
}

func InfoW(v ...any) {
	toColored(info, colors.White, v...)
}

func InfoBR(v ...any) {
	toColored(info, colors.BrightRed, v...)
}

func InfoBG(v ...any) {
	toColored(info, colors.BrightGreen, v...)
}

func InfoBY(v ...any) {
	toColored(info, colors.BrightYellow, v...)
}

func InfoBB(v ...any) {
	toColored(info, colors.BrightBlue, v...)
}

func InfoBM(v ...any) {
	toColored(info, colors.BrightMagenta, v...)
}

func InfoBC(v ...any) {
	toColored(info, colors.BrightCyan, v...)
}

func InfoBW(v ...any) {
	toColored(info, colors.BrightWhite, v...)
}
