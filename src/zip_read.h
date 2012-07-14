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

#ifndef ZIP_READ_H
#define ZIP_READ_H

#include <stdint.h>
#include <string>
#include <vector>
#include "static_vector.h"

#define FILE_READ_SIZE 12 // could increase in future for specialized decoders


// future: check decompressed bytes against type
typedef enum {
    FILE_TYPE_ASCII,     // expect all characters < 128
    FILE_TYPE_JPEG,      // expect a jpeg or exif file header
    FILE_TYPE_UNKNOWN    // not sure what to expect
} file_type_classification_type;

/*
 * Struct containing everything useful about a member of a zip file.
 */
typedef struct {
    //StaticVector<uint8_t, FILE_READ_SIZE> d;
    std::vector<uint8_t>                  d;
    std::string                           file_path;
    uint16_t                              version;
    uint16_t                              flags;
    uint16_t                              compression_method;
    uint16_t                              lastmodtime;
    uint16_t                              lastmoddate;
    uint32_t                              crc32;
    uint32_t                              compressed_size;
    uint32_t                              uncompressed_size;
    uint16_t                              extra_field_len;
    file_type_classification_type         file_type;
} file_info_type;


#define ZIP_FLAG_ENCRYPTED          0x1
#define ZIP_FLAG_ENDS_WITH_MODTIME  0x8


/*
 * decode_zip_file_header
 *
 * @f          file
 * @file_info  any info needed for decrypting (out)
 */
bool decode_zip_file_header(FILE            *f,
                            file_info_type  *file_info);

/*
 * Read zip file from disk.
 *
 * @path path to a zip file
 * Return: info about all files found in the zip file
 */
std::vector<file_info_type> load_zip(const char *path);


#endif /* ZIP_READ_H */
