/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Name } from '../types';

export function getNames(
  parameters?: {
    terms: string[],
    expand?: boolean,
    matchCase?: boolean,
    all?: boolean,
    custom?: boolean,
    prefund?: boolean,
    addr?: boolean,
    tags?: boolean,
    clean?: boolean,
    regular?: boolean,
    dryRun?: boolean,
    autoname?: address,
    create?: boolean,
    update?: boolean,
    delete?: boolean,
    undelete?: boolean,
    remove?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Name[]>(
    { endpoint: '/names', method: 'get', parameters, options },
  );
}
