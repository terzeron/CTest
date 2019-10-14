#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ft2build.h>
#include FT_FREETYPE_H

int main(int argc, char *argv[])
{
    int i;
    int j;
    FT_Library library;
    FT_Face face;
    int error;

    if (FT_Init_FreeType(&library) < 0) {
        fprintf(stderr, "can't initialize freetype, %d: %s\n", errno,
                strerror(errno));
        return -1;
    }

    for (i = 1; i < argc; i++) {
        j = 0;
    restart:
        error = FT_New_Face(library, argv[i], j, &face);
        switch (error) {
        case FT_Err_Cannot_Open_Resource:
            j++;
            goto restart;
        case FT_Err_Unknown_File_Format:
            fprintf(stderr, "unknown file format, %d: %s\n",
                    errno, strerror(errno));
            return -1;
        case FT_Err_Invalid_File_Format:
            fprintf(stderr, "invalid file format, %d: %s\n",
                    errno, strerror(errno));
            return -1;
        case 0:
            break;
        default:
            fprintf(stderr, "error=%d\n", error);
            return -1;
        }
        printf("%s: %s-%s\n", argv[i], face->family_name,
               face->style_name);
        FT_Done_Face(face);
    }

    return 0;
}
