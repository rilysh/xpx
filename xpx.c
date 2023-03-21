#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "xpx.h"

// TODO: Write a readme
static void xpx_cmd_init(void)
{
    args[0] = SHELL_PATH;
    args[1] = "-c";
    args[2] = pkg_pack;
    args[3] = NULL;

    memset(pkg_pack, '\0', sizeof(pkg_pack));
}

static void xpx_install_pkg(char *pkg, int sync)
{
    if (sync)
        strcat(pkg_pack, XBPS_INSTALL_SYNC);
    else
        strcat(pkg_pack, XBPS_INSTALL);

    strcat(pkg_pack, pkg);
    execv(SHELL_PATH, args);
}

static void xpx_uninstall_pkg(char *pkg, int recursive)
{
    if (recursive)
        strcat(pkg_pack, XBPS_REMOVE_RECURSVIVE);
    else
        strcat(pkg_pack, XBPS_REMOVE);

    strcat(pkg_pack, pkg);
    execv(SHELL_PATH, args);
}

static void xpx_remove_cache_or_orphans(int cache, int orphans)
{
    if (cache)
        strcat(pkg_pack, XBPS_REMOVE_CACHE);
    if (orphans)
        strcat(pkg_pack, XBPS_REMOVE_ORPHANS);

    execv(SHELL_PATH, args);
}

static void xpx_list_pkgs_or_repos(int repos, int pkgs)
{
    if (repos)
        strcat(pkg_pack, XBPS_QUERY_REPOS);
    if (pkgs)
        strcat(pkg_pack, XBPS_QUERY_PKGS);

    execv(SHELL_PATH, args);
}

static char *clear_args(int len, char **arg)
{
    char *buf = malloc(ARG_SIZE);
    int i;

    memset(buf, '\0', sizeof(&buf));

    for (i = 2; i < len; i++) {
        if (i != 2 && strcmp(arg[i], " ") != 0)
            strcat(buf, " ");
        strcat(buf, arg[i]);
    }

    return buf;
}

static void help(void)
{
    fprintf(stdout,
        "xpx - A package wrapper for XBPS\n\n"
        "Usage:\n"
        "   [-i|--install]          -- Install a package\n"
        "   [-u|--uninstall]        -- Uninstall a package\n"
        "   [-o|--remove-orphans]   -- Remove all orphan packages\n"
        "   [-O|--clean-cache]      -- Remove all cached packages\n"
        "   [-l|--list-pkgs]        -- List all installed packages\n"
        "   [-L|--list-repos]       -- List all available repos\n"
        "Other options:\n"
        "   [-S|--sync]             -- Sync remote repos index\n"
        "   [-R|--recursive]        -- Recursively select all packages when uninstall\n"
        "Text aliases:\n"
        "   install, install-sync, uninstall, uninstall-recs"
        ", clean-cache, clean-orphans, list-pkgs, list-repos\n"
    );
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        help();
        exit(EXIT_SUCCESS);
    }

    char *buf = clear_args(argc, argv);

    xpx_cmd_init();

    // Looks kinda messy, maybe someday I'll clean this up
    if (argv[1][0] != '-') {
        if (strcmp(argv[1], "install") == 0)
            if (argc < 3)
                goto lack_args;
            else
                xpx_install_pkg(buf, 0);
        else if (strcmp(argv[1], "install-sync") == 0)
            if (argc < 3)
                goto lack_args;
            else
                xpx_install_pkg(buf, 1);
        else if (strcmp(argv[1], "uninstall") == 0)
            if (argc < 3)
                goto lack_args;
            else
                xpx_uninstall_pkg(buf, 0);
        else if (strcmp(argv[1], "uninstall-recs") == 0)
            if (argc < 3)
                goto lack_args;
            else
                xpx_uninstall_pkg(buf, 1);

        /* Commands which have no arguments */
        else if (strcmp(argv[1], "clean-cache") == 0)
            xpx_remove_cache_or_orphans(1, 0);
        else if (strcmp(argv[1], "clean-orphans") == 0)
            xpx_remove_cache_or_orphans(0, 1);
        else if (strcmp(argv[1], "list-pkgs") == 0)
            xpx_list_pkgs_or_repos(0, 1);
        else if (strcmp(argv[1], "list-repos") == 0)
            xpx_list_pkgs_or_repos(1, 0);
        else
            if (argc > 2)
                goto unknown;

        free(buf);
        exit(EXIT_SUCCESS);

    lack_args:
        fprintf(stderr, "Error: Missing arguments\n");
        free(buf);
        exit(EXIT_FAILURE);

    unknown:
        fprintf(stderr, "Error: Unknown command name \"%s\"\n", argv[1]);
        free(buf);
        exit(EXIT_FAILURE);
    }

    int idx;
    int sync_opt = 0, install_opt = 0, uninstall_opt = 0,
    recursive_opt = 0, clean_cache_opt = 0, remove_orphans_opt = 0;
    const char *opts = "SROoLlihu:";

    struct option long_options[] = {
        { "sync", no_argument, 0, 'S' },
        { "recursive", no_argument, 0, 'R' },
        { "install", required_argument, 0, 'i' },
        { "uninstall", required_argument, 0, 'u' },
        { "clean-cache", no_argument, 0, 'O' },
        { "remove-orphans", no_argument, 0, 'o' },
        { "list-pkgs", no_argument, 0, 'l' },
        { "list-repos", no_argument, 0, 'L' },
        { "help", no_argument, 0, 'h' }
    };

    opterr = 0; /* Disable error messages */

    while ((idx = getopt_long(argc, argv, opts, long_options, NULL)) != -1) {
        switch (idx) {
        case 'i':
            install_opt = 1;
            break;

        case 'u':
            uninstall_opt = 1;
            break;

        case 'S':
            sync_opt = 1;
            break;

        case 'R':
            recursive_opt = 1;
            break;

        case 'O':
            clean_cache_opt = 1;
            break;

        case 'o':
            remove_orphans_opt = 1;
            break;

        case 'l':
            xpx_list_pkgs_or_repos(0, 1);
            break;

        case 'L':
            xpx_list_pkgs_or_repos(1, 0);
            break;

        case 'h':
            help();
            goto cleanup;

        default:
            /* Keep it open to allow arguments which will be passed
            through XBPS */
            break;
        }
    }

    if ((install_opt || uninstall_opt) && argc < 3) {
        help();
        goto cleanup;
    }

    if (install_opt) {
        xpx_install_pkg(buf, sync_opt);
    } else if (uninstall_opt) {
        xpx_uninstall_pkg(buf, recursive_opt);
    }

    if (clean_cache_opt)
        xpx_remove_cache_or_orphans(clean_cache_opt, remove_orphans_opt);
    else if (remove_orphans_opt)
        xpx_remove_cache_or_orphans(clean_cache_opt, remove_orphans_opt);
    else if (clean_cache_opt && remove_orphans_opt)
        xpx_remove_cache_or_orphans(clean_cache_opt, remove_orphans_opt);

cleanup:
    free(buf);
    exit(EXIT_SUCCESS);
}
