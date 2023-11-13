/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, blknum, ChunkAddress, ChunkBloom, ChunkIndex, ChunkPinReport, ChunkRecord, ChunkStats, double, IpfsPin, Manifest, ReportCheck } from '../types';

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
    maxAddrs?: blknum,
    deep?: boolean,
    rewrite?: boolean,
    count?: boolean,
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
  return ApiCallers.fetch<Appearance[] | ChunkAddress[] | ChunkBloom[] | ChunkIndex[] | ChunkPinReport[] | ChunkRecord[] | ChunkStats[] | IpfsPin[] | Manifest[] | ReportCheck[]>(
    { endpoint: '/chunks', method: 'get', parameters, options },
  );
}
