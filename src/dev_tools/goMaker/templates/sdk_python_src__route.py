#
# This file was generated with makeClass --sdk. Do not edit it.
#
from . import session

{{.Route}}Cmd = "{{.Route}}"
{{.Route}}Pos = "{{.FirstPositional}}"
{{.Route}}Fmt = "json"
{{.Route}}Opts = {
[{OPTIONS}][{GLOBALS}]    "fmt": {"hotkey": "-x", "type": "flag"},
}

def {{.Route}}(self):
    ret = self.toUrl({{.Route}}Cmd, {{.Route}}Pos, {{.Route}}Fmt, {{.Route}}Opts)
    url = 'http://localhost:8080/' + ret[1]
    if ret[0] == 'json':
        return session.get(url).json()
    return session.get(url).text
