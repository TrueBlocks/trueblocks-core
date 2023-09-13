/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, blknum, ChunkAddress, ChunkBloom, ChunkIndex, ChunkPinReport, ChunkRecord, ChunkStats, double, Manifest, ReportCheck } from '../types';

export function getChunks(
  parameters?: {
    mode: 'manifest' | 'index' | 'blooms' | 'addresses' | 'appearances' | 'stats',
    blocks?: blknum[],
    check?: boolean,
    pin?: boolean,
    publish?: boolean,
    publisher?: address,
    remote?: boolean,
    belongs?: address[],
    firstBlock?: blknum,
    lastBlock?: blknum,
    maxAddrs?: blknum,
    deep?: boolean,
    sleep?: double,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    ether?: boolean,
    raw?: boolean,
    cache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | ChunkAddress[] | ChunkBloom[] | ChunkIndex[] | ChunkPinReport[] | ChunkRecord[] | ChunkStats[] | Manifest[] | ReportCheck[]>(
    { endpoint: '/chunks', method: 'get', parameters, options },
  );
}
