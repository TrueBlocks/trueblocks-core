#!/usr/bin/env bash

## Runs from `trueblocks-core/docs/readmes`
## As we get better we can make this more robust, or rewrite it as something that makes more sense

echo "Freshening README files from trueblocks-core"

SOURCE=$HOME/Development/trueblocks-core/docs
DATA_MODEL=$SOURCE/content/data-model
API=$SOURCE/content/api
READMES=$SOURCE/content/readmes
INTROS=$READMES/intros
CONTENT=content/docs/chifra
#DATE=`date "+%FT%T"`
DATE="2021-05-08T01:35:20"

#----------------------------------
ACCOUNTS=$CONTENT/accounts.md
cat << _EOF_ > $ACCOUNTS &&
---
title: "Accounts"
description: ""
lead: ""
date: $DATE
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1100
toc: true
---
_EOF_

echo "    Building accounts page..."
cat $INTROS/accounts.md >> $ACCOUNTS && \
cat $READMES/apps/list.md >> $ACCOUNTS && \
cat $READMES/apps/acctExport/README.md >> $ACCOUNTS && \
cat $READMES/apps/monitors.md >> $ACCOUNTS && \
cat $READMES/tools/ethNames/README.md >> $ACCOUNTS && \
cat $READMES/tools/grabABI/README.md >> $ACCOUNTS

#----------------------------------
DATA=$CONTENT/chaindata.md
cat << _EOF_ > $DATA &&
---
title: "Chain Data"
description: ""
lead: ""
date: $DATE
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1200
toc: true
---
_EOF_

echo "    Building chain data page..."
cat $INTROS/chaindata.md >> $DATA && \
cat $READMES/tools/getBlocks/README.md >> $DATA && \
cat $READMES/tools/getTrans/README.md >> $DATA && \
cat $READMES/tools/getReceipts/README.md >> $DATA && \
cat $READMES/tools/getLogs/README.md >> $DATA && \
cat $READMES/tools/getTraces/README.md >> $DATA && \
cat $READMES/tools/whenBlock/README.md >> $DATA

#----------------------------------
STATE=$CONTENT/chainstate.md
cat << _EOF_ > $STATE &&
---
title: "Chain State"
description: ""
lead: ""
date: $DATE
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1300
toc: true
---
_EOF_

echo "    Building chain state page..."
cat $INTROS/chainstate.md >> $STATE && \
cat $READMES/tools/getState/README.md >> $STATE && \
cat $READMES/tools/getTokens/README.md >> $STATE

#----------------------------------
ADMIN=$CONTENT/admin.md
cat << _EOF_ > $ADMIN &&
---
title: "Admin"
description: ""
lead: ""
date: $DATE
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1400
toc: true
---
_EOF_

echo "    Building admin page..."
cat $INTROS/admin.md >> $ADMIN && \
cat $READMES/apps/cacheStatus/README.md >> $ADMIN && \
cat $READMES/apps/serve.md >> $ADMIN && \
cat $READMES/apps/blockScrape/README.md >> $ADMIN && \
cat $READMES/apps/init.md >> $ADMIN && \
cat $READMES/apps/pinMan/README.md >> $ADMIN

#----------------------------------
OTHER=$CONTENT/other.md
cat << _EOF_ > $OTHER &&
---
title: "Other"
description: ""
lead: ""
date: $DATE
lastmod:
  - :git
  - lastmod
  - date
  - publishDate
draft: false
images: []
menu:
  docs:
    parent: "chifra"
weight: 1500
toc: true
---
_EOF_

echo "    Building other page..."
cat $INTROS/other.md >> $OTHER && \
cat $READMES/tools/getQuotes/README.md >> $OTHER && \
cat $READMES/apps/fireStorm/README.md >> $OTHER && \
cat $READMES/tools/ethslurp/README.md >> $OTHER

echo "    README files updated."

echo "    Copying data model pages."
cp -pR $DATA_MODEL/* content/data-model/collections/

echo "    Copying API YAML file."
# cp -fp $API/openapi.yaml content/api/
echo "    NOT COPIED"

echo
