# EMCurses: Terminate your Browser!

EMCurses is a port of [PDCurses](http://pdcurses.sourceforge.net/) to WASM. It
is not merely PDCurses/SDL compiled with Emscripten - which cannot be done
easily anyway - but a distinct port that uses the
[termlib](http://www.masswerk.at/termlib/) Javascript library for output.

In essence, EMCurses helps you in porting your favourite terminal-based
applications to the Web platform; or in designing Web pages in the environment
you are accustomed to: the terminal and C programming language.

EMCurses is a work in progress and only a quick hack so far. It's already
functional, though.

Run demos here: https://dxdxdt.github.io/emscripten/pdcurses-demos/

## Installation
Set up your emsdk env vars(`source .../emsdk_env.sh`) and then run:

```sh
cd emscripten/
./build-cmake.sh
./install-cmake.sh
```

These files will be installed in `$EMSDK/upstream/emscripten/cache/sysroot`:

- include/curses.h
- lib/libemcurses.a
- lib/emcurses/termlib.js
- lib/pkgconfig/emcurses.pc

Link `libemcurses.a` to your WASM module. `termlib.js` can either be embedded to
your WASM module or loaded in the page using `<script>`. To embed `termlib.js`,

```sh
emcc \
    module.c \
    -o module.js \
    --pre-js "$EMSDK/upstream/emscripten/cache/sysroot/lib/emcurses/termlib.js" \
    "$EMSDK/upstream/emscripten/cache/sysroot/lib/libemcurses.a"
```

You may use termlib.js from the original author:
https://www.masswerk.at/termlib/
