/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { CacheItem, Chain } from '.';

export type Status = {
  cachePath?: string
  caches: CacheItem[]
  chain?: string
  chainConfig?: string
  chainId?: string
  chains: Chain[]
  clientVersion?: string
  hasEsKey?: boolean
  hasPinKey?: boolean
  indexPath?: string
  isApi?: boolean
  isArchive?: boolean
  isScraping?: boolean
  isTesting?: boolean
  isTracing?: boolean
  networkId?: string
  progress?: string
  rootConfig?: string
  rpcProvider?: string
  version?: string
}
