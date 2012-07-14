/*
 *  Copyright (C) 2012 Ryan Lothian and Marc Lehmann. See AUTHORS file.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#include "generators.h"
#include "zip_read.h"
#include "zip_crack.h"

/*
 * Add all characters in string 's' to the end of 'v'.
 */
static void
add_chars (StaticVector<char, 256> * v,
           const char *              s)
{
    for (uint32_t j = 0; s[j] != '\0'; j++) {
        v->push_back(s[j]);
    }
}


/*
 * Format: [aA1!]+(:.+)?
 */
static StaticVector<char, 256>
parse_charset (const std::string &encoded_cs)
{
    StaticVector<char, 256>  charset;
    bool                     seen_colon = false;

    for (uint32_t i = 0; i < encoded_cs.length(); i++) {
        char c = encoded_cs[i];

        if (seen_colon) {
            charset.push_back(c);
        } else {
            switch (c) {
            case 'a':
                add_chars(&charset, "abcdefghijklmnopqrstuvwxyz");
                break;

            case 'A':
                add_chars(&charset, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
                break;

            case '1':
                add_chars(&charset, "0123456789");
                break;

            case '!':
                add_chars(&charset, "!:$%&/()=?{[]}+-*~#");
                break;

            default:
                fprintf(stderr, "Unknown charset specifier, only 'aA1!:' recognized\n");
                exit(1);
            }
        }
    }
    return charset;
}


/*
 * main
 */
int
main (int argc, char *argv[])
{
    int                          rc;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <zip file name>\n";
        rc = 1;
    } else {
        StaticVector<char, 256>      bfg_charset = parse_charset("a");
        uint64_t                     count      = 27 * 27 * 27 * 27 * 27 * 27;
        uint64_t                     chunk_size = 1000000;
        uint64_t                     chunks     = 1 + ((count - 1) / chunk_size);
        PasswordCollectorPrint       pwc;
        DecodeChecker                dc;
        std::vector<file_info_type>  files = load_zip(argv[1]);

        time_t start_time = time(NULL);

        #pragma omp parallel for schedule(dynamic)
        for (uint64_t i = 0; i < chunks; i++) {
             #pragma omp critical
             std::cout << i << " " << (i * chunk_size) << " " << chunk_size << "\n";

             BruteforceGenerator  bfg(bfg_charset, i * chunk_size, chunk_size);
//            OnePasswordGenerator spg("noradi");
            crack_zip_password(files, bfg, dc, pwc);
        }

        std::cout << count / (time(NULL) - start_time) << " passwords checked per second (total " << count << ").\n";

        rc = 0;
    }

    return rc;
}

