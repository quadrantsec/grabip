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
#include <stdbool.h>
#include <arpa/inet.h>

#include <inttypes.h>

#ifdef HAVE_CONFIG_H
#include "config.h"             /* From autoconf */
#endif

#include "grabip-defs.h"
#include "parseip.h"


uint_fast8_t Parse_IP( const char *buf, struct _Lookup_Cache_Entry *Lookup_Cache )
{

    int32_t i = 0;

    uint_fast16_t num_colons = 0;
    uint_fast16_t num_dots = 0;
    uint_fast16_t current_position = 0;

    bool valid = false;

    char *ptr1 = NULL;
    char *ptr2 = NULL;

    char *ip_1 = NULL;
    char *ip_2 = NULL;

    struct sockaddr_in sa;

    char *mod_string = malloc( BUFFER_SIZE );

    if ( mod_string  == NULL )
        {
            fprintf(stderr, "[%s, line %d] Fatal Error: Can't allocate memory! Abort!\n", __FILE__, __LINE__);
            exit(-1);
        }

    memset(mod_string, 0, BUFFER_SIZE );

    for (i=0; i<strlen(buf); i++)
        {

            if ( buf[i] != '"' && buf[i] != '(' && buf[i] != ')' &&
                    buf[i] != '[' && buf[i] != ']' && buf[i] != '<' &&
                    buf[i] != '>' && buf[i] != '{' && buf[i] != '}' &&
                    buf[i] != ',' && buf[i] != '/' && buf[i] != '@' &&
                    buf[i] != '=' && buf[i] != '-' && buf[i] != '!' &&
                    buf[i] != '|' && buf[i] != '_' && buf[i] != '+' &&
                    buf[i] != '&' && buf[i] != '%' && buf[i] != '$' &&
                    buf[i] != '~' && buf[i] != '^' && buf[i] != '#' &&
                    buf[i] != '\'' && buf[i] != '\t' && buf[i] != '\n' &&
                    buf[i] != '\r' && buf[i] != '#' )
                {

                    mod_string[i] = buf[i];
                    mod_string[i+1] = '\0';

                }
            else
                {

                    mod_string[i] = ' ';
                    mod_string[i+1] = '\0';

                }

        }

    ptr1 = strtok_r(mod_string, " ", &ptr2);

    while ( ptr1 != NULL )
        {

            num_colons = 0;
            num_dots = 0;
//            num_hashes = 0;

            for ( i = 0; i < strlen(ptr1); i++ )
                {

                    switch(ptr1[i])
                        {

                        case(':'):
                            num_colons++;
                            break;

//                        case('#'):
//                            num_hashes++;
//                            break;

                        case('.'):
                            num_dots++;
                            break;

                        }
                }

//            printf("Colons: %d, Dots: %d\n", num_colons, num_dots );

            /* Needs to have proper IPv6 or IPv4 encoding. num_dots > 4 is for IP with trailing
            period. */

            if ( ( num_colons < 2 && num_dots < 3 ) || ( num_dots > 4 ) )
                {
                    ptr1 = strtok_r(NULL, " ", &ptr2);          /* move to next token */
                    continue;
                }

            /* Stand alone IPv4 address */

            if ( num_dots == 3 && num_colons == 0 )
                {

                    valid = inet_pton(AF_INET, ptr1,  &(sa.sin_addr));

                    if ( valid == 1 )
                        {

                            // printf("Got standalone IP: %s\n", ptr1);
                            memcpy(Lookup_Cache[current_position].ip, ptr1, MAX_IP_SIZE);
                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }

                        }


                }

            /* Stand alone IPv4 with trailing period */

            if ( num_dots == 4 && ptr1[ strlen(ptr1)-1 ] == '.' )
                {

                    /* Remove period */

                    ptr1[ strlen(ptr1)-1 ] = '\0';

                    valid = inet_pton(AF_INET, ptr1,  &(sa.sin_addr));

                    if ( valid == 1 )
                        {

//                            printf("IP with period: %s\n", ptr1);
                            memcpy(Lookup_Cache[current_position].ip, ptr1, MAX_IP_SIZE);

                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }
                        }

                }


            /* IPv4 with 192.168.2.1:12345 or inet:192.168.2.1 */

            if ( num_colons == 1 && num_dots == 3)
                {

                    /* test both sides */

                    ip_1 = strtok_r(ptr1, ":", &ip_2);

                    /* trailing : */

                    if ( ip_1 != NULL )
                        {
                            valid = inet_pton(AF_INET, ip_1,  &(sa.sin_addr));
                        }

                    if ( valid == 1 )
                        {

//                            printf("With colon: %s\n", ip_1);
                            memcpy(Lookup_Cache[current_position].ip, ip_1, MAX_IP_SIZE);

                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }

                        }

                    /* Leading : */

                    if ( ip_2 != NULL )
                        {
                            valid = inet_pton(AF_INET, ip_2,  &(sa.sin_addr));
                        }

                    if ( valid == 1 )
                        {

//                            printf("Leading colon: %s\n", ip_2);

                            memcpy(Lookup_Cache[current_position].ip, ip_2, MAX_IP_SIZE);
                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }

                        }

                }


            /* Stand alone IPv6 */

//            printf("num_colons: %d\n", num_colons);

            if ( num_colons > 2 )
                {

//                    printf("INIT: %s\n", ptr1);

                    valid = inet_pton(AF_INET6, ptr1,  &(sa.sin_addr));

                    if ( valid == 1 )
                        {

                            memcpy(Lookup_Cache[current_position].ip, ptr1, MAX_IP_SIZE);

//                            printf("Stand alone IPv6 %s\n", ptr1);

                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }
                        }
                }

            /* Stand alone IPv6 with trailing period */

            if ( num_colons > 2 && ptr1[ strlen(ptr1)-1 ] == '.' )
                {

                    /* Erase the period */

                    ptr1[ strlen(ptr1)-1 ] = '\0';

                    valid = inet_pton(AF_INET6, ptr1,  &(sa.sin_addr));

                    if ( valid == 1 )
                        {

//			printf("IPv6 with . at the end: %s\n", ptr1);

                            memcpy(Lookup_Cache[current_position].ip, ptr1, MAX_IP_SIZE);

                            current_position++;

                            if ( current_position > MAX_PARSE_IP )
                                {
                                    fprintf(stderr, "Went over the MAX_PARSE_IP limit of %d\n", MAX_PARSE_IP);
                                    exit(-1);
                                }


                        }
                }


            ptr1 = strtok_r(NULL, " ", &ptr2);

        } /* while ptr1 != NULL */

    free(mod_string);
    return(current_position);

}
