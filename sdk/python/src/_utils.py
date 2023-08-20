import os, sys, logging

def toUrl(self, cmd, posName, defFmt, options):
    fmt = ''
    ret = ''
    skip = False
    for i, arg in enumerate(sys.argv):
        # logging.info("arg: " + arg)
        if i < 2:
            continue
        
        for key in options:
            if arg == options[key]["hotkey"]:
                arg = "--" + key

        if skip == True:
            skip = False
            continue

        val = ''
        option = arg.replace("-","")
        if option == "help":
            os.system("chifra " + cmd + " --help")

        # logging.info(option + ":")
        if options.keys().__contains__(option) == True:
            # logging.info("  hotKey: " + options[option]["hotkey"])
            # logging.info("  type: " + options[option]["type"])

            match options[option]["type"]:
                case 'switch':
                    if len(ret) > 0:
                        val += "&"
                    val += option
                case 'flag':
                    if len(sys.argv) > i+1:
                        if len(ret) > 0:
                            val += "&"
                        val += arg.replace("-", "") + "=" + sys.argv[i+1]
                        if option == "fmt":
                            fmt = sys.argv[i+1]
                        skip = True

        if val == "":
            if len(ret) > 0:
                val += "&"
            if arg.startswith("-") == True:
                val += arg.replace('-', '')
            else:
                val += posName + "=" + arg
            
        ret += val

    if fmt == '':
        fmt = defFmt
        if len(ret) > 0:
            ret += "&"
        ret += "fmt=" + fmt

    # logging.info("cmd: " + cmd)
    # logging.info("fmt: " + fmt)
    # logging.info("ret: " + ret)
    return fmt, cmd + "?" + ret
