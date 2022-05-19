package validate

import (
	"fmt"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

func Test_BlockRanges(t *testing.T) {
	for _, item := range testBlocks {
		if !item.enabled {
			continue
		}
		// fmt.Println("----------->", item.input)
		inputs := strings.Split(item.input, " ")
		var results []blockRange.Identifier
		var err error
		err = ValidateIdentifiers(
			"mainnet",
			inputs,
			ValidBlockIdWithRangeAndDate,
			100,
			&results,
		)
		if err != nil {
			fmt.Println(item.input, err)
			continue
		}
		for _, br := range results {
			blockList, err := br.ResolveBlocks("mainnet")
			if err != nil {
				t.Error(br)
				t.Error(err)
			}
			if len(blockList) > 0 {
				max := len(blockList)
				min := 0
				if len(blockList) > 10 {
					max = 10
				}
				if len(blockList) > 3 {
					min = len(blockList) - 3
				}
				str := fmt.Sprintf("%s|%d|%v...%v", br.Orig, len(blockList), blockList[:max], blockList[min:])
				check := colors.Green
				if str != item.expected {
					check = colors.Red
					// fmt.Printf("got:    %s\nwanted: %s%s%s\n", str, check, item.expected, colors.Off)
					t.Errorf("got:    %s\nwanted: %s%s%s\n", str, check, item.expected, colors.Off)
				}
				// } else {
				// 	fmt.Println("No blocks:", colors.Yellow, br, colors.Off)
			}
		}
	}
}

func Test_TransactionIds(t *testing.T) {
	// for i, item := range testTxs {
	// if !item.enabled {
	// continue
	// }
	// if strings.Contains(item.input, ":next") || strings.Contains(item.input, ":prev") {
	// 	continue
	// }
	// 	fmt.Println("-----", i, "----->", item.input)
	// 	inputs := strings.Split(item.input, " ")
	// 	expecteds := strings.Split(item.expected, " ")
	// 	var results []blockRange.Identifier
	// 	var err error
	// 	err = ValidateIdentifiers(
	// 		"mainnet",
	// 		inputs,
	// 		ValidTransId,
	// 		100,
	// 		&results,
	// 	)
	// 	if err != nil {
	// 		fmt.Println(colors.Red, item.input, err, colors.Off)
	// 		continue
	// 	}
	// 	for i, br := range results {
	// 		fmt.Println(br)
	// 		txList, err := br.ResolveTxs("mainnet")
	// 		if err != nil {
	// 			t.Error(colors.Red, br)
	// 			t.Error(err, colors.Off)
	// 		}
	// 		for _, tx := range txList {
	// 			res := fmt.Sprintf("%d.%d", tx.BlockNumber, tx.TransactionIndex)
	// 			if res != expecteds[i] {
	// 				fmt.Printf("%sgot: %d.%d expected: %s%s", colors.Red, tx.BlockNumber, tx.TransactionIndex, expecteds[i], colors.Off)
	// 			} else {
	// 				fmt.Printf("%s%d.%d%s", colors.Green, tx.BlockNumber, tx.TransactionIndex, colors.Off)
	// 			}
	// 		}
	// 		if len(txList) == 0 {
	// 			fmt.Printf("%sgot: nothing expected: %s%s", colors.Red, expecteds[i], colors.Off)
	// 		}
	// 		fmt.Println()
	// 	}
	// }
}

type TestCase struct {
	input    string
	expected string
	enabled  bool
}

var testBlocks = []TestCase{
	{
		input:    "1001001.0",
		expected: "1001001.0",
		// enabled:  true,
	},
	{
		input:    "195633",
		expected: "195633|1|[195633]...[195633]",
		enabled:  true,
	},
	{
		input:    "195633-200000",
		expected: "195633-200000|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "195633-1441661589",
		expected: "195633-1441661589|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "195633-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693",
		expected: "195633-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
		enabled:  true,
	},
	{
		input:    "195633-2015-09-07",
		expected: "195633-2015-09-07|1|[195633]...[195633]",
		enabled:  true,
	},
	{
		input:    "195633-2015-09-07T21",
		expected: "195633-2015-09-07T21|4256|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199886 199887 199888]",
		enabled:  true,
	},
	{
		input:    "195633-2015-09-07T21:33",
		expected: "195633-2015-09-07T21:33|4365|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199995 199996 199997]",
		enabled:  true,
	},
	{
		input:    "195633-2015-09-07T21:33:09",
		expected: "195633-2015-09-07T21:33:09|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
		enabled:  true,
	},
	{
		input:    "195633-iceage",
		expected: "195633-iceage|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
		enabled:  true,
	},
	{
		input:    "195633-iceage:51",
		expected: "195633-iceage:51|86|[195633 195684 195735 195786 195837 195888 195939 195990 196041 196092]...[199866 199917 199968]",
		enabled:  true,
	},
	{
		input:    "195433-iceage:hourly",
		expected: "195433-iceage:hourly|23|[195433 195634 195845 196049 196249 196438 196646 196843 197031 197234]...[199494 199694 199889]",
		enabled:  true,
	},
	{
		input:    "1441583975",
		expected: "1441583975|1|[195633]...[195633]",
		enabled:  true,
	},
	{
		input:    "1441583975-200000",
		expected: "1441583975-200000|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "1441583975-1441661589",
		expected: "1441583975-1441661589|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "1441583975-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693",
		expected: "1441583975-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "1441583975-2015-09-07",
		expected: "1441583975-2015-09-07|1|[195633]...[195633]",
	},
	{
		input:    "1441583975-2015-09-07T21",
		expected: "1441583975-2015-09-07T21|4256|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199886 199887 199888]",
	},
	{
		input:    "1441583975-2015-09-07T21:33",
		expected: "1441583975-2015-09-07T21:33|4365|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199995 199996 199997]",
	},
	{
		input:    "1441583975-2015-09-07T21:33:09",
		expected: "1441583975-2015-09-07T21:33:09|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "1441583975-iceage",
		expected: "1441583975-iceage|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "1441583975-iceage:51",
		expected: "1441583975-iceage:51|86|[195633 195684 195735 195786 195837 195888 195939 195990 196041 196092]...[199866 199917 199968]",
	},
	{
		input:    "1440709168-iceage:daily",
		expected: "1440709168-iceage:daily|12|[148428 153135 157788 162118 166272 170394 174332 177909 181292 185735]...[185735 190656 195634]",
		enabled:  true,
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2|1|[195633]...[195633]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-200000",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-200000|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-1441661589",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-1441661589|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
		enabled:  true,
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07|1|[195633]...[195633]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21|4256|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199886 199887 199888]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21:33",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21:33|4365|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199995 199996 199997]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21:33:09",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-2015-09-07T21:33:09|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-iceage",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-iceage|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-iceage:51",
		expected: "0xf6bfe0a959330e01858c63512b9deb4c4308944584eb0080e2b85e90fcedd9f2-iceage:51|86|[195633 195684 195735 195786 195837 195888 195939 195990 196041 196092]...[199866 199917 199968]",
	},
	{
		input:    "0x15576d5deb72a552fcde26caf8587c40bf39f0c8f9fde5bbb21f3164e0cf2bc5-devcon1:weekly",
		expected: "0x15576d5deb72a552fcde26caf8587c40bf39f0c8f9fde5bbb21f3164e0cf2bc5-devcon1:weekly|14|[93129 128130 162118 190656 225596 259991 295664 330002 364660 399788]...[470667 506226 541677]",
	},
	{
		input:    "2015-09-06",
		expected: "2015-09-06|1|[190656]...[190656]",
	},
	{
		input:    "2015-09-06T23-200000",
		expected: "2015-09-06T23-200000|4567|[195433 195434 195435 195436 195437 195438 195439 195440 195441 195442]...[199997 199998 199999]",
	},
	{
		input:    "2015-09-06T23:59-1441661589",
		expected: "2015-09-06T23:59-1441661589|4369|[195631 195632 195633 195634 195635 195636 195637 195638 195639 195640]...[199997 199998 199999]",
	},
	{
		input:    "2015-09-06T23:59:35-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693",
		expected: "2015-09-06T23:59:35-0x13ced9eaa49a522d4e7dcf80a739a57dbf08f4ce5efc4edbac86a66d8010f693|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
		enabled:  true,
	},
	{
		input:    "2015-09-06-2015-09-07",
		expected: "2015-09-06-2015-09-07|4978|[190656 190657 190658 190659 190660 190661 190662 190663 190664 190665]...[195631 195632 195633]",
	},
	{
		input:    "2015-09-06T23-2015-09-07T21",
		expected: "2015-09-06T23-2015-09-07T21|4456|[195433 195434 195435 195436 195437 195438 195439 195440 195441 195442]...[199886 199887 199888]",
	},
	{
		input:    "2015-09-06T23:59-2015-09-07T21:33",
		expected: "2015-09-06T23:59-2015-09-07T21:33|4367|[195631 195632 195633 195634 195635 195636 195637 195638 195639 195640]...[199995 199996 199997]",
		enabled:  true,
	},
	{
		input:    "2015-09-06T23:59:35-2015-09-07T21:33:09",
		expected: "2015-09-06T23:59:35-2015-09-07T21:33:09|4367|[195633 195634 195635 195636 195637 195638 195639 195640 195641 195642]...[199997 199998 199999]",
	},
	{
		input:    "2015-09-06-iceage",
		expected: "2015-09-06-iceage|9344|[190656 190657 190658 190659 190660 190661 190662 190663 190664 190665]...[199997 199998 199999]",
	},
	{
		input:    "2015-09-06T23-iceage:51",
		expected: "2015-09-06T23-iceage:51|90|[195433 195484 195535 195586 195637 195688 195739 195790 195841 195892]...[199870 199921 199972]",
	},
	{
		input:    "2018-03-18T23:23:11-istanbul:monthly",
		expected: "2018-03-18T23:23:11-istanbul:monthly|22|[5174124 5357794 5534862 5710964 5883489 6065979 6249398 6430272 6620475 6803256]...[8653171 8849471 9029509]",
	},
	{
		input:    "istanbul",
		expected: "istanbul|1|[9069000]...[9069000]",
		enabled:  true,
	},
	{
		input:    "muirglacier-9200001",
		expected: "muirglacier-9200001|1|[9200000]...[9200000]",
	},
	{
		input:    "istanbul-9069050:10",
		expected: "istanbul-9069050:10|5|[9069000 9069010 9069020 9069030 9069040]...[9069020 9069030 9069040]",
	},
	{
		input:    "istanbul-1575765384",
		expected: "istanbul-1575765384|50|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9069047 9069048 9069049]",
	},
	{
		input:    "istanbul-0xc6c6091af102c8294959e2a2b15a7fc68270ae17bfef3dec04d81398104daf40",
		expected: "istanbul-0xc6c6091af102c8294959e2a2b15a7fc68270ae17bfef3dec04d81398104daf40|50|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9069047 9069048 9069049]",
	},
	{
		input:    "istanbul-2019-12-09",
		expected: "istanbul-2019-12-09|5508|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9074505 9074506 9074507]",
	},
	{
		input:    "istanbul-2019-12-09T00",
		expected: "istanbul-2019-12-09T00|5508|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9074505 9074506 9074507]",
	},
	{
		input:    "istanbul-2019-12-08T00:36",
		expected: "istanbul-2019-12-08T00:36|48|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9069045 9069046 9069047]",
	},
	{
		input:    "istanbul-2019-12-08T00:36:24",
		expected: "istanbul-2019-12-08T00:36:24|50|[9069000 9069001 9069002 9069003 9069004 9069005 9069006 9069007 9069008 9069009]...[9069047 9069048 9069049]",
	},
	{
		input:    "istanbul-muirglacier:13001",
		expected: "istanbul-muirglacier:13001|11|[9069000 9082001 9095002 9108003 9121004 9134005 9147006 9160007 9173008 9186009]...[9173008 9186009 9199010]",
		enabled:  true,
	},
	{
		input:    "daohack-muirglacier:6000",
		expected: "daohack-muirglacier:6000|1247|[1718497 1724497 1730497 1736497 1742497 1748497 1754497 1760497 1766497 1772497]...[9182497 9188497 9194497]",
	},
	{
		input:    "iceage-13922671:annually",
		expected: "iceage-13922671:annually|8|[0 778482 2912406 4832685 6988614 9193265 11565019 13916165]...[9193265 11565019 13916165]",
	},
	{
		input:    "0",
		expected: "0|1|[0]...[0]",
		enabled:  true,
	},
}

var testTxs = []TestCase{
	{
		input:    "1001001.0",
		expected: "1001001.0",
		enabled:  true,
	},
	{
		input:    "1001001.0:next",
		expected: "1001001.1",
		enabled:  true,
	},
	{
		input:    "1001001.0:prev",
		expected: "1001000.2",
		enabled:  true,
	},
	{
		input:    "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53.2",
		expected: "1001001.2",
		enabled:  true,
	},
	{
		input:    "0x0b4c6fb75ded4b90218cf0346b0885e442878f104e1b60bf75d5b6860eeacd53:*",
		expected: "1001001.0 1001001.1 1001001.2 1001001.3",
		enabled:  true,
	},
	{
		input:    "0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6",
		expected: "1001000.2",
		enabled:  true,
	},
	{
		input:    "0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6:next",
		expected: "1001001.0",
		enabled:  true,
	},
	{
		input:    "1001001.0 1001001.0:next 1001001.0:prev 1001001.2",
		expected: "1001001.0 1001001.1 1001000.2 1001001.2",
		enabled:  true,
	},
	{
		input:    "0x730724cb08a6eb17bf6b3296359d261570d343ea7944a17a9d7287d77900db08 0xef2ea39c20ba09553b2f3cf02380406ac766039ca56612937eed5e7f3503fb3a 0xc20a01b9d0bc87268376d189044e2c76cb2b34dda31e5525cbef45b3c30849e6 0x5352c80aa2073e21ce6c4aa5488c38455f3519955ece7dca5af3e326797bcc63",
		expected: "1001001.0 1001001.1 1001000.2 1001001.2",
		enabled:  true,
	},
}
