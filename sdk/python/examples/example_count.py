from __future__ import print_function
import openapi_client.apis.tag_to_api
from openapi_client import ApiException

addrs = ["0x007f7f58d3eb5b7510a301ecc749fc1fcddbe14d", "0x2910543af39aba0cd09dbb2d50200b3e800a63d2"]

api_instance = openapi_client.apis.tag_to_api.AccountsApi()

try:
    api_response = api_instance.accounts_list({
        "addrs": addrs,
        "count": 1
    })
except ApiException as e:
    print("Exception when calling API: %s\n" % e)

more_txs = dict(address="", count=0)

for app in api_response.body.get("data"):
    if more_txs["count"] > app["nRecords"]:
        continue
    more_txs["address"] = app["address"]
    more_txs["count"] = app["nRecords"]

print("Account {address} has more transactions ({count})".format(**more_txs))