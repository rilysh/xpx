#ifndef XPX_H
#define XPX_H

#ifndef ARG_SIZE
#define ARG_SIZE	512
#endif

#define SHELL_PATH				"/usr/bin/sh"
#define XBPS_INSTALL			"xbps-install "
#define XBPS_INSTALL_SYNC		"xbps-install -S "
#define XBPS_REMOVE				"xbps-remove "
#define XBPS_REMOVE_RECURSVIVE	"xbps-remove -R "
#define XBPS_REMOVE_ORPHANS		"xbps-remove -o "
#define XBPS_REMOVE_CACHE		"xbps-remove -O "
#define XBPS_QUERY_PKGS			"xbps-query -l "
#define XBPS_QUERY_REPOS		"xbps-query -L "

static char pkg_pack[ARG_SIZE];
static char *args[4];

#endif
