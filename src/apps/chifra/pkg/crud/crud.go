package crud

import (
	"errors"
	"fmt"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

type CrudField struct {
	Value   string
	Updated bool
}

type Operation string

const (
	Create   Operation = "create"
	Update   Operation = "update"
	Delete   Operation = "delete"
	Undelete Operation = "undelete"
	Remove   Operation = "remove"
	Autoname Operation = "autoname"
)

func OpFromString(op string) Operation {
	m := map[string]Operation{
		"create":   Create,
		"update":   Update,
		"delete":   Delete,
		"undelete": Undelete,
		"remove":   Remove,
		"autoname": Autoname,
	}
	return m[op]
}

type NameCrud struct {
	Address    CrudField
	Name       CrudField
	Tags       CrudField
	Source     CrudField
	Symbol     CrudField
	Decimals   CrudField
	Deleted    CrudField
	IsContract CrudField
	IsCustom   CrudField
	IsErc20    CrudField
	IsErc721   CrudField
	IsPrefund  CrudField
	Prefund    CrudField
	Parts      CrudField
}

func (cd *NameCrud) Validate(requireName bool) error {
	addr := base.HexToAddress(cd.Address.Value)
	if addr.IsZero() {
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

		cd = &NameCrud{}
		cd.Address.Value, cd.Address.Updated = r.PostForm.Get("address"), r.PostForm.Has("address")
		cd.Name.Value, cd.Name.Updated = r.PostForm.Get("name"), r.PostForm.Has("name")
		cd.Tags.Value, cd.Tags.Updated = r.PostForm.Get("tags"), r.PostForm.Has("tags")
		cd.Source.Value, cd.Source.Updated = r.PostForm.Get("source"), r.PostForm.Has("source")
		cd.Symbol.Value, cd.Symbol.Updated = r.PostForm.Get("symbol"), r.PostForm.Has("symbol")
		cd.Decimals.Value, cd.Decimals.Updated = r.PostForm.Get("decimals"), r.PostForm.Has("decimals")
		cd.Deleted.Value, cd.Deleted.Updated = r.PostForm.Get("deleted"), r.PostForm.Has("deleted")
		cd.IsContract.Value, cd.IsContract.Updated = r.PostForm.Get("iscontract"), r.PostForm.Has("iscontract")
		cd.IsErc20.Value, cd.IsErc20.Updated = r.PostForm.Get("iserc20"), r.PostForm.Has("iserc20")
		cd.IsErc721.Value, cd.IsErc721.Updated = r.PostForm.Get("iserc721"), r.PostForm.Has("iserc721")
		cd.IsPrefund.Value, cd.IsPrefund.Updated = r.PostForm.Get("isprefund"), r.PostForm.Has("isprefund")
		cd.Prefund.Value, cd.Prefund.Updated = r.PostForm.Get("prefund"), r.PostForm.Has("prefund")
		cd.Parts.Value, cd.Parts.Updated = r.PostForm.Get("parts"), r.PostForm.Has("parts")
		cd.IsCustom.Value, cd.IsCustom.Updated = "true", true
	} else {
		cd.Address.Value, cd.Address.Updated = os.LookupEnv("TB_NAME_ADDRESS")
		cd.Name.Value, cd.Name.Updated = os.LookupEnv("TB_NAME_NAME")
		cd.Tags.Value, cd.Tags.Updated = os.LookupEnv("TB_NAME_TAGS")
		cd.Source.Value, cd.Source.Updated = os.LookupEnv("TB_NAME_SOURCE")
		cd.Symbol.Value, cd.Symbol.Updated = os.LookupEnv("TB_NAME_SYMBOL")
		cd.Decimals.Value, cd.Decimals.Updated = os.LookupEnv("TB_NAME_DECIMALS")
		cd.Deleted.Value, cd.Deleted.Updated = os.LookupEnv("TB_NAME_DELETED")
		cd.IsContract.Value, cd.IsContract.Updated = os.LookupEnv("TB_NAME_ISCONTRACT")
		cd.IsErc20.Value, cd.IsErc20.Updated = os.LookupEnv("TB_NAME_ISERC20")
		cd.IsErc721.Value, cd.IsErc721.Updated = os.LookupEnv("TB_NAME_ISERC721")
		cd.IsPrefund.Value, cd.IsPrefund.Updated = os.LookupEnv("TB_NAME_ISPREFUND")
		cd.Prefund.Value, cd.Prefund.Updated = os.LookupEnv("TB_NAME_PREFUND")
		cd.Parts.Value, cd.Parts.Updated = os.LookupEnv("TB_NAME_PARTS")
		cd.IsCustom.Value, cd.IsCustom.Updated = "true", true
	}

	if err := cd.Validate(requireName); err != nil {
		return nil, err
	}

	return cd, nil
}

func CrudFromName(name types.Name) *NameCrud {
	cd := NameCrud{}
	cd.Address.Value, cd.Address.Updated = name.Address.Hex(), true
	cd.Name.Value, cd.Name.Updated = name.Name, true
	cd.Tags.Value, cd.Tags.Updated = name.Tags, true
	cd.Source.Value, cd.Source.Updated = name.Source, true
	cd.Symbol.Value, cd.Symbol.Updated = name.Symbol, true
	cd.Decimals.Value, cd.Decimals.Updated = fmt.Sprintf("%d", name.Decimals), true
	cd.Deleted.Value, cd.Deleted.Updated = fmt.Sprintf("%t", name.Deleted), true
	cd.IsContract.Value, cd.IsContract.Updated = fmt.Sprintf("%t", name.IsContract), true
	cd.IsErc20.Value, cd.IsErc20.Updated = fmt.Sprintf("%t", name.IsErc20), true
	cd.IsErc721.Value, cd.IsErc721.Updated = fmt.Sprintf("%t", name.IsErc721), true
	cd.IsPrefund.Value, cd.IsPrefund.Updated = fmt.Sprintf("%t", name.IsPrefund), true
	cd.Prefund.Value, cd.Prefund.Updated = name.Prefund.String(), true
	cd.Parts.Value, cd.Parts.Updated = "custom", true
	cd.IsCustom.Value, cd.IsCustom.Updated = "true", true
	return &cd
}

func (cd *NameCrud) SetEnv() {
	os.Setenv("TB_NAME_ADDRESS", cd.Address.Value)
	os.Setenv("TB_NAME_NAME", cd.Name.Value)
	os.Setenv("TB_NAME_TAGS", cd.Tags.Value)
	os.Setenv("TB_NAME_SOURCE", cd.Source.Value)
	os.Setenv("TB_NAME_SYMBOL", cd.Symbol.Value)
	os.Setenv("TB_NAME_DECIMALS", cd.Decimals.Value)
	os.Setenv("TB_NAME_DELETED", cd.Deleted.Value)
	os.Setenv("TB_NAME_ISCONTRACT", cd.IsContract.Value)
	os.Setenv("TB_NAME_ISCUSTOM", cd.IsCustom.Value)
	os.Setenv("TB_NAME_ISERC20", cd.IsErc20.Value)
	os.Setenv("TB_NAME_ISERC721", cd.IsErc721.Value)
	os.Setenv("TB_NAME_ISPREFUND", cd.IsPrefund.Value)
	os.Setenv("TB_NAME_PREFUND", cd.Prefund.Value)
	os.Setenv("TB_NAME_PARTS", "custom")
}

func (cd *NameCrud) Unsetenv() {
	os.Unsetenv("TB_NAME_ADDRESS")
	os.Unsetenv("TB_NAME_NAME")
	os.Unsetenv("TB_NAME_TAGS")
	os.Unsetenv("TB_NAME_SOURCE")
	os.Unsetenv("TB_NAME_SYMBOL")
	os.Unsetenv("TB_NAME_DECIMALS")
	os.Unsetenv("TB_NAME_DELETED")
	os.Unsetenv("TB_NAME_ISCONTRACT")
	os.Unsetenv("TB_NAME_ISCUSTOM")
	os.Unsetenv("TB_NAME_ISERC20")
	os.Unsetenv("TB_NAME_ISERC721")
	os.Unsetenv("TB_NAME_ISPREFUND")
	os.Unsetenv("TB_NAME_PREFUND")
	os.Unsetenv("TB_NAME_PARTS")
}
