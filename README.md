# tup, without FUSE, working around partial locks & inotify

[![Maintainer status](https://img.shields.io/badge/maintained%3F-no!-red.svg?style=flat)](https://twitter.com/nilium/status/894671737266163712)

This fork modifies tup to run properly under WSL (Windows Subsystem for Linux)

In particular:

  * It goes back to the ldpreload approach, since there's no FUSE support on WSL right now
    * Instead of using unix sockets, it uses temporary files
      (this is because of the master_fork architcture in recent tups)
    * Only 64-bit is supported (the lib isn't dual-built), but more could be
  * It uses `flock` instead of `fcntl` to lock files, since the latter has incorrect behavior on WSL
    * See <https://github.com/fasterthanlime/locktest> for details
  * It works around partial inotify support on WSL:
    * Instead of reacting to OPEN and CLOSE events on `.tup/object`, it
    uses two files: `.tup/object` and `.tup/object_lock`, and touches them
    (which inotify-on-WSL detects)
  * Oh, also it uses Makefiles to build itself, which is terrible and a sin,
    but it was a lot faster to iterate with that than `./build-nofuse.sh`

It's a whole *pile* of hacks, so, **DON'T USE IT**. And if you *do* use it,
you accept that I'm not going to support any of it. Not planning on accepting PRs etc.

I just have too many projects going on, sorry!

## About Tup

Tup is a file-based build system for Linux, OSX, and Windows. It takes
as input a list of file changes and a directed acyclic graph (DAG). It
then processes the DAG to execute the appropriate commands required to
update dependent files. Updates are performed with very little overhead
since tup implements powerful build algorithms to avoid doing
unnecessary work. This means you can stay focused on your project rather
than on your build system.

Further information can be found at http://gittup.org/tup
