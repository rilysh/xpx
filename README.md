### xpx
A small wrapper for the XBPS package manager

### Usage
Also available with `xpx -h/--help`
```
Usage:
   [-i|--install]          -- Install a package
   [-u|--uninstall]        -- Uninstall a package
   [-o|--remove-orphans]   -- Remove all orphan packages
   [-O|--clean-cache]      -- Remove all cached packages
   [-l|--list-pkgs]        -- List all installed packages
   [-L|--list-repos]       -- List all available repos
Other options:
   [-S|--sync]             -- Sync remote repos index
   [-R|--recursive]        -- Recursively select all packages when uninstall
Text aliases:
   install, install-sync, uninstall, uninstall-recs, clean-cache, clean-orphans, list-pkgs, list-repos
```
#### Examples
- `xpx -i sl` (Install without syncing the repositories)
- `xpx -Si sl` (Sync the repositories and install)
- `xpx -u sl` (Uninstall a package)
- `xpx -Ru sl` (Recursively uninstall a package, useful if programs are dynamically linked and install other dependencies)
- `xpx -o` (Remove orphan packages)
- `xpx -O` (Remove cached packages)
- `xpx -Oo` (Remove both cached and orphan packages)
#### Note
Optionally, you can use text aliases instead parameters. However, they've certain limitations. For example: `xpx install sl`

### Why?
Using XBPS from the ground up is nice, however, if you're coming from Debian's APT package manager, it's quite much cleaner in terms of usage than XBPS. Due to being separate binaries, XBPS can be a "little" overwhelming when you need the package manager most. Also, "XBPS" is quite an odd name for a package manager (though you can easily create an alias, still, all are separate programs).

### Aliases
If you don't want to use xpx, however, you can create aliases in your shells' init script.
```sh
alias pkg_install="xbps-install -S"
alias pkg_remove="xbps-remove -R"
alias pkg_cleanup="xbps-remove -Oo"
# And go on!
```
