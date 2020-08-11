library(ggplot2)
library(dplyr)
library(tidyverse)

setwd('/Users/jrush/Development/trueblocks-core/src/other/issuance/working');

read_csv("results2.csv", col_names = TRUE, col_types = cols(blockNumber = "i")) -> df
head(df)
df <- df %>% mutate(bin = floor(blockNumber / 10000) * 10000)
df

ggplot(df, aes(x=bin)) + geom_histogram()

# Change the width of bins
ggplot(df, aes(x=bin)) + 
  geom_histogram(binwidth=10000)

# Change colors
p<-ggplot(df, aes(y=uncleReward)) + 
  geom_histogram(color="blue", fill="white", binwidth=50000)
p <- p + ggtitle('Number of Uncles Produced Per 10,000 Blocks Over Time') + xlab('Week') + ylab('Uncles per 10,000 blocks') + xlab('blockNumber')
p
