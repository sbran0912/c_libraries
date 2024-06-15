#include "lib_core.h"

int randomNum(int range_start, int range_end) {
    struct timespec tp;
    clockid_t clk_id;
    clk_id = CLOCK_MONOTONIC;
    clock_gettime(clk_id, &tp);
    srand(tp.tv_nsec);
    return (rand() % (range_end - range_start + 1)) + range_start;
}

float limitNum(float number, float limit) {
	int vorzeichen = (number < 0) ? -1 : 1;
	float numberMag = number * vorzeichen;
	if (numberMag > limit) numberMag = limit;

	return numberMag * vorzeichen;
}

float constrainNum(float value, float min, float max) {
  return fminf(max, fmaxf(min, value));
}

float mapNum(float n, float range_old, float range_new) {
  return (n / range_old * range_new);
}

List list_create(size_t capacity, size_t size) {
    return (List){.array = malloc(capacity * size), .capacity = capacity, .size =  size, .count = 0};
}

void list_append(List* l, void* val) {
    l->count++;
    if ((l->count) > (l->capacity)) {
        l->capacity = l->capacity + 10;
        l->array = realloc(l->array, l->capacity * l->size);
    }
    char* new_val = l->array + ((l->count-1) * l->size);   //count 1 = index 0, count 2 = index 1 etc.
    memcpy(new_val, val, l->size);

}

void* list_at(List* l, int index) {
    char* val = l->array + (index * l->size);
    return (void*)val;
}

void list_free(List* l) {
    free(l->array);
    l->array = NULL;
}

void list_reduce(List* l) {    
    if (l->count < l->capacity) {
        l->capacity = l->count;   //Kapazität auf Länge setzen
        l->array = realloc(l->array, l->capacity * l->size);
    }
}

ListStr liststr_create(size_t capacity) {
    return (ListStr){.array = malloc(capacity * sizeof(char*)), .capacity = capacity, .count = 0};
}

void liststr_append(ListStr* l, char val[]) {
    l->count++;
    if ((l->count) > (l->capacity)) {
        l->capacity = l->capacity + 10;
        l->array = realloc(l->array, l->capacity * sizeof(char*));
    }
    l->array[l->count-1] = malloc(strlen(val)+1);
    strcpy(l->array[l->count-1], val);
}

void liststr_free(ListStr* l) {
    for (size_t i = 0; i < l->count; i++) {
        free(l->array[i]);
    }
    free(l->array);
    l->array = NULL;
}

void liststr_reduce(ListStr* l) {    
    if (l->count < l->capacity) {
        l->capacity = l->count;   //Kapazität auf Länge setzen
        l->array = realloc(l->array, l->capacity * sizeof(char*));
    }
}

ListStr split(char string[], const char delimiter) {
    ListStr result = liststr_create(10);
    char d[0];
    d[0] = delimiter;

    char *token = strtok(string, d);
    while (token != NULL) {
        liststr_append(&result, token);
        token = strtok(NULL, d);
    }
    return result;
}

ListFloat2d listf2d_create(size_t rows, size_t cols, float val) {
    float** arr = malloc(rows * sizeof(float*));
    for (size_t i = 0; i < rows; i++) {
        arr[i] = malloc(cols * sizeof(float));
        for (size_t j = 0; j < cols;j++) {
            arr[i][j] = val;
        }
    }
    return (ListFloat2d){.array = arr, .rows = rows, .cols = cols};
}

void listf2d_free(ListFloat2d* l) {
    for (size_t i = 0; i < l->rows; i++) {
        free(l->array[i]);
    }
    free(l->array);
    l->array = NULL;
}

void* arr_init(size_t capacity, size_t elemsize) {
    size_t *raw = malloc(3 * sizeof(size_t) + (capacity * elemsize));
    raw[0] = 0;                 // Number of elements currently in the array
    raw[1] = capacity;         // capacity - how many elements can the array currently store
    raw[2] = elemsize;           // Size of elements
    return (void*) &raw[3];     // Pointer to the beginning of the array
}

size_t arr_len(void* arr) {
    return *((size_t*)arr - 3);
}

size_t arr_cap(void* arr) {
    return *((size_t*)arr - 2);
}

void arr_append(void* ref_arr, void* val) {
    // Übergabe der Referenz des Pointers (Pointer auf Pointer)
    size_t* raw = (size_t*)(*(void**)ref_arr) - 3;
    size_t len = raw[0];
    size_t cap = raw[1];
    size_t elemsize = raw[2];

    len++;                  //increase counter
    raw[0] = len;           //save counter in array

    if (len > cap) {
        size_t new_cap = cap + 10;
        raw[1] = new_cap;      //calculate new capacity
        raw = realloc(raw, 3 * sizeof(size_t) + (new_cap * elemsize));
        *(void**)ref_arr = (void*)&raw[3]; //Array-Pointer wird wieder auf richtigen Startpunkt gestellt
    } 
    
    char* new_item = (void*)&raw[3] + ((len-1) * elemsize);   //count 1 = index 0, count 2 = index 1 etc.
    memcpy(new_item, val, elemsize);
}

void arr_reduce(void* ref_arr) {
    // Übergabe der Referenz des Pointers (Pointer auf Pointer)
    size_t* raw = (size_t*)(*(void**)ref_arr) - 3;

    size_t len = raw[0];
    size_t cap = raw[1];
    size_t elemsize = raw[2];
    
    if (len < cap) {
        raw[1] = len; //Kapazität auf Länge setzen
        raw = realloc(raw, 3 * sizeof(size_t) + (len * elemsize)); //Speicherbereich kürzen
        *(void**)ref_arr = (void*)&raw[3]; //Array-Pointer wird wieder auf richtigen Startpunkt gestellt
    }
}

void arr_destroy(void* ref_arr) {
    size_t* raw = (size_t*)(*(void**)ref_arr) - 3;
    free(raw);
}

Vector2 Vec2Scale(Vector2 v, float n) {
	return (Vector2) {v.x * n, v.y * n};
}

Vector2 Vec2Div(Vector2 v, float n) {
	Vector2 result = {v.x/n, v.y/n};
	return result;

}

Vector2 Vec2Add(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x + v2.x, v1.y + v2.y};
	return result;
}

Vector2 Vec2Sub(Vector2 v1, Vector2 v2) {
	Vector2 result = {v1.x - v2.x, v1.y - v2.y};
	return result;
}

Vector2 Vec2Normalize(Vector2 v) {
    Vector2 result = {0};
    float len = sqrtf((v.x*v.x) + (v.y*v.y));
    if (len > 0.001) {
        result.x = v.x / len;
        result.y = v.y / len;
    }
    return result;	
}

Vector2 Vec2Limit(Vector2 v, float max) {
	Vector2 result = v;
	float len = sqrtf(result.x * result.x + result.y * result.y);
	if (len > max) {
		result.x = result.x / len * max;
		result.y = result.y / len * max;
	}
	return result;
}

Vector2 Vec2SetMag(Vector2 v, float magnitude) {
    Vector2 result = {0};
    float len = sqrtf((v.x*v.x) + (v.y*v.y));
    if (len > 0.001) {
        result.x = v.x / len * magnitude;
        result.y = v.y / len * magnitude;
    }
    return result;	

}

float Vec2DotProduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float Vec2CrossPoduct(Vector2 v1, Vector2 v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vector2 Vec2Perp(Vector2 v) {
	return (Vector2){-v.y, v.x};
}

float Vec2Mag(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

float Vec2Magsq(Vector2 v) {
	return v.x * v.x + v.y * v.y;
}

float Vec2Dist(Vector2 v1, Vector2 v2) {
	Vector2 vdist = Vec2Sub(v1, v2);
	return Vec2Mag(vdist);
}

Vector2 Vec2Rotate(Vector2 v, Vector2 base, float n) {
	Vector2 direction = Vec2Sub(v, base);
	float x = direction.x * cosf(n) - direction.y * sinf(n);
	float y = direction.x * sinf(n) + direction.y * cosf(n);
	return (Vector2){x = x + base.x, y = y + base.y};
}

float Vec2Heading(Vector2 v) {
	return atan2f(v.y, v.x);
}

float Vec2Angle(Vector2 v1, Vector2 v2) {
    float result = 0.0f;
    float dot = v1.x*v2.x + v1.y*v2.y;
    float det = v1.x*v2.y - v1.y*v2.x;
    result = atan2f(det, dot);

    return result;
}

float minDistance(Vector2 p, Vector2 start_a, Vector2 end_a) {
	float dist = -1.0f;

	//Vektor start_a to end_a (line_a)
	Vector2 line_a = Vec2Sub(end_a, start_a);
	//Vektor imaginary line start_a to p
	Vector2 start_a_to_p = Vec2Sub(p, start_a);
	//Magnitude of line_a
	float magnitude = Vec2Mag(line_a);

	//Scalarprojecton from line (start_a to p) on line_a
	line_a = Vec2Normalize(line_a);
	float sp = Vec2DotProduct(line_a, start_a_to_p);

	//Scalarprojection in magnitude of line_a?
	if (sp > 0.0001 && sp <= magnitude) {
		line_a= Vec2Scale(line_a, sp);
		dist = Vec2Mag(Vec2Sub(start_a_to_p, line_a));
	}
	return dist;
}