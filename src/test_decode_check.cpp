/*
 * rlcrackzip - Zip password/decode checking test program.
 *
 * Copyright (C) 2012 Ryan Lothian (see AUTHORS file).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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


#include <stdint.h>
#include "zip_crack.h"


/*
 * Unit tests for the DecodeCheckers.
 */
int main (int   argc,
          char *argv[])
{
    /* Decoded bytes from noradi.zip */
    uint32_t                  db[12] = {37, 69, 43, 227, 152, 48, 192, 143, 213, 60, 173, 210};
    StaticVector<uint8_t, FILE_READ_SIZE> decoded_bytes(12);

    /* Move to StaticVector  structure */
    for (uint32_t  i = 0; i < 12; i++) {
        decoded_bytes[i] = db[i];
    }

    file_info_type  file;
    file.crc32              = 3530965632U;
    file.version            = 20;
    file.flags              = 1;
    file.compression_method = 8;
    file.lastmodtime        = 42244;
    file.lastmoddate        = 9325;
    file.compressed_size    = 104;
    file.uncompressed_size  = 110;

    DecodeChecker   dc;

    bool  check_result = dc.check(file, decoded_bytes);

    std::cout << check_result << "\n";
    return check_result ? 0 : 1;
}
