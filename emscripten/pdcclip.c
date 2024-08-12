/* Public Domain Curses */

#include "pdcemscripten.h"

RCSID("$Id: pdcclip.c,v 1.6 2008/07/14 04:24:52 wmcbrine Exp $")

#include <stdlib.h>
#include <string.h>

/*man-start**************************************************************

  Name:                                                         clipboard

  Synopsis:
        int PDC_getclipboard(char **contents, long *length);
        int PDC_setclipboard(const char *contents, long length);
        int PDC_freeclipboard(char *contents);
        int PDC_clearclipboard(void);

  Description:
        PDC_getclipboard() gets the textual contents of the system's 
        clipboard. This function returns the contents of the clipboard 
        in the contents argument. It is the responsibilitiy of the 
        caller to free the memory returned, via PDC_freeclipboard().
        The length of the clipboard contents is returned in the length 
        argument.

        PDC_setclipboard copies the supplied text into the system's 
        clipboard, emptying the clipboard prior to the copy.

        PDC_clearclipboard() clears the internal clipboard.

  Return Values:
        indicator of success/failure of call.
        PDC_CLIP_SUCCESS        the call was successful
        PDC_CLIP_MEMORY_ERROR   unable to allocate sufficient memory for 
                                the clipboard contents
        PDC_CLIP_EMPTY          the clipboard contains no text
        PDC_CLIP_ACCESS_ERROR   no clipboard support

  Portability                                X/Open    BSD    SYS V
        PDC_getclipboard                        -       -       -
        PDC_setclipboard                        -       -       -
        PDC_freeclipboard                       -       -       -
        PDC_clearclipboard                      -       -       -

**man-end****************************************************************/

int PDC_getclipboard(char **contents, long *length)
{
    char *str;
    size_t len;

    PDC_LOG(("PDC_getclipboard() - called\n"));

    str = EM_ASM_PTR({
        try {
            return stringToNewUTF8(await navigator.clipboard.readText());
        }
        catch (e) {
            return null;
        }
    });

    if (str == NULL) {
        return PDC_CLIP_ACCESS_ERROR;
    }

    len = strlen(str);
    if (len == 0) {
        free(str);
        return PDC_CLIP_EMPTY;
    }

    *contents = str;
    *length = (long)len;
    return PDC_CLIP_SUCCESS;
}

int PDC_setclipboard(const char *contents, long length)
{
    int ret;

    PDC_LOG(("PDC_setclipboard() - called\n"));

    ret = EM_ASM_INT({
        try {
            await navigator.clipboard.writeText($0);
            return 1;
        }
        catch (e) {
            return 0;
        }
    }, contents);

    return ret ? PDC_CLIP_SUCCESS : PDC_CLIP_ACCESS_ERROR;
}

int PDC_freeclipboard(char *contents)
{
    PDC_LOG(("PDC_freeclipboard() - called\n"));

    free(contents);

    return PDC_CLIP_SUCCESS;
}

int PDC_clearclipboard(void)
{
    int ret;

    PDC_LOG(("PDC_clearclipboard() - called\n"));

    ret = EM_ASM_INT({
        try {
            await navigator.clipboard.writeText("");
            return 1;
        }
        catch (e) {
            return 0;
        }
    });

    return ret ? PDC_CLIP_SUCCESS : PDC_CLIP_ACCESS_ERROR;
}
