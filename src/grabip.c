/*
** Copyright (C) 2009-2023 Quadrant Information Security <quadrantsec.com>
** Copyright (C) 2009-2023 Champ Clark III <cclark@quadrantsec.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#ifdef HAVE_CONFIG_H
#include "config.h"             /* From autoconf */
#endif

#include "grabip-defs.h"
#include "grabip.h"
#include "parseip.h"

int main()
{

    char *buf = NULL;
    buf = malloc( BUFFER_SIZE );

    if ( buf == NULL )
        {
            fprintf(stderr, "[%s, line %d] Error allocating memory for buffer.\n", __FILE__, __LINE__);
            exit(-1);
        }

    uint16_t lookup_cache_size = 0;
    uint16_t i = 0;

    uint64_t ip_entry_size = 0;
    uint64_t z = 0;

    struct _IP_Entry *IP_Entry = NULL;

    struct _Lookup_Cache_Entry *Lookup_Cache = NULL;
    Lookup_Cache = malloc(sizeof(struct _Lookup_Cache_Entry) * MAX_PARSE_IP);

    if ( Lookup_Cache == NULL )
        {
            fprintf(stderr, "[%s, line %d] Failed to allocate memory for Lookup_Cache.", __FILE__, __LINE__);
            exit(1);
        }

    memset(Lookup_Cache, 0, sizeof(_Lookup_Cache_Entry) * MAX_PARSE_IP);

    while (fgets(buf, BUFFER_SIZE, stdin))
        {

            lookup_cache_size = Parse_IP( buf, Lookup_Cache );

            for ( i = 0; i < lookup_cache_size; i++ )
                {

                    bool found = false;

                    for ( z = 0; z < ip_entry_size; z++ )
                        {
                            if ( !strcmp(IP_Entry[z].ip, Lookup_Cache[i].ip ) )
                                {

                                    found = true;

                                }
                        }

                    if ( found == false )
                        {

                            IP_Entry = (_IP_Entry *) realloc(IP_Entry, (ip_entry_size+1) * sizeof(_IP_Entry));

                            if ( IP_Entry == NULL )
                                {
                                    fprintf(stderr, "[%s, line %d] Failed to reallocate memory for _IP_Entry\n", __FILE__, __LINE__);
                                    exit(-1);
                                }

                            memset(&IP_Entry[ip_entry_size], 0, sizeof(struct _IP_Entry));

                            memcpy(IP_Entry[ip_entry_size].ip, Lookup_Cache[i].ip, MAX_IP_SIZE);
                            ip_entry_size++;

                        }
                }
        }


    for ( z = 0; z < ip_entry_size; z++ )
        {
            fprintf(stdout, "%s\n", IP_Entry[z].ip);
        }

    free(Lookup_Cache);
    free(buf);
}


