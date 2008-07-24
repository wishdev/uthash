#include <stdlib.h>    /* malloc       */
#include <stddef.h>    /* offsetof     */
#include <stdio.h>     /* printf       */
#include <string.h>    /* memset       */
#include "uthash.h"

#define UTF32 1

typedef struct {
  UT_hash_handle hh;
  int len;
  char encoding;      /* these two fields */
  int text[];         /* comprise the key */
} msg_t;

int main(int argc, char *argv[]) {
    int keylen;
    msg_t *msg, *msgs = NULL;
    struct { char encoding; int text[]; } *lookup_key;

    int beijing[] = {0x5317, 0x4eac};   /* UTF-32LE for 北京 */

    /* allocate and initialize our structure */
    msg = malloc( sizeof(msg_t) + sizeof(beijing) );
    memset(msg, 0, sizeof(msg_t)+sizeof(beijing)); /* zero fill */
    msg->len = sizeof(beijing);
    msg->encoding = UTF32;
    memcpy(msg->text, beijing, sizeof(beijing));

    /* calculate the key length including padding, using formula */
    keylen =   offsetof(msg_t, text)       /* offset of last key field */
             + sizeof(beijing)             /* size of last key field */
             - offsetof(msg_t, encoding);  /* offset of first key field */

    /* add our structure to the hash table */
    HASH_ADD( hh, msgs, encoding, keylen, msg);

    /* look it up to prove that it worked :-) */
    msg=NULL;

    lookup_key = malloc(sizeof(*lookup_key) + sizeof(beijing));
    memset(lookup_key, 0, sizeof(*lookup_key) + sizeof(beijing));
    lookup_key->encoding = UTF32;
    memcpy(lookup_key->text, beijing, sizeof(beijing));
    HASH_FIND( hh, msgs, &lookup_key->encoding, keylen, msg );
    if (msg) printf("found \n");
    free(lookup_key);
}
