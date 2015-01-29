/*
 * Copyright (c) 2006-2015 David Schor (davidschor@zigwap.com), ZigWap LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <unistd.h>
#include <stdlib.h>
#include "ctest.h"
#include "aleph.h"

CTEST(Categories, check_cat)
{
    int i;
    a_str a;
    
    a = a_new("test");
    
    /* 4 bytes each cat */
    for (i = 0; i < 1000; ++i)
        a = a_cat(a, "test");
    
    ASSERT_EQUAL(4004, a_len(a));
    ASSERT_EQUAL(4004, a_size(a));

    /* 3-bytes each */
    a = a_cat(a, "ⒶⒷⒸ");
    ASSERT_EQUAL(4004+3, a_len(a));
    ASSERT_EQUAL(4004+9, a_size(a));
    
    /* 2-bytes each */
    a = a_cat(a, "אבגד");
    ASSERT_EQUAL(4004+3+4, a_len(a));
    ASSERT_EQUAL(4004+9+8, a_size(a));
    
    a_free(a);
}

CTEST(Categories, check_cat_chr)
{
    int i;
    a_str a;
    
    a = a_new("test");
    
    /* should only concat the FIRST character ONLY */
    a = a_cat_chr(a, "∜∜∛∛");
    a = a_cat_chr(a, "∛∛∜∜");
    
    ASSERT_EQUAL(4+1+1, a_len(a));
    ASSERT_EQUAL(4+3+3, a_size(a));
    
    a = a_cat_chr(a, "ⒶⒷⒸ");
    a = a_cat_chr(a, "ⒷⒸⒶ");
    a = a_cat_chr(a, "ⒸⒶⒷ");
    
    ASSERT_EQUAL(6+1+1+1, a_len(a));
    ASSERT_EQUAL(10+3+3+3, a_size(a));
    
    a_free(a);
}

CTEST(Categories, check_cat_str)
{
    a_str a, b, c, d, e, f;
    
    a = a_new("مرحبا");
    ASSERT_EQUAL(5, a_len(a));
    ASSERT_EQUAL(10, a_size(a));
    
    b = a_new("γεια σας");
    ASSERT_EQUAL(8, a_len(b));
    ASSERT_EQUAL(15, a_size(b));
    
    c = a_new("שלום");
    ASSERT_EQUAL(4, a_len(c));
    ASSERT_EQUAL(8, a_size(c));
    
    d = a_new("hello");
    ASSERT_EQUAL(5, a_len(d));
    ASSERT_EQUAL(5, a_size(d));
    
    e = a_new("您好");
    ASSERT_EQUAL(2, a_len(e));
    ASSERT_EQUAL(6, a_size(e));
    
    f = a_new(NULL);
    f = a_cat_str(a_cat_str(a_cat_str(
            a_cat_str(a_cat_str(f, a), b), c), d), e);
    
    ASSERT_EQUAL(5+8+4+5+2, a_len(f));
    ASSERT_EQUAL(10+15+8+5+6, a_size(f));
    
    ASSERT_EQUAL(0, a_icmp_cstr(f, "مرحباγεια σαςשלוםhello您好"));

    /* small sanity test */
    ASSERT_EQUAL(a_len(a)+a_len(b)
        +a_len(c)+a_len(d)+a_len(e), a_len(f));
    ASSERT_EQUAL(a_size(a)+a_size(b)
        +a_size(c)+a_size(d)+a_size(e), a_size(f));
    
    a_free_n(a, b, c, d, e, f, NULL);
}