

typedef struct _Lookup_Cache_Entry _Lookup_Cache_Entry;
struct _Lookup_Cache_Entry
{
    char ip[MAX_IP_SIZE];
};

uint_fast8_t Parse_IP( const char *buf, struct _Lookup_Cache_Entry *Lookup_Cache );

