from __future__ import print_function
import openapi_client.apis.tag_to_api

addrs = ["0xf503017d7baf7fbc0fff7492b751025c6a78179b"]

accounts_api = openapi_client.apis.tag_to_api.AccountsApi()
state_api = openapi_client.apis.tag_to_api.ChainStateApi()

names_response = accounts_api.accounts_names({
    "terms": "Dai Stablecoin"
})

dai_address = ""

for entry in names_response.body.get("data"):
    if entry["name"] == "Dai Stablecoin":
        dai_address = entry["address"]

if not dai_address:
    raise ValueError("DAI address not found")

tokens_response = state_api.chainstate_tokens({
    "addrs": [dai_address] + addrs,
})

response = tokens_response.body.get("data")
if not response:
    if tokens_response.body.get("error"):
        raise ValueError(tokens_response.body.get("error"))
    raise ValueError("Expected data")

for balance in response:
    print("Account {0} holds {1} DAI".format(balance["holder"], balance["balance"]))
