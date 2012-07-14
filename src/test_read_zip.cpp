/*
 * rlcrackzip - Zip password cracker.
 * This test program displays info about a zip file.
 *
 * Copyright (C) 2012 Ryan Lothian (see AUTHORS file).
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



#include <iostream>
#include <vector>

#include "zip_read.h"

int
main (int   argc,
      char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <zipfile name>\n";
        return 1;
    }

    std::vector<file_info_type>  members = load_zip(argv[1]);

    for (uint32_t i = 0; i < members.size(); i++) {
        std::cout << "Member:             " << members[i].file_path << "\n"
                  << "Version:            " << members[i].version   << "\n"
                  << "Flags:              " << members[i].flags     << "\n"
                  << "Compression method: " << members[i].compression_method << "\n"
                  << "Last mod:           " << members[i].lastmodtime << " " << members[i].lastmoddate << "\n"
                  << "CRC32:              " << members[i].crc32 << "\n"
                  << "Compressed size:    " << members[i].compressed_size << "\n"
                  << "Uncompressed size:  " << members[i].uncompressed_size << "\n"
                  << "Extra field len:    " << members[i].extra_field_len << "\n"
                  << "Leading bytes:      ";
        for (uint32_t  j = 0; j < 12; j++) {
            std::cout << static_cast<uint32_t>(members[i].d[j]) << " ";
        }
        std::cout << "\n\n";
    }

    return 0;
}

