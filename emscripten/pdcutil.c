/* Public Domain Curses */

#include <emscripten.h>

#include "pdcemscripten.h"

RCSID("$Id: pdcutil.c,v 1.6 2008/07/14 04:24:52 wmcbrine Exp $")

void PDC_beep(void)
{
    PDC_LOG(("PDC_beep() - called\n"));
    EM_ASM((function () {
        let started = false;

        if (!this.emcurses_beep) {
            this.emcurses_beep = {
                ctx: null,
                os: null,
                playing: false,
                queue: 0
            };
        }

        // I know it's aesthetically challenged!
        try {
            if (this.emcurses_beep.playing) {
                if (this.emcurses_beep.queue < 10) {
                    this.emcurses_beep.queue += 1;
                }
            }
            else {
                this.emcurses_beep.ctx = new AudioContext();
                os = this.emcurses_beep.os = this.emcurses_beep.ctx.createOscillator();

                this.emcurses_beep.os.type = "sine";
                this.emcurses_beep.os.frequency.value = 1000;
                this.emcurses_beep.os.connect(
                    this.emcurses_beep.ctx.destination);
                this.emcurses_beep.os.start();
                this.emcurses_beep.playing = started = true;

                function atend () {
                    this.emcurses_beep.os.stop();

                    setTimeout(function () {
                        if (this.emcurses_beep.queue > 0) {
                            let started = false;

                            try {
                                this.emcurses_beep.queue -= 1;
                                this.emcurses_beep.os =
                                    this.emcurses_beep.ctx.createOscillator();
                                this.emcurses_beep.os.type = "sine";
                                this.emcurses_beep.os.frequency.value = 1000;
                                this.emcurses_beep.os.connect(
                                    this.emcurses_beep.ctx.destination);
                                this.emcurses_beep.os.start();
                                started = true;
                                setTimeout(atend, 200);
                            }
                            catch (e) {
                                // don't want the sound to stuck
                                console.error(e);

                                if (started) {
                                    this.emcurses_beep.os.stop();
                                    this.emcurses_beep.ctx.close();
                                }
                                this.emcurses_beep.ctx = null;
                                this.emcurses_beep.os = null;
                                this.emcurses_beep.playing = false;
                                this.emcurses_beep.queue = 0;
                            }
                        }
                        else {
                            this.emcurses_beep.ctx.close();
                            this.emcurses_beep.ctx = null;
                            this.emcurses_beep.os = null;
                            this.emcurses_beep.playing = false;
                        }
                    }, 200);
                }

                setTimeout(atend, 200);
            }
        }
        catch (e) {
            console.error(e);
            if (started) {
                // don't want the sound to stuck
                this.emcurses_beep.os.stop();
                this.emcurses_beep.ctx.close();
            }
            this.emcurses_beep.ctx = null;
            this.emcurses_beep.os = null;
            this.emcurses_beep.playing = false;
            this.emcurses_beep.queue = 0;
        }
    })());
}

void PDC_napms(int ms)
{
    /* This is a synchronous sleep, which isn't normally possible with
     * Emscripten. Code using it must be compiled with -s EMTERPRETIFY=1
     * -s EMTERPRETIFY_ASYNC=1, and all functions in the call graph leading
     * to this sleep must use the Emterpreter. For more information, see:
     * https://github.com/kripken/emscripten/wiki/Emterpreter
     */
    emscripten_sleep(ms);
    PDC_LOG(("PDC_napms() - called: ms=%d\n", ms));
}

const char *PDC_sysname(void)
{
    return "emscripten";
}
