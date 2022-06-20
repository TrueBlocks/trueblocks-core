library(lubridate)
library(readr)
library(tidyverse)
library(ggthemes)

setwd("~/Development/trueblocks-core/src/examples/dynamic/gasHole/")
df <- read_csv('by_txs.csv') %>%
  mutate(month = date(tx_date)) %>%
  mutate(month = ceiling_date(month, "month")) %>%
  mutate(txid = transactionIndex) %>%
  mutate(bn = blockNumber) %>%
  mutate(fromName = paste0(from, " (", name, ")"))

theData <- df %>% select(bn, txid, tx_date, from, usdGasCost, etherGasCost, fromName, hash, month)

byAddr <- theData %>%
  group_by(fromName) %>%
  summarise(
    ethSpent = sum(etherGasCost),
    usdSpent = sum(usdGasCost)
  )
byAddr
write.csv(byAddr,"./by_addr.csv", row.names = FALSE)

byMonthInAddr <- theData %>%
  group_by(fromName, month) %>%
  summarise(
    ethSpent = sum(etherGasCost),
    usdSpent = sum(usdGasCost)
  )
byMonthInAddr
write.csv(byMonthInAddr,"./by_month.csv", row.names = FALSE)

