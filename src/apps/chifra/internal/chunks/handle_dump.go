package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/validate"
)

func (opts *ChunksOptions) HandleDump(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	_ = rCtx
	_ = blockNums
	return validate.Usage("Extractor for {0} not yet implemented.", opts.Mode)
}

/*
chifra chunks addresses --details --chain sepolia --output ten

echo
echo "Timestamps..."
chifra when --timestamps 2>&1 | tee -a ten/scrape

echo
echo "Manifest"
cat ~/Library/Application\ Support/TrueBlocks/config/sepolia/manifest.json 2>/dev/null | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks pins" | tee -a ten/scrape
chifra chunks pins 2>/dev/null | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks manifest" | tee -a ten/scrape
chifra chunks manifest 2>/dev/null | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra when --timestamps --check" | tee -a ten/scrape
chifra when --timestamps --check 2>&1 | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks blooms" | tee -a ten/scrape
chifra chunks blooms 2>&1 | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks addresses --detail" | tee -a ten/scrape
chifra chunks addresses --details 2>&1 | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks index" | tee -a ten/scrape
chifra chunks index 2>&1 | tee -a ten/scrape

echo | tee -a ten/scrape
echo "chifra chunks stats" | tee -a ten/scrape
chifra chunks stats 2>&1 | tee -a ten/scrape

rm -f testing
*/
