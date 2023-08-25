import typing_extensions

from openapi_client.apis.tags import TagValues
from openapi_client.apis.tags.accounts_api import AccountsApi
from openapi_client.apis.tags.chain_data_api import ChainDataApi
from openapi_client.apis.tags.chain_state_api import ChainStateApi
from openapi_client.apis.tags.admin_api import AdminApi
from openapi_client.apis.tags.other_api import OtherApi

TagToApi = typing_extensions.TypedDict(
    'TagToApi',
    {
        TagValues.ACCOUNTS: AccountsApi,
        TagValues.CHAIN_DATA: ChainDataApi,
        TagValues.CHAIN_STATE: ChainStateApi,
        TagValues.ADMIN: AdminApi,
        TagValues.OTHER: OtherApi,
    }
)

tag_to_api = TagToApi(
    {
        TagValues.ACCOUNTS: AccountsApi,
        TagValues.CHAIN_DATA: ChainDataApi,
        TagValues.CHAIN_STATE: ChainStateApi,
        TagValues.ADMIN: AdminApi,
        TagValues.OTHER: OtherApi,
    }
)
