/*
 * rlcrackzip - Zip password cracker.
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

#ifndef GENERATORS_H
#define GENERATORS_H

#include <string>
#include <vector>

#include "static_vector.h"
#include "stdint.h"


#define MAX_PW_LEN 200

class GeneratorInterface
{
public:

    virtual bool isDone() const = 0;

    /*
     * @pw_out      New password                                 (OUT)
     *
     * @same_chars  Number of leading characters in common with
     *              the previous password.                       (OUT)
     */
    virtual void next(StaticVector<char, MAX_PW_LEN>  *pw_out,
                      uint32_t                        *same_chars) = 0;
};


/*
 * Rather than handling files here directly, this just works with memory
 * for speed, and to allow the complexities of splitting a wordlist
 * among several threads to be kept out of the innards.
 */
class MemoryWordlistGenerator : public GeneratorInterface
{
private:
    const char  *wordlist;
    const char  *old_word_start;

public:
    /*
     * wordlist must be null-terminated.
     */
    MemoryWordlistGenerator(const char *wordlist);
    bool isDone() const;
    void next(StaticVector<char, MAX_PW_LEN>  *pw_out,
              uint32_t                        *same_chars);
};


/*
 * Good for testing.
 */
class OnePasswordGenerator : public GeneratorInterface
{
private:
    std::string  password;
    bool         done;
public:
    OnePasswordGenerator(std::string  password);
    bool isDone() const;
    void next(StaticVector<char, MAX_PW_LEN>  *pw_out,
              uint32_t                        *same_chars);
};


class BruteforceGenerator : public GeneratorInterface
{
    StaticVector<char, 256>        charset;
    StaticVector<uint32_t, 256>    char_map;
    uint64_t                       count_requested;
    uint64_t                       count_generated;
    uint64_t                       start_index;
public:
    bool isDone() const;

    /* start_index and gen_count are used for distributing work */
    BruteforceGenerator(const StaticVector<char, 256>&  charset,
                        uint64_t                        start_index=0,
                        uint64_t                        gen_count=(uint64_t)(-1));

    void next(StaticVector<char, MAX_PW_LEN>  *pw_out,
              uint32_t                        *same_chars);
};


#endif /* GENERATORS_H */

