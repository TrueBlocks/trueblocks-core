### Support for XDG Base Directory Spec

Source: [https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)

**XDG_DATA_HOME**: base directory for "user-specific data"
- We have no user-specific data in our desktop app -- one user, but if we want to support multi-user we should plan ahead
- Currently, this folder will remain usused
- defaults to $HOME/.local/share

**XDG_CONFIG_HOME**: base directory for "user-specific configuration files"
- All of our .toml files should be here
- This needs to be per chain, but some of the settings can be global (i.e. blockScraper configs should default to global settings, but allow for customization per chain)
- $HOME/.config

**XDG_STATE_HOME**: base directory for "user-specific state"
- Not sure what this is and how it differs from "user-specific data"
- $HOME/.local/state
- state that should persist between restarts, but not important enough to store in $XDG_DATA_HOME.
- could be logs, history, recently used files, application views, formats, etc.

There is no envoronment variable for where executable files should be placed, but it does mention a "single base directory for executables"
- Possible location $HOME/.local/bin, but not required
- Check on installation that this is in the $PATH

Other variables:
=================
**XDG_DATA_DIRS**: "preference ordered base directories where data files should be searched"
- entries should be seperated with a colon ':'.
- default SHOULD be /usr/local/share/:/usr/share/
- describes a path for searching -- duplicate information may be "first-in", "last-in", or "merged" at application specific
- XDG_DATA_HOME overrides any value found here

**XDG_CONFIG_DIRS**: "preference ordered base directories where configuration files should be searched."
- entries should be seperated with a colon ':'.
- default SHOULD be /etc/xdg
- describes a path for searching -- duplicate information may be "first-in", "last-in", or "merged" at application specific
- XDG_CONFIG_HOME overrides any value found here

**XDG_CACHE_HOME**: base directory for "user-specific non-essential (cached) data"
- default SHOULD be set to $HOME/.cache

**XDG_RUNTIME_DIR**: base directory for "non-essential, user-specific runtime files and other file objects" (i.e. extensions?)
- MUST be owned by the user
- MUST be the only one having read and write access to it
- MUST have Unix access mode of 0700
- MUST be bound to the user being logged in
- MUST be created when the user first logs in
- Files in the folder MUST be removed if the user logs out or the system reboots
- Same value should be presented if the user logs in again (although, empty)
- MUST be on a local file system
- MUST by fully-featured by the standards of the operating system.
- MAY be periodically cleaned-up.
- SHOULD report and error (and continue) if not set

Other:
=======
- All paths must be absolute (ignore if not)
- When trying to write, if a folder does not exist, it can be created, but...
  - create with 700 permissions
  - don't change permissions if the folder already exists
- When trying to read, if a folder doesn't exist or the file cannot be accessed
  - ignore the fact that it's missing and take defaults
  - if the file is required, it's okay to fall to defaults or quit
- SHOULD describe what happens if a file or value is found in multiple locations in the search path
  - for example, first-in-wins, last-in-wins, merge
