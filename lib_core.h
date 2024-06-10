#ifndef CORE_H
#define CORE_H
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

typedef struct {
	float index[3][3];
} Matrix3x3;

typedef struct {
	float index[3];
} Matrix3x1;

typedef struct {
    void* array;         //Array
    size_t count;       //Länge
    size_t capacity;    //Kapazität
    size_t size;      //Größe des Elements
} List;

typedef struct {
    char** array;         //Array of Strings
    size_t count;       //Länge
    size_t capacity;    //Kapazität
} ListStr;

typedef struct {
    float** array;      //Array of Strings
    size_t rows;     //Anzahl Zeilen
    size_t cols;     //Anzahl Spalten
} ListFloat2d;


int randomNum(int range_start, int range_end);
float limitNum(float number, float limit);
float constrainNum(float value, float min, float max); // limits value between min and max
float mapNum(float n, float range_old, float range_new); // scales value n to a new range

List list_create(size_t capacity, size_t size);
void list_append(List* l, void* val);
void* list_at(List* l, int index);
void list_free(List* l);
ListStr liststr_create(size_t capacity);
ListStr liststr_append(ListStr* l, char val[]);
void liststr_free(ListStr* l);
ListFloat2d listf2d_create(size_t rows, size_t cols, float val);
void listf2d_free(ListFloat2d* l);

Vector2 Vec2Scale(Vector2 v, float n);
Vector2 Vec2Div(Vector2 v, float n);
Vector2 Vec2Add(Vector2 v, Vector2 v_add);
Vector2 Vec2Sub(Vector2 v, Vector2 v_sub);
Vector2 Vec2Normalize(Vector2 v);
Vector2 Vec2Limit(Vector2 v, float max);
Vector2 Vec2SetMag(Vector2 v, float magnitude);
float Vec2DotProduct(Vector2 v1, Vector2 v2);
float Vec2CrossPoduct(Vector2 v1, Vector2 v2);
Vector2 Vec2Perp(Vector2 v);
float Vec2Mag(Vector2 v);
float Vec2Magsq(Vector2 v);
float Vec2Dist(Vector2 v1, Vector2 v2);
Vector2 Vec2Rotate(Vector2 v, Vector2 base, float n);
float Vec2Heading(Vector2 v);
float Vec2Angle(Vector2 v1, Vector2 v2);

float minDistance(Vector2 p, Vector2 start_a, Vector2 end_a);


#endif