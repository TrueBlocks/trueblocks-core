/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */

import * as ApiCallers from '../lib/api_callers';
import { address, blknum, ChunkAddress, ChunkAppearance, ChunkBloom, ChunkIndex, ChunkManifest, ChunkPin, ChunkStats, float64, Message, uint64 } from '../types';

export function getChunks(
  parameters?: {
    mode: 'manifest' | 'index' | 'blooms' | 'pins' | 'addresses' | 'appearances' | 'stats',
    blocks?: blknum[],
    check?: boolean,
    pin?: boolean,
    publish?: boolean,
    remote?: boolean,
    belongs?: address[],
    firstBlock?: blknum,
    lastBlock?: blknum,
    maxAddrs?: uint64,
    deep?: boolean,
    rewrite?: boolean,
    count?: boolean,
    sleep?: float64,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<ChunkAddress[] | ChunkAppearance[] | ChunkBloom[] | ChunkIndex[] | ChunkManifest[] | ChunkPin[] | ChunkStats[] | Message[]>(
    { endpoint: '/chunks', method: 'get', parameters, options },
  );
}
