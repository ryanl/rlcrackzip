/*
 * Performance-oriented stack-based replacement for std::vector.
 *
 * Copyright (C) 2012 Ryan Lothian.
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

#ifndef STATIC_VECTOR_H
#define STATIC_VECTOR_H

#define STATIC_VECTOR_NOINIT_HACK 0

#include "assert.h"
#include "boost/type_traits/alignment_of.hpp"
#include "boost/type_traits/type_with_alignment.hpp"

/*!
@class StaticVector

@brief This is a replacement for vector of elements of type T 
       when an upper-bound on the size of the array (n) is known 
       in advance.

StaticVector doesn't dynamically allocate memory. The idea is that working on the stack will be a little faster heap allocation. The downside is that StaticVector uses more memory than necessary if the actual size is less than the maximum. Also, you must be careful when allocating arrays on the stack not to run out of stack space.

StaticVector does not call constructors or destructors.

@author Ryan Lothian
*/


template <class T, unsigned int n>
class StaticVector {
private:
    unsigned int entries;

#if STATIC_VECTOR_NOINIT_HACK
    union dummy_u
    {
        char data[ sizeof(T) ];
        BOOST_DEDUCED_TYPENAME boost::type_with_alignment<
          ::boost::alignment_of<T>::value >::type aligner_;
    } _vec[n] ;
    T* vec;

#else
    T vec[n];
#endif

public:
        /* Copy constructor */
        StaticVector(const StaticVector<T, n>& other)  {
            this->entries = other.entries;
#if STATIC_VECTOR_NOINIT_HACK
            this->vec = (T*)(this->_vec);
#endif
            for (unsigned int i = 0; i < other.entries; i++) {
                this->vec[i] = other.vec[i];
            }
        }

	/*!
		Sets the number of entries to 0 (but does not call object destructors).
	*/
	void clear() {
		entries = 0;
	}

	/*!
		Does nothing except assert i <= n (if NDEBUG is not set). Provided for
		convenience so StaticVector can replace std::vector more easily.
	*/
	void reserve(unsigned int i) {
		assert(i <= n);	
	}

	/*!
		Creates an empty array.
	*/
	StaticVector(unsigned int entries=0) :
		entries(entries)
	{
            assert(entries <= n);
#if STATIC_VECTOR_NOINIT_HACK
	    vec = (T*)(_vec);
#endif
	}

        void resize(unsigned int new_size) {
            assert(new_size <= n);

            entries = new_size;
        }

	/*!
		Adds an element to the end of the array.
	*/
	void push_back(const T &t) {
		assert(entries < n);
		vec[entries] = t;
		entries++;
	}

	/*!
		Element access (const).
	*/
	const T& operator [] (unsigned int i) const {
		assert(i < entries);
		return vec[i];
	}

	/*!
		Element access.
	*/
	T& operator [] (unsigned int i) {
		assert(i < entries);
		return vec[i];
	}

	/*!
		Returns the number of entries of the array.
	*/
	unsigned int size() const {
		return entries;
	}

	// fast copy - don't have to copy parts of the array that aren't in use
	void operator = (const StaticVector<T,n>& other) {
	    entries = other.entries;
	    for (unsigned int i = 0; i < entries; i++) {
	        vec[i] = other[i];
	    }
	}
};

#endif

