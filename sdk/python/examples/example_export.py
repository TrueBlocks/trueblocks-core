from __future__ import print_function
import openapi_client.apis.tag_to_api
from openapi_client import ApiException

api_instance = openapi_client.apis.tag_to_api.AccountsApi()

try:
    api_response = api_instance.accounts_export({
        "addrs": "0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
        "logs": 1,
        "firstBlock": 1429283,
        "lastBlock": 1429468,
        "articulate": 1,
        "maxRecords": 3,
        "cache": 1,
    })
except ApiException as e:
    print("Exception when calling API: %s\n" % e)

more_txs = dict(address="", count=0)

response = api_response.body.get("data")
if not response:
    print("Expected data")
    exit(1)

for tx in response:
    if not tx["articulatedLog"]:
        continue
    print("Executed `{0}` of contract deployed to {1}.\nInputs:".format(tx["articulatedLog"]["name"], tx["address"]))
    for key, value in tx["articulatedLog"]["inputs"].items():
        print("\t{0}: {1}".format(key, value))
