import { ipfshash, uint64 } from '.';

export type CacheRecordUp = {
  range: string
  bloomHash:   ipfshash
  indexHash:   ipfshash
  bloomSize:   uint64
  indexSize:   uint64
  firstApp:    uint64
  latestApp:   uint64
  filename:    string
  nApps:       uint64
  nAppearance: uint64
}
