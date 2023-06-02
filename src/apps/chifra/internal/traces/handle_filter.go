package tracesPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *TracesOptions) HandleFilter() error {
	abiMap := make(abi.AbiInterfaceMap)
	loadedMap := make(map[base.Address]bool)
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawTrace], errorChan chan error) {
		traces, err := rpcClient.GetTracesByFilter(opts.Globals.Chain, opts.Filter)
		if err != nil {
			errorChan <- err
			cancel()
			return
		}
		if len(traces) == 0 {
			return
		}

		for _, trace := range traces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			trace := trace
			trace.Timestamp = rpc.GetBlockTimestamp(opts.Globals.Chain, uint64(trace.BlockNumber))
			if opts.Articulate {
				var err error
				address := trace.Action.To
				if !loadedMap[address] {
					if err = abi.LoadAbi(chain, address, abiMap); err != nil {
						errorChan <- err // continue even with an error
						err = nil
					}
				}
				if err == nil {
					trace.ArticulatedTrace, err = articulate.ArticulateTrace(&trace, abiMap)
					if err != nil {
						errorChan <- err // continue even with an error
					}
				}
			}
			modelChan <- &trace
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
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

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
        if (isTestMode() && !isApiMode()) {
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

name        ,type    ,strDefault ,object ,array ,nowrite ,omitempty ,minimal ,noaddfld ,doc ,disp ,example ,description
fromBlock   ,blknum  ,           ,       ,      ,        ,          ,        ,         ,    ,     ,        ,
toBlock     ,blknum  ,           ,       ,      ,        ,          ,        ,         ,    ,     ,        ,
fromAddress ,address ,           ,       ,true  ,        ,          ,        ,         ,    ,     ,        ,
toAddress   ,address ,           ,       ,true  ,        ,          ,        ,         ,    ,     ,        ,
after       ,uint64  ,           ,       ,      ,        ,          ,        ,         ,    ,     ,        ,
count       ,uint64  ,           ,       ,      ,        ,          ,        ,         ,    ,     ,        ,

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

*/
