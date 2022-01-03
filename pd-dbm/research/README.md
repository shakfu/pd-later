
# Hashmap Implementations



## tidwall hashmap

- [tidwall/hashmap.c](https://github.com/tidwall/hashmap.c)



## Mashpoe hashmap
- [Mashpoe/c-hashmap](https://github.com/Mashpoe/c-hashmap)


## sheredom hashmap

- [sheredom/hashmap.h](https://github.com/sheredom/hashmap.h)


## phtdacosta dict-utils

- [phtdacosta/dict-utils](https://github.com/phtdacosta/dict-utils)


## richard-w hashmap

- [hashmap.c](https://gist.github.com/Richard-W/9568649)

```c
/* Copyright (C) 2014  Richard Wiedenhöft <richard.wiedenhoeft@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/* A very simple hash-algorithm */
unsigned long int hashmap_fnv1a(void *data, unsigned long int len) {
	unsigned char *p = (unsigned char *) data;
	unsigned long int h = 2166136261UL;
	unsigned long int i;

	for(i = 0; i < len; i++) {
		h = (h ^ p[i]) * 16777619;
	}

	return h;
}

/* Maps the hash to the interval [0, max_hash] */
int hashmap_hash(char *str, int max_hash) {
	unsigned long int fnv1a_hash = hashmap_fnv1a((void*) str, strlen(str));
	int hash = (int) (fnv1a_hash % (max_hash + 1));
	return hash;
}

/* A pointer to this struct is defined as hashmap_t. */
struct hashmap_s {
	int size;
	struct hashmap_field *fields;
};

/* Contains a number of entries which have the same hash */
struct hashmap_field {
	int size;
	struct hashmap_entry *entries;
};

/* A key-value pair */
struct hashmap_entry {
	char *key;
	void *val;
	size_t len;
};

/* Creates a new hashmap */
hashmap_t hashmap_new(int size) {
	hashmap_t map = (hashmap_t) malloc(sizeof(struct hashmap_s));
	map->size = size;
	map->fields = (struct hashmap_field *) malloc(sizeof(struct hashmap_field) * size);
	int i;
	for(i = 0; i < size; i++) {
		struct hashmap_field *field = map->fields + i;
		field->size = 0;
		field->entries = NULL;
	}
	return map;
}

/* Frees the hashmap. You have to manually set the pointer to NULL after using this function */
void hashmap_free(hashmap_t map) {
	int i;
	for(i = 0; i < map->size; i++) {
		struct hashmap_field *field = map->fields + i;
		if(field->entries != 0) {
			int j;
			for(j = 0; j < field->size; j++) {
				struct hashmap_entry *entry = field->entries + j;
				free(entry->key);
				free(entry->val);
			}
			free(field->entries);
		}
	}
	free(map->fields);
	free(map);
}

/* Sets a value in the hashmap. The memory area that val points to is copied. */
void hashmap_set(hashmap_t map, char *key, void *value, size_t length) {
	int hash = hashmap_hash(key, map->size - 1);
	struct hashmap_field *field = map->fields + hash;
	struct hashmap_entry *entry;

	int i;
	/* Check if entry with the same key already exists in field. */
	for(i = 0; i < field->size; i++) {
		entry = field->entries + i;
		if(strcmp(entry->key, key) == 0) {
			/* Depending on value entry is deleted
			or another value is set. In both cases the
			old val can be freed */
			free(entry->val);
			goto set_val;
		}
	}
	/* Create new entry */
	if(value == NULL) return;
	field->size++;
	field->entries = realloc((void*) field->entries, field->size * sizeof(struct hashmap_entry));
	entry = field->entries + field->size - 1;
	entry->key = strdup(key);

	set_val:
	if(value != NULL) {
		entry->val = memcpy(malloc(length), value, length);
		entry->len = length;
	} else {
		/* val is already freed. Key is left */
		free(entry->key);
		field->size--;
		/* Copy last entry to new position */
		if(entry != (field->entries + field->size)) {
			memcpy((void*) entry, (void*) (field->entries + field->size), sizeof(struct hashmap_entry));
		}
		/* Shrink field */
		field->entries = realloc((void*) field->entries, field->size * sizeof(struct hashmap_entry));
	}
}

/* Get a void-ptr from the hash-map. It is copied from the hashmap and must be freed manually. */
void *hashmap_get(hashmap_t map, char *key) {
	int hash = hashmap_hash(key, map->size - 1);
	struct hashmap_field *field = map->fields + hash;
	struct hashmap_entry *entry;

	int i;
	for(i = 0; i < field->size; i++) {
		entry = field->entries + i;
		if(strcmp(entry->key, key) == 0) {
			return memcpy(malloc(entry->len), entry->val, entry->len);
		}
	}
	return NULL;
}
```

The header is `hashmap.h`:

```c
/* Copyright (C) 2014  Richard Wiedenhöft <richard.wiedenhoeft@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

typedef struct hashmap_s *hashmap_t;

hashmap_t hashmap_new(int);
void hashmap_free(hashmap_t);
void *hashmap_get(hashmap_t, char*);
void hashmap_set(hashmap_t, char*, void*, size_t);
```
