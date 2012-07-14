/*
 * rlcrackzip - Zip password cracker.
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

/*
 * This module provides classes for generating passwords.
 *
 * So far the following generators exist:
 * - brute force
 * - word list   (future)
 */

#include <cstdlib>
#include <stdint.h>
#include <assert.h>

#include "generators.h"
#include "optimization.h"


OnePasswordGenerator::OnePasswordGenerator(std::string  password) :
    password(password),
    done(false)
{
}


bool
OnePasswordGenerator::isDone() const
{
    return this->done;
}


void
OnePasswordGenerator::next(StaticVector<char, MAX_PW_LEN>  *pw_out,
                           uint32_t                        *same_chars)
{
    pw_out->resize(this->password.length());
    for (uint32_t i = 0; i < this->password.length(); i++) {
        (*pw_out)[i] = this->password[i];
    }
    *same_chars = 0;
    this->done = true;
}



/*
 * BruteforceGenerator
 */
bool
BruteforceGenerator::isDone() const
{
    return (this->count_generated >= this->count_requested);
//    return this->state.size() == 5;
}


/* start_index and gen_count are used for distributing work */
BruteforceGenerator::BruteforceGenerator(const StaticVector<char, 256>&  charset,
                                         uint64_t                        start_index,
                                         uint64_t                        gen_count)
 
{
    assert(gen_count > 0);

    // TODO could make the map do the +1 instead

    this->charset = charset;
    this->char_map.resize(256);
    for (uint32_t i = 0; i < charset.size(); i++) {
       this->char_map[charset[i]] = i;
    }

    this->start_index     = start_index;
    this->count_generated = 0;
    this->count_requested = gen_count;
}

void BruteforceGenerator::next(StaticVector<char, MAX_PW_LEN>  *pw_out,
                               uint32_t                        *same_chars)
{
    if (UNLIKELY(this->count_generated == 0)) {
        uint64_t combinations = 1;

        /* First is special case. State is already set and pw_out's value is unknown. */
        pw_out->clear();
        /* Work out how long the first password generated should be */
        uint32_t pw_length;

        for (pw_length = 0; this->start_index >= combinations; pw_length++) {
            this->start_index -= combinations;
            combinations *= this->charset.size();
        }

        pw_out->resize(pw_length);
        /* Work out the first password */
        for (int32_t i = pw_length - 1; i >= 0; i--) {
            (*pw_out)[i] = this->charset[i % this->charset.size()];
            i /= this->charset.size();
        }
        *same_chars = 0;

    } else {

        /* Next combination */
        *same_chars = pw_out->size();
        int32_t i;

        for (i = pw_out->size() - 1; i >= 0; i--) {
             uint32_t  char_val = this->char_map[(*pw_out)[i]];
             *same_chars -= 1;
             if (char_val < this->charset.size() - 1) {
                 (*pw_out)[i] = this->charset[char_val + 1];
                 break;
             } else {
                 (*pw_out)[i] = this->charset[0];
             }
        }

        /* If we need to lengthen the key then do so. Note that all the other members will be 0 already. */
        if (UNLIKELY(i < 0)) {
            pw_out->push_back(this->charset[0]);
        }
    }
    this->count_generated++;
}

