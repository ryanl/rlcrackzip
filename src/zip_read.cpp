/*
 * rlcrackzip - Zip password cracker.
 * This file parses zip files.
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


#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "file_access.h"
#include "assert.h"
#include "zip_read.h"


#define verbosity 0

/*
 * decode_zip_file_header
 *
 * @f          file
 * @file_info  any info needed for decrypting (out)
 */
bool
decode_zip_file_header (FILE            *f,
                        file_info_type  *file_info)
{
    bool    ok = true;

    file_info->version            = fgetu16(f);
    file_info->flags              = fgetu16(f);
    file_info->compression_method = fgetu16(f);
    file_info->lastmodtime        = fgetu16(f);
    file_info->lastmoddate        = fgetu16(f);
    file_info->crc32              = fgetu32(f);
    file_info->compressed_size    = fgetu32(f);
    file_info->uncompressed_size  = fgetu32(f);
    uint16_t name_len             = fgetu16(f); // not in file_info as std::string stores length anyway
    file_info->extra_field_len    = fgetu16(f);

    size_t   count_read = 0;
    char     member_path[65537]; // big enough

    /* these are unused.  */
    assert(name_len < sizeof(member_path));

    count_read = fread(member_path, 1, name_len, f);
    assert(count_read == name_len);
    (void)count_read; // prevent unused variable warning when asserts are disabled

    member_path[name_len] = 0;
    file_info->file_path = std::string(member_path);

    /* Skip over the extra field */
    fseek(f, file_info->extra_field_len, SEEK_CUR);

    if ((file_info->flags & ZIP_FLAG_ENCRYPTED) != 0) {
        if (file_info->compressed_size < 12) {
            std::cerr << "Data for member '" << member_path << " is less than minimum 12 bytes. Bailing out.\n";
            ok = false;
        } else {
//            uint32_t  count_to_read = std::min<unsigned int>(FILE_READ_SIZE, file_info->compressed_size);

            file_info->d.clear();
            for (uint32_t i = 0; i < file_info->compressed_size; i++) {
                 int c = fgetc(f);
                 assert(c != EOF);
                 file_info->d.push_back(c);
            }
            file_info->compressed_size -= 12; // ignore leading 12 bytes
        }
    } else {
        ok = false;
        std::cout << "Member '" << member_path << " is not encrypted, skipping.\n";
        fseek(f, file_info->compressed_size, SEEK_CUR);
    }

    return ok;
}


/*
 * Read zip file from disk.
 *
 * @path path to a zip file
 * Return: info about all files found in the zip file
 */
std::vector<file_info_type>
load_zip (const char *path)
{
    bool                         ok = true;
    std::vector<file_info_type>  files;
    FILE                        *f;

    f = fopen(path, "rb");
    if (f == NULL) {
        fprintf(stderr, "Could not open '%s': %s\n", path, strerror(errno));
        ok = false;
    }

    while (ok && !feof(f)) {
        uint32_t segment_id = fgetu32(f);  // indicates type of segment

        /* Header for a file contained within the zip file */
        if (segment_id == 0x04034b50UL) {
            file_info_type  file_info;
            ok = decode_zip_file_header(f, &file_info);

            if (ok) {
                files.push_back(file_info);
            }
        } else if (segment_id == 0x08074b50UL) {	/* extended local sig (?)  */
            fseek(f, 12, SEEK_CUR);

        } else if (segment_id == 0x30304b50UL) {
            /* ignore */

        } else if (segment_id == 0x02014b50UL || segment_id == 0x06054b50UL) {
            ok = false; // @@@ why? is this an end of file marker?

        } else {
            fprintf(stderr, "found id %08x, '%s' is not a zipfile ver 2.xx, skipping\n",
                    segment_id, path);
            ok = false;
        }
    }

    if (f != NULL) {
        fclose(f);
        f = NULL;
    }

    return files;
}

