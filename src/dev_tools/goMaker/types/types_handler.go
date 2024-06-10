package types

import "strings"

type Handler struct {
	Position float64 `json:"position"`
	Name     string  `json:"name"`
	Option   *Option `json:"option"`
}

func (h *Handler) Test() string {
	if h.Name == "Show" {
		return ""
	} else if h.Name == "Crud" {
		return "opts.anyCrud()"
	} else if h.Name == "Decache" {
		return "opts.Globals." + h.Name
	} else if strings.Contains(h.Option.DataType, "blknum") {
		r := "opts." + h.Name
		if h.Option.DefVal == "" {
			r += " > 0"
		} else {
			r += " != " + h.Option.DefVal
		}
		return r
	} else {
		if h.Option.IsArray() ||
			strings.Contains(h.Option.DataType, "string") ||
			strings.Contains(h.Option.DataType, "address") {
			return "len(opts." + h.Name + ") > 0"
		} else {
			return "opts." + h.Name
		}
	}
}

func (h *Handler) Handler() string {
	if h.Option.Route == "export" || h.Option.Route == "list" {
		return "err = opts.Handle" + h.Name + "(monitorArray)"
	} else if h.Option.Route == "chunks" {
		return "err = opts.Handle" + h.Name + "(blockNums)"
	}
	return "err = opts.Handle" + h.Name + "()"
}

func (h *Handler) executeTemplate(name, tmplCode string) string {
	return executeTemplate(h, "handler", name, tmplCode)
}
