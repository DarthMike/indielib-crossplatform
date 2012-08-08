/*****************************************************************************************
 * File: Tools.cpp
 * Desc: IndieLib Helper class
 *****************************************************************************************/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <michael@visualdesign.dk> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Michael Fogh Kristensen
 * ----------------------------------------------------------------------------
 */

#include "Tools.h"


/**
 * Cross-Platform version of itoa. support only 10 radix for easy use and better performance
 * taken from http://code.google.com/p/my-itoa/  (LGPL)
 *
 * @param val             The int that needs to be converted
 * @param buf             The char buffer that is going to held the result
 * @return                The length of the result String.
 */
int Tools::itoa(int val, char* buf) {
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;               //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0) {
        *p++ = '-';
        val = 0 - val;
    }
    
	u = (unsigned int)val;
    b = p;

    do {
        a = u % radix;
        u /= radix;
        *p++ = (char)a + '0';
    } while (u > 0);

    len = (int)(p - buf);
    *p-- = 0;

    //swap
    do {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    } while (b < p);

    return len;
}
