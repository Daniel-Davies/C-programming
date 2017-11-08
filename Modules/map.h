/* A map keeps track of the relationship between keys and values.  Keys are
used to lookup values.  The module is generic, as values are pointers, but keys are of type int.
A function for comparing two keys is passed to the newMap function.
Iteration is supported by passing a function pointer to the iterate
function.  Maps are not thread-safe. */

// The map type is opaque.
struct map;
typedef struct map map;

// The type of a comparison function (to be passed by pointer to newMap).
typedef int comparison(int key1, int key2);

// The type of an action function (for iteration).
typedef void action(int key, void *value, void *data);

// Create a new map, with a given comparison function.
map *newMap(comparison *compare, int size);

// Lookup a key and get the corresponding value back, or NULL if not found.
void *lookup(map *m, int key);

// Insert or update a key and value.
void put(map *m, int key, void *value);

// Traverse the map, calling the given action function on each key/value pair.
// On each call, the data object will be passed to the action function.
void iterate(map *m, action *act, void *data);

int testing();
