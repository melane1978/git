#ifndef HASHMAP_H
#define HASHMAP_H

/*
 * Generic implementation of hash-based key-value mappings.
 * See Documentation/technical/api-hashmap.txt.
 */

/* FNV-1 functions */

extern unsigned int strhash(const char *buf);
extern unsigned int strihash(const char *buf);
extern unsigned int memhash(const void *buf, size_t len);
extern unsigned int memihash(const void *buf, size_t len);

/* data structures */

struct hashmap_entry {
	struct hashmap_entry *next;
	unsigned int hash;
};

typedef int (*hashmap_cmp_fn)(const void *entry, const void *entry_or_key,
		const void *keydata);
typedef void (*hashmap_free_fn)(void *entry);

struct hashmap {
	struct hashmap_entry **table;
	hashmap_cmp_fn cmpfn;
	unsigned int size, tablesize;
};

struct hashmap_iter {
	struct hashmap *map;
	struct hashmap_entry *next;
	unsigned int tablepos;
};

/* hashmap functions */

extern void hashmap_init(struct hashmap *map, hashmap_cmp_fn equals_function,
		size_t initial_size);
extern void hashmap_free(struct hashmap *map, hashmap_free_fn free_function);

/* hashmap_entry functions */

static inline void hashmap_entry_init(void *entry, int hash)
{
	struct hashmap_entry *e = entry;
	e->hash = hash;
	e->next = NULL;
}
extern void *hashmap_get(const struct hashmap *map, const void *key,
		const void *keydata);
extern void *hashmap_get_next(const struct hashmap *map, const void *entry);
extern void hashmap_add(struct hashmap *map, void *entry);
extern void *hashmap_put(struct hashmap *map, void *entry);
extern void *hashmap_remove(struct hashmap *map, const void *key,
		const void *keydata);

/* hashmap_iter functions */

extern void hashmap_iter_init(struct hashmap *map, struct hashmap_iter *iter);
extern void *hashmap_iter_next(struct hashmap_iter *iter);
static inline void *hashmap_iter_first(struct hashmap *map,
		struct hashmap_iter *iter)
{
	hashmap_iter_init(map, iter);
	return hashmap_iter_next(iter);
}

#endif
