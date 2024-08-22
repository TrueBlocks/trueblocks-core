package crud

import (
	"errors"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type crudValue interface {
	base.Address | string
}

type Field[CV crudValue] struct {
	Value   CV
	Updated bool
}

type NameCrud struct {
	Address  Field[base.Address]
	Name     Field[string]
	Tags     Field[string]
	Source   Field[string]
	Symbol   Field[string]
	Decimals Field[string]
}

type NameOperation string

const (
	Create   NameOperation = "create"
	Update   NameOperation = "update"
	Delete   NameOperation = "delete"
	Undelete NameOperation = "undelete"
	Remove   NameOperation = "remove"
	Autoname NameOperation = "autoname"
)

func (cd *NameCrud) Validate(requireName bool) error {
	if cd.Address.Value.IsZero() {
		return errors.New("address is required in crud.Validate")
	}
	if requireName && cd.Name.Value == "" {
		return errors.New("address is required in crud.Validate")
	}
	return nil
}

func NewNameCrud(requireName bool, r *http.Request) (*NameCrud, error) {
	cd := &NameCrud{}
	if r != nil {
		if err := r.ParseForm(); err != nil {
			return nil, err
		}

		if len(r.PostForm) == 0 {
			return nil, errors.New("empty form")
		}

		cd = &NameCrud{
			Address: Field[base.Address]{
				Value:   base.HexToAddress(r.PostForm.Get("address")),
				Updated: r.PostForm.Has("address"),
			},
			Name: Field[string]{
				Value:   r.PostForm.Get("name"),
				Updated: r.PostForm.Has("name"),
			},
			Tags: Field[string]{
				Value:   r.PostForm.Get("tags"),
				Updated: r.PostForm.Has("tags"),
			},
			Source: Field[string]{
				Value:   r.PostForm.Get("source"),
				Updated: r.PostForm.Has("source"),
			},
			Symbol: Field[string]{
				Value:   r.PostForm.Get("symbol"),
				Updated: r.PostForm.Has("symbol"),
			},
			Decimals: Field[string]{
				Value:   r.PostForm.Get("decimals"),
				Updated: r.PostForm.Has("decimals"),
			},
		}
	} else {
		address, addressUpdated := os.LookupEnv("TB_NAME_ADDRESS")
		cd.Address = Field[base.Address]{
			Value:   base.HexToAddress(address),
			Updated: addressUpdated,
		}
		name, nameUpdated := os.LookupEnv("TB_NAME_NAME")
		cd.Name = Field[string]{
			Value:   name,
			Updated: nameUpdated,
		}
		tags, tagsUpdated := os.LookupEnv("TB_NAME_TAGS")
		cd.Tags = Field[string]{
			Value:   tags,
			Updated: tagsUpdated,
		}
		source, sourceUpdated := os.LookupEnv("TB_NAME_SOURCE")
		cd.Source = Field[string]{
			Value:   source,
			Updated: sourceUpdated,
		}
		symbol, symbolUpdated := os.LookupEnv("TB_NAME_SYMBOL")
		cd.Symbol = Field[string]{
			Value:   symbol,
			Updated: symbolUpdated,
		}
		decimals, decimalsUpdated := os.LookupEnv("TB_NAME_DECIMALS")
		cd.Decimals = Field[string]{
			Value:   decimals,
			Updated: decimalsUpdated,
		}
	}

	if err := cd.Validate(requireName); err != nil {
		return nil, err
	}

	return cd, nil
}

func (cd *NameCrud) SetEnv() {
	os.Setenv("TB_NAME_ADDRESS", cd.Address.Value.String())
	os.Setenv("TB_NAME_NAME", cd.Name.Value)
	os.Setenv("TB_NAME_TAGS", cd.Tags.Value)
	os.Setenv("TB_NAME_SOURCE", cd.Source.Value)
	os.Setenv("TB_NAME_SYMBOL", cd.Symbol.Value)
	os.Setenv("TB_NAME_DECIMALS", cd.Decimals.Value)
}

func (cd *NameCrud) Unsetenv() {
	os.Unsetenv("TB_NAME_ADDRESS")
	os.Unsetenv("TB_NAME_NAME")
	os.Unsetenv("TB_NAME_TAGS")
	os.Unsetenv("TB_NAME_SOURCE")
	os.Unsetenv("TB_NAME_SYMBOL")
	os.Unsetenv("TB_NAME_DECIMALS")
}
