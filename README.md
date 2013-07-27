libsdl_extras_interposer
========================

A library interposer that hooks SDL to provide additional features on Linux

This library interposer may be used in the following fashion:

$ export SDL_FS_TOGGLE=1
$ export SDL_FS_TOGGLE_NO_UNGRAB
$ export SDL_HDB_COLORKEY_FIX=1
$ LD_PRELOAD=/absolute/path/to/libsdl_extras.so <program-name>

SDL_FS_TOGGLE: defaults to off.  Set to 1 to enable. Allows toggling
fullscreen with alt-enter.  Good for programs like Neverwinter Nights
1 that do not normally support in-game fullscreen toggle.

SDL_FS_TOGGLE_NO_UNGRAB: defaults to off.  Set to "1" to enable.  When
toggling fullscreen, we normally ungrab the mouse so that the mouse
can leave the window.  This will cause the mouse to remain grabbed.
Useful for programs where it would be nice to enter a windowed mode
but leave the mouse constrained to the window.

SDL_HDB_COLORKEY_FIX: defaults to off.  Set to "1" to enable.  If this
is set, all attempts to use SDL_SetColorKey with the value 0xff00ff
will instead use 0xf800f8.  Fixes the Hyperspace Delivery Boy problem
when using newer SDLs that causes all sprites to have bright magenta
areas instead of transparent area.

Both a 32- and 64- bit library are built, as a number of closed-source
games have only 32-bit versions.