/* $Id: md5.h 3553 2011-05-05 06:14:19Z nanang $ */
/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJLIB_UTIL_MD5_H__
#define __PJLIB_UTIL_MD5_H__

/**
 * @file md5.h
 * @brief MD5 Functions
 */

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/** MD5 context. */
typedef struct md5_context
{
	uint32_t buf[4];	/**< buf    */
	uint32_t bits[2];	/**< bits   */
	uint8_t  in[64];	/**< in	    */
} md5_context;

/** Initialize the algorithm. 
 *  @param pms		MD5 context.
 */
void md5_init(md5_context *pms);

/** Append a string to the message. 
 *  @param pms		MD5 context.
 *  @param data		Data.
 *  @param nbytes	Length of data.
 */
void md5_update( md5_context *pms, const uint8_t *data, unsigned nbytes);

/** Finish the message and return the digest. 
 *  @param pms		MD5 context.
 *  @param digest	16 byte digest.
 */
void md5_final(md5_context *pms, char *digest);


char * md5(const char *data, int len, char *out);
/**
 * @}
 */

#endif	/* __PJLIB_UTIL_MD5_H__ */
