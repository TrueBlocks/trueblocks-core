// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *TracesOptions) HandleFilter() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)
	traceFilter := types.TraceFilter{}
	_, br := traceFilter.ParseBangString(chain, opts.Filter)

	ids := make([]identifiers.Identifier, 0)
	if _, err := validate.ValidateIdentifiersWithBounds(chain, []string{fmt.Sprintf("%d-%d", br.First, br.Last+1)}, validate.ValidBlockIdWithRangeAndDate, 1, &ids); err != nil {
		return err
	}
	opts.TransactionIds = ids

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		apps, _, err := identifiers.IdsToApps(chain, opts.TransactionIds)
		if err != nil {
			errorChan <- err
			cancel()
		}

		if sliceOfMaps, cnt, err := types.AsSliceOfMaps[types.Transaction](apps, false); err != nil {
			errorChan <- err
			cancel()

		} else if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
			cancel()

		} else {
			bar := logger.NewBar(logger.BarOptions{
				Enabled: !testMode && !utils.IsTerminal(),
				Total:   int64(cnt),
			})

			for _, thisMap := range sliceOfMaps {
				for app := range thisMap {
					thisMap[app] = new(types.Transaction)
				}

				iterFunc := func(app types.Appearance, value *types.Transaction) error {
					if block, err := opts.Conn.GetBlockBodyByNumber(uint64(app.BlockNumber)); err != nil {
						errorChan <- fmt.Errorf("block at %s returned an error: %w", app.Orig(), err)
						return nil

					} else {
						for _, tx := range block.Transactions {
							if traces, err := opts.Conn.GetTracesByTransactionHash(tx.Hash.Hex(), &tx); err != nil {
								delete(thisMap, app)
								return fmt.Errorf("block at %s returned an error: %w", app.Orig(), err)

							} else if len(traces) == 0 {
								delete(thisMap, app)
								return fmt.Errorf("block at %s has no traces", app.Orig())

							} else {
								tr := make([]types.Trace, 0, len(traces))
								for index := range traces {
									if opts.Articulate {
										if err = abiCache.ArticulateTrace(&traces[index]); err != nil {
											errorChan <- err // continue even with an error
										}
									}
									traces[index].TraceIndex = base.TraceId(index)
									tr = append(tr, traces[index])
								}
								value.Traces = append(value.Traces, tr...)
							}
						}
						bar.Tick()
						return nil
					}
				}

				iterErrorChan := make(chan error)
				iterCtx, iterCancel := context.WithCancel(context.Background())
				defer iterCancel()
				go utils.IterateOverMap(iterCtx, iterErrorChan, thisMap, iterFunc)
				for err := range iterErrorChan {
					if !testMode || nErrors == 0 {
						errorChan <- err
						nErrors++
					}
				}

				items := make([]types.Trace, 0, len(thisMap))
				for _, tx := range thisMap {
					items = append(items, tx.Traces...)
				}
				sort.Slice(items, func(i, j int) bool {
					if items[i].BlockNumber == items[j].BlockNumber {
						if items[i].TransactionIndex == items[j].TransactionIndex {
							return items[i].TraceIndex < items[j].TraceIndex
						}
						return items[i].TransactionIndex < items[j].TransactionIndex
					}
					return items[i].BlockNumber < items[j].BlockNumber
				})

				nPassed := uint64(0)
				nShown := uint64(0)
				for nTested, item := range items {
					ok, _ := traceFilter.PassesBasic(&item, uint64(nTested), nPassed)
					if ok {
						if (traceFilter.After == 0 || nPassed >= traceFilter.After) && (traceFilter.Count == 0 || uint64(nShown) < traceFilter.Count) {
							modelChan <- &item
							nShown++
						}
						nPassed++
					}
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

/*
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTrace));
        for (auto f : options.filters) {
            CTraceArray traces;
            getTracesByFilter(traces, f);
            for (auto trace : traces) {
                CBlock block;
                getBlock(block, trace.blockNumber);
                CTransaction trans;
                trans.timestamp = block.timestamp;
                trace.pTransaction = &trans;
                cout << (!options.firstOut ? ", " : "");
                cout << trace << endl;
                options.firstOut = false;
            }
        }
        once = false;
    }
    cout << export Postamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
void getTracesByFilter(CTraceArray& traces, const CTraceFilter& filter) {
    if (filter.Format() == CTraceFilter().Format())
        return;

    string_q toAddrs;
    for (auto addr : filter.toAddress) {
        if (!isZeroAddr(addr))
            toAddrs += ("\"" + addr + "\",");
    }
    if (!toAddrs.empty()) {
        toAddrs = "[" + trim(toAddrs, ',') + "]";
    }

    string_q fromAddrs;
    for (auto addr : filter.fromAddress) {
        if (!isZeroAddr(addr))
            fromAddrs += ("\"" + addr + "\",");
    }
    if (!fromAddrs.empty()) {
        fromAddrs = "[" + trim(fromAddrs, ',') + "]";
    }

#define AA(test, name, val) ((test) ? "\"" + string_q(name) + "\": " + (val) + "," : "")
    string_q params;
    params += AA(filter.fromBlock, "fromBlock", "\"" + uint_2_Hex(filter.fromBlock) + "\"");
    params += AA(filter.toBlock, "toBlock", "\"" + uint_2_Hex(filter.toBlock) + "\"");
    params += AA(!fromAddrs.empty(), "fromAddress", fromAddrs);
    params += AA(!toAddrs.empty(), "toAddress", toAddrs);
    params += AA(filter.after, "after", uint_2_Str(filter.after));
    params += AA(filter.count, "count", uint_2_Str(filter.count));
    params = "{" + trim(params, ',') + "}";

    cerr << substitute(params, " ", "") << endl;

    if (!filter.empty()) {
        string_q headerLine = "fromBlock,toBlock,fromAddress,toAddress,after,count";
        CStringArray headers;
        explode(headers, headerLine, ',');
        CTraceFilter f;
        string_q line = substitute(filter, "!", ",");
        f.parseCSV(headers, line);
        // block numbers may be hex, number or special, so handle them now
        CStringArray parts;
        explode(parts, filter, '!');
        if (parts.size() > 1)
            extractBlocksFromFilter(f.fromBlock, parts[0], f.toBlock, parts[1]);
        if (f.fromBlock > f.toBlock)
            return usage("filter.fromBlock must be less or equal to filter.toBlock.");
        if (f.fromBlock + 100 < f.toBlock)
            return usage("filter.fromBlock must be no more than 100 blocks before filter.toBlock.");
        filters.push_back(f);
        manageFields("CTraceAction:balance,init,refundAddress,selfDestructed", false);  // hide
        manageFields("CTraceResult:code,address", false);
        manageFields("CTrace:error", false);
        GETRUNTIME_CLASS(CTrace)->sortFieldList();
        GETRUNTIME_CLASS(CTraceAction)->sortFieldList();
        GETRUNTIME_CLASS(CTraceResult)->sortFieldList();
        if (isTestMode() && !isApi Mode()) {
            ostringstream os;
            for (auto ff : filters) {
                os << ff << endl;
            }
            LOG_INFO(os.str());
        }
        return true;
    }

bool COptions::extractBlocksFromFilter(blknum_t& b1, const string_q& p1, blknum_t& b2, const string_q& p2) {
    blknum_t latest = getLatestBlock_client();
    COptionsBlockList blocks(this);

    // if p1 is empty, the user provided nothing so just return
    if (p1.empty())
        return true;
    // parse p1 into b1
    if (!parseBlockList2(this, blocks, p1, latest))
        return usage("Could not parse invalid block " + p1 + ".");
    b1 = blocks.numList.size() ? blocks.numList[0] : latest;

    // if p2 is empty, set b2 to b1 and return
    if (p2.empty()) {
        b2 = b1;
        return true;
    }

    if (!parseBlockList2(this, blocks, p2, latest))
        return usage("Could not parse invalid block " + p2 + ".");
    b2 = blocks.numList.size() > 1 ? blocks.numList[1] : b1;

    return true;
}

name        ,type    ,strDefault ,array ,omitempty ,doc ,example ,description
fromBlock   ,blk num  ,           ,      ,          ,    ,        ,
toBlock     ,blk num  ,           ,      ,          ,    ,        ,
fromAddress ,address ,           ,true  ,          ,    ,        ,
toAddress   ,address ,           ,true  ,          ,    ,        ,
after       ,uint64  ,           ,      ,          ,    ,        ,
count       ,uint64  ,           ,      ,          ,    ,        ,

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTraceFilter : public CBaseNode {
  public:
    blknum_t fromBlock;
    blknum_t toBlock;
    CAddressArray fromAddress;
    CAddressArray toAddress;
    uint64_t after;
    uint64_t count;

  public:
    CTraceFilter(void);
    CTraceFilter(const CTraceFilter& tr);
    virtual ~CTraceFilter(void);
    CTraceFilter& operator=(const CTraceFilter& tr);

    DECLARE_NODE(CTraceFilter);

    const string_q getStringAt(const string_q& fieldName, size_t i) const override;

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CTraceFilter& it) const;
    bool operator!=(const CTraceFilter& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CTraceFilter& v1, const CTraceFilter& v2);
    friend ostream& operator<<(ostream& os, const CTraceFilter& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CTraceFilter& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTraceFilter::CTraceFilter(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceFilter::CTraceFilter(const CTraceFilter& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTraceFilter::~CTraceFilter(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::initialize(void) {
    CBaseNode::initialize();

    fromBlock = 0;
    toBlock = 0;
    fromAddress.clear();
    toAddress.clear();
    after = 0;
    count = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTraceFilter::duplicate(const CTraceFilter& tr) {
    clear();
    CBaseNode::duplicate(tr);

    fromBlock = tr.fromBlock;
    toBlock = tr.toBlock;
    fromAddress = tr.fromAddress;
    toAddress = tr.toAddress;
    after = tr.after;
    count = tr.count;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTraceFilter& CTraceFilter::operator=(const CTraceFilter& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CTraceFilter::operator==(const CTraceFilter& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CTraceFilter& v1, const CTraceFilter& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CTraceFilter> CTraceFilterArray;
extern CArchive& operator>>(CArchive& archive, CTraceFilterArray& array);
extern CArchive& operator<<(CArchive& archive, const CTraceFilterArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CTraceFilter& tra);
extern CArchive& operator>>(CArchive& archive, CTraceFilter& tra);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRACEFILTER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE


//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTraceFilter, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextTracefilterChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTracefilterChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTraceFilter::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["tracefilter_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTracefilterChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTracefilterChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTraceFilter*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTraceFilter::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTracefilterChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "after") {
                return uint_2_Str(after);
            }
            break;
        case 'c':
            if (fieldName % "count") {
                return uint_2_Str(count);
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                return uint_2_Str(fromBlock);
            }
            if (fieldName % "fromAddress" || fieldName % "fromAddressCnt") {
                size_t cnt = fromAddress.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + fromAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                return uint_2_Str(toBlock);
            }
            if (fieldName % "toAddress" || fieldName % "toAddressCnt") {
                size_t cnt = toAddress.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += ("\"" + toAddress[i] + "\"");
                    retS += ((i < cnt - 1) ? ",\n" + indentStr() : "\n");
                }
                return retS;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "after") {
                after = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "count") {
                count = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fromBlock") {
                fromBlock = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "fromAddress") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    fromAddress.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        case 't':
            if (fieldName % "toBlock") {
                toBlock = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "toAddress") {
                string_q str = fieldValue;
                while (!str.empty()) {
                    toAddress.push_back(str_2_Addr(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTraceFilter::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> fromBlock;
    archive >> toBlock;
    archive >> fromAddress;
    archive >> toAddress;
    archive >> after;
    archive >> count;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fromBlock;
    archive << toBlock;
    archive << fromAddress;
    archive << toAddress;
    archive << after;
    archive << count;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTraceFilter::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CTraceFilter copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceFilterArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTraceFilterArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTraceFilter::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTraceFilter, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTraceFilter, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTraceFilter, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceFilter, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTraceFilter, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTraceFilter, "fromBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTraceFilter, "toBlock", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTraceFilter, "fromAddress", T_ADDRESS | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceFilter, "toAddress", T_ADDRESS | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTraceFilter, "after", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTraceFilter, "count", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTraceFilter, "schema");
    HIDE_FIELD(CTraceFilter, "deleted");
    HIDE_FIELD(CTraceFilter, "showing");
    HIDE_FIELD(CTraceFilter, "cname");

    builtIns.push_back(_biCTraceFilter);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTracefilterChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTraceFilter* tra = reinterpret_cast<const CTraceFilter*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tra);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CTraceFilter::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CTraceFilter& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTraceFilter& tra) {
    tra.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTraceFilter& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const string_q CTraceFilter::getStringAt(const string_q& fieldName, size_t i) const {
    if (fieldName % "fromAddress" && i < fromAddress.size())
        return (fromAddress[i]);
    if (fieldName % "toAddress" && i < toAddress.size())
        return (toAddress[i]);
    return "";
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRACEFILTER = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

bool wrangleTxId(string_q& argOut, string_q& errorMsg) {
    if (contains(argOut, "0x"))
        return true;

    // valid args are 'latest', 'bn.txid', 'bn.txid.next', or 'bn.txid.prev'
    replaceReverse(argOut, ":", ".");

    CStringArray parts;
    explode(parts, argOut, '.');

    if ((parts.size() == 2) && endsWith(argOut, ".*")) {
        CBlock block;
        getBlockLight(block, str_2_Uint(parts[0]));
        argOut = parts[0] + ".0";
        if (block.transactions.size() > 0)
            argOut += ("-to-" + uint_2_Str(block.transactions.size()));
        return true;
    }

    if (parts.size() == 0) {
        errorMsg = argOut + " does not appear to be a valid transaction index (no parts).";
        return false;

    } else if (parts.size() == 1 && (parts[0] != "latest" && parts[0] != "first")) {
        errorMsg = argOut + " does not appear to be a valid transaction index (one part, not first or latest).";
        return false;

    } else if (parts.size() > 3) {
        errorMsg = argOut + " does not appear to be a valid transaction index (too many).";
        return false;
    } else {
        // two or three parts...
        if (!isNumeral(parts[0]) || !isNumeral(parts[1])) {
            errorMsg = argOut + " does not appear to be a valid transaction index.";
            return false;
        }
    }

    if (parts.size() == 2)
        return true;

    // it's directional
    if (parts[0] == "latest") {
        CBlock block;
        getBlockLight(block, "latest");
        if (block.transactions.size() > 0) {
            ostringstream os;
            os << block.blockNumber << "." << (block.transactions.size() - 1);
            argOut = os.str();
            return true;
        }
        parts[0] = uint_2_Str(block.blockNumber);
        parts[1] = "0";
        parts[2] = "prev";
    }

    if (parts[0] == "first") {
        argOut = "46147.0";
        return true;
    }

    ASSERT(parts[2] == "prev" || parts[2] == "next");
    return getDirectionalTxId(str_2_Uint(parts[0]), str_2_Uint(parts[1]), parts[2], argOut, errorMsg);
}

//--------------------------------------------------------------------------------
bool getDirectionalTxId(blknum_t bn, tx num_t txid, const string_q& dir, string_q& argOut, string_q& errorMsg) {
    blknum_t lastBlock = getLatestBlock_client();

    if (bn < firstTransactionBlock()) {
        argOut = uint_2_Str(firstTransactionBlock()) + ".0";
        return true;
    }

    CBlock block;
    getBlock(block, bn);

    argOut = "";
    tx num_t nextid = txid + 1;
    while (argOut.empty() && bn >= firstTransactionBlock() && bn <= lastBlock) {
        if (dir == "next") {
            if (nextid < block.transactions.size()) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(nextid);
                return true;
            }
            block = CBlock();
            getBlock(block, ++bn);
            nextid = 0;
        } else if (dir == "prev") {
            if (txid > 0 && block.transactions.size() > 0) {
                argOut = uint_2_Str(block.blockNumber) + "." + uint_2_Str(txid - 1);
                return true;
            }
            if (bn == 0)
                return true;
            block = CBlock();
            getBlock(block, --bn);
            txid = block.transactions.size();
        }
    }
    errorMsg = "Could not find " + dir + " transaction to " + uint_2_Str(bn) + "." + uint_2_Str(txid);
    return false;
}

//--------------------------------------------------------------------------------
bool parseTransList2(COptionsBase* opt, COptionsTransList& transList, const string_q& argIn) {
    string_q errorMsg;
    string_q arg = argIn;
    if (!wrangleTxId(arg, errorMsg))
        return (opt ? opt->usage(errorMsg) : false);
    string_q ret = transList.parseTransList(arg);
    if (!ret.empty())
        return (opt ? opt->usage(ret) : false);
    return true;
}

//--------------------------------------------------------------
extern bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last);
bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        CBlock b1, b2;
        getBlockHeader(b1, mid);
        getBlockHeader(b2, mid + 1);
        bool atMid = (b1.timestamp <= block.timestamp);
        bool atMid1 = (b2.timestamp <= block.timestamp);
        if (atMid && !atMid1) {
            block = b1;
            return true;
        } else if (!atMid) {
            // we're too high, so search below
            return findTimestamp_binarySearch(block, first, mid - 1);
        }
        // we're too low, so search above
        return findTimestamp_binarySearch(block, mid + 1, last);
    }
    getBlockHeader(block, first);
    return true;
}

//---------------------------------------------------------------
bool lookupDate(CBlock& block, const timestamp_t& ts, blknum_t latest) {
    time_q date = ts_2_Date(ts);

    // speed up
    blknum_t start = 1, stop = latest;
    if (date.GetYear() >= 2021) {
        start = 11565019;
    } else if (date.GetYear() >= 2020) {
        start = 9193265;
        stop = 11565019;
    } else if (date.GetYear() >= 2019) {
        start = 6988614;
        stop = 9193265;
    } else if (date.GetYear() >= 2018) {
        start = 4832685;
        stop = 6988614;
    } else if (date.GetYear() >= 2017) {
        start = 2912406;
        stop = 4832685;
    } else if (date.GetYear() >= 2016) {
        start = 778482;
        stop = 2912406;
    }

    block.timestamp = ts;
    bool ret = findTimestamp_binarySearch(block, start, stop);
    if (!isTestMode()) {
        cerr << "\r";
        cerr.flush();
    }
    return ret;
}

//--------------------------------------------------------------------------------
bool parseBlockList2(COptionsBase* opt, & blocks, const string_q& argIn, blknum_t latest) {
    string_q ret = blocks.parseBlockList_inner(argIn, latest);
    if (endsWith(ret, "\n")) {
        cerr << "\n  " << ret << "\n";
        return false;
    } else if (!ret.empty()) {
        return (opt ? opt->usage(ret) : false);
    }

    if (blocks.skip_type != BY_NOTHING) {
        time_q startDate = getBlockDate(blocks.start);
        time_q stopDate = getBlockDate(blocks.stop);
        CTimeArray times;
        switch (blocks.skip_type) {
            case BY_HOUR:
                expandHourly(times, startDate, stopDate);
                break;
            case BY_DAY:
                expandDaily(times, startDate, stopDate);
                break;
            case BY_WEEK:
                expandWeekly(times, startDate, stopDate);
                break;
            case BY_MONTH:
                expandMonthly(times, startDate, stopDate);
                break;
            case BY_QUARTER:
                expandQuarterly(times, startDate, stopDate);
                break;
            case BY_YEAR:
            default:
                expandAnnually(times, startDate, stopDate);
                break;
        }
        blocks.start = NOPOS;
        blocks.stop = NOPOS;
        for (auto t : times) {
            CBlock block;
            lookupDate(block, date_2_Ts(t), latest);
            blocks.numList.push_back(block.blockNumber);
        }
    }

    return true;
}

parseBlockOption
(string_q& msg, blknum_t lastBlock, direction_t offset,
                                             bool& hasZero) const {
    blknum_t ret = NOPOS;

    string_q arg = msg;
    msg = "";

    // if it's a number, return it
    if (isNumeral(arg) || isHexStr(arg)) {
        ret = str_2_Uint(arg);
        if (!ret)
            hasZero = true;

    } else {
        // if it's not a number, it better be a special value, and we better be able to find it
        CNameValue spec;
        if (COptionsBase::findSpecial(spec, arg)) {
            if (spec.first == "latest")
                spec.second = uint_2_Str(lastBlock);
            ret = str_2_Uint(spec.second);
            if (!ret)
                hasZero = true;

        } else {
            msg = "The given value '" + arg + "' is not a valid identifier." + (isApi Mode() ? "" : "\n");
            return NOPOS;
        }
    }

    ASSERT(opts);
    if (opts->isEnabled(OPT_CHECKLATEST) && ret > lastBlock) {
        string_q lateStr = (isTestMode() ? "--" : uint_2_Str(lastBlock));
        msg = "Block " + arg + " is later than the last valid block " + lateStr + "." + (isApi Mode() ? "" : "\n");
        return NOPOS;
    }

    if (offset == PREV && ret > 0)
        ret--;
    else if (offset == NEXT && ret < lastBlock)
        ret++;

    if (!ret)
        hasZero = true;
    return ret;
}

//--------------------------------------------------------------------------------
static string_q skip_markers = "untimed|annually|quarterly|monthly|weekly|daily|hourly";

//--------------------------------------------------------------------------------
string_q ::parseBlockList_inner(const string_q& argIn, blknum_t lastBlock) {
    string_q arg = argIn;

    direction_t offset = (contains(arg, ".next") ? NEXT : contains(arg, ".prev") ? PREV : NODIR);
    arg = substitute(substitute(arg, ".next", ""), ".prev", "");

    skip_type = BY_NOTHING;

    // scrape off the skip marker if any
    if (contains(arg, ":")) {
        string_q skip_marker = arg;
        arg = nextTokenClear(skip_marker, ':');
        if (isNumeral(skip_marker)) {
            skip = max(blknum_t(1), str_2_Uint(skip_marker));
        } else {
            if (!contains(skip_markers, skip_marker))
                return "Input argument appears to be invalid. No such skip marker: " + argIn;
            if (contains(skip_marker, "annually")) {
                skip_type = BY_YEAR;
            } else if (contains(skip_marker, "quarterly")) {
                skip_type = BY_QUARTER;
            } else if (contains(skip_marker, "monthly")) {
                skip_type = BY_MONTH;
            } else if (contains(skip_marker, "weekly")) {
                skip_type = BY_WEEK;
            } else if (contains(skip_marker, "daily")) {
                skip_type = BY_DAY;
            } else if (contains(skip_marker, "hourly")) {
                skip_type = BY_HOUR;
            } else {
                ASSERT(0);  // should never happen
                skip = 1;
            }
            CStringArray m;
            explode(m, skip_markers, '|');
            LOG_INFO("skip_type: ", m[skip_type - 19]);  // see definition of BY_YEAR for this offset
        }
    }

    if (contains(arg, "-") || contains(arg, "+")) {
        // If we already have a range, bail
        if (start != stop)
            return "Specify only a single block range at a time.";

        if (startsWith(arg, "latest"))
            return "Cannot start range with 'latest'";

        if (contains(arg, "+")) {
            string_q n = nextTokenClear(arg, '+');
            blknum_t s1 = parseBlockOption(n, lastBlock, NODIR, hasZeroBlock);
            if (!n.empty())
                return n;
            n = arg;
            blknum_t s2 = parseBlockOption(n, lastBlock, NODIR, hasZeroBlock);
            if (!n.empty())
                return n;
            s2 = s1 + s2;
            arg = uint_2_Str(s1) + "-" + uint_2_Str(s2);
        }

        string_q stp = arg;
        string_q strt = nextTokenClear(stp, '-');
        string_q msg = strt;
        start = parseBlockOption(msg, lastBlock, offset, hasZeroBlock);
        if (!msg.empty())
            return msg;
        msg = stp;
        stop = parseBlockOption(msg, lastBlock, offset, hasZeroBlock);
        if (!msg.empty())
            return msg;

        if (stop <= start)
            return "'stop' must be strictly larger than 'start'";

    } else {
        if (isHash(arg)) {
            hashList.push_back(arg);

        } else {
            string_q msg = arg;
            blknum_t num = parseBlockOption(msg, lastBlock, offset, hasZeroBlock);
            if (!msg.empty())
                return msg;
            numList.push_back(num);
        }
    }

    latest = lastBlock;
    return "";
}

//--------------------------------------------------------------------------------
void ::Init(void) {
    numList.clear();
    hashList.clear();
    start = stop = 0;
    skip = 1;
    hashFind = NULL;
}

//--------------------------------------------------------------------------------
::(const COptionsBase* o) {
    Init();
    opts = o;
}

//--------------------------------------------------------------------------------
bool ::forEveryBlock Number(UINT64VISITFUNC func, void* data) const {
    if (!func)
        return false;

    for (uint64_t i = start; i < stop; i = i + skip) {
        if (!(*func)(i, data))
            return false;
    }
    for (size_t i = 0; i < numList.size(); i++) {
        uint64_t n = numList[i];
        if (!(*func)(n, data))
            return false;
    }
    if (hashFind) {
        for (size_t i = 0; i < hashList.size(); i++) {
            uint64_t n = (*hashFind)(hashList[i], data);
            if (n != NOPOS) {
                if (!(*func)(n, data))
                    return false;
            }
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool ::isInRange(blknum_t bn) const {
    if (start <= bn && bn < stop)
        return true;
    for (size_t i = 0; i < numList.size(); i++)
        if (bn == numList[i])
            return true;
    return false;
}

}  // namespace qblocks
class  {
  public:
    const COptionsBase* opts;
    CBlknumArray numList;
    CStringArray hashList;
    HASHFINDFUNC hashFind;
    blknum_t start;
    blknum_t stop;
    blknum_t skip;
    blknum_t latest;
    period_t skip_type;
    bool hasZeroBlock{false};

    (const COptionsBase* o);
    void Init(void);
    string_q parseBlockList_inner(const string_q& arg, blknum_t latest);
    bool forEveryBlock Number(UINT64VISITFUNC func, void*) const;
    bool empty(void) const {
        return !(hashList.size() || numList.size() || (start != stop));
    }
    size_t nItems(void) const {
        return hashList.size() + numList.size() + (start - stop);
    }
    size_t size(void) const {
        CUintArray nums;
        forEveryBlockNumber(listBlocks, &nums);
        return nums.size();
    }
    uint64_t operator[](size_t offset) const {
        CUintArray nums;
        forEveryBlockNumber(listBlocks, &nums);
        return nums[offset];
    }
    bool isInRange(blknum_t bn) const;
    blknum_t parseBlockOption(string_q& msg, blknum_t lastBlock, direction_t offset, bool& hasZero) const;

  private:
    (void) = delete;
};
*/
