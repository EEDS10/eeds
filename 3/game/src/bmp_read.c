#include <stdio.h>
#include <stdlib.h>

#include "bmp_read.h"

bitmap_t *load_bmp(FILE *bmp) {
    int i, x, y, bytes;
    unsigned char *file_buffer;
    unsigned char header[54];
    bitmap_t *bitmap = malloc(sizeof(*bitmap));

    fread(header, sizeof(*header), 54, bmp);

    bitmap->width = *((int *) &(header[18]));
    bitmap->height = abs(*((int *) &(header[22])));

    bitmap->bitmap = malloc(sizeof(*bitmap->bitmap) * bitmap->height);
    for (i = 0; i < bitmap->height; i++) {
        bitmap->bitmap[i] = malloc(sizeof(**bitmap->bitmap) * bitmap->width);
    }

    bytes = bitmap->width * bitmap->height * 4;
    file_buffer = malloc(sizeof(*file_buffer) * bytes);
    fread(file_buffer, sizeof(*file_buffer), bytes, bmp);

    for (i = 0, x = 0, y = 0; i < bytes;) {
        bitmap->bitmap[x][y].blue = file_buffer[i++] & 255;
        bitmap->bitmap[x][y].green = file_buffer[i++] & 255;
        bitmap->bitmap[x][y].red = file_buffer[i++] & 255;
        i++;

        if (++y == bitmap->width) {
            x++;
            y = 0;
        }
    }

    free(file_buffer);

    return bitmap;
}

void free_bitmap(bitmap_t *bitmap) {
    for (int i = 0; i < bitmap->height; i++) {
        free(bitmap->bitmap[i]);
    }

    free(bitmap->bitmap);
    free(bitmap);
}
