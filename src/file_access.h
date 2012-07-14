/*
 * rlcrackzip - Zip password cracker.
 *
 * Copyright (C) 2012 Ryan Lothian and Marc Lehmann (see AUTHORS file).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */


#ifndef FILE_ACCESS_H
#define FILE_ACCCES_H

#include <cstdio>
#include <stdint.h>


/*
 * Read a little-endian uint32_t from a file.
 */
inline uint32_t
fgetu32 (FILE *f)
{
    uint32_t r;

    r  = fgetc(f);
    r += fgetc(f) <<  8;
    r += fgetc(f) << 16;
    r += fgetc(f) << 24;

    return r;
}


/*
 * Read a little-endian uint16_t from a file.
 */
inline uint32_t
fgetu16 (FILE *f)
{
    uint32_t r;

    r  = fgetc(f);
    r |= fgetc(f) <<  8;

    return r;
}


#endif /* ifndef FILE_ACCESS_H */
