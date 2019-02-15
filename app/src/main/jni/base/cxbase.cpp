#include "cxbase.h"

int memManager::refInt = 0;
int memManager::refFloat = 0;
int memManager::refDouble = 0;
int memManager::refPointer = 0;
void memManager::print() {
    fprintf( stderr, "mem int: %d, float: %d, double: %d, pointer: %d\n",
                            refInt, refFloat, refDouble, refPointer );
}

void** MallocPointers( int n ) {
    void** tmp = (void**)malloc( sizeof(void*) *n );
    if ( tmp )
        memManager::refPointer++;
    return tmp;
}
void FreePointers( void **ptr ) {
    if ( ptr ) {
        memManager::refPointer--;
        free( ptr );
    }
}

int* MallocInt( int n ) {
    int *tmp = (int*)malloc( sizeof(int) *n );
    if ( tmp )
        memManager::refInt++;
    return tmp;
}

void FreeInt( int *ptr ) {
    if ( ptr ) {
        memManager::refInt--;
        free( ptr );
    }
}

float* MallocFloat( int n ) {
    float *tmp = (float*)malloc( sizeof(float) *n );
    if ( tmp )
        memManager::refFloat++;
    return tmp;
}

void FreeFloat( float *ptr ) {
    if ( ptr ) {
        memManager::refFloat--;
        free( ptr );
    }
}

double* MallocDouble( int n ) {
    double *tmp = (double*)malloc( sizeof(double) *n );
    if ( tmp )
        memManager::refDouble++;
    return tmp;
}

void FreeDouble( double *ptr ) {
    if ( ptr ) {
        memManager::refDouble--;
        free( ptr );
    }
}


const float MIN_FVAR = 1E-10;

bool isEqualF( float fval1, float fval2 ) {
    float delta = fval1 - fval2;
    if ( delta > -MIN_FVAR && delta < MIN_FVAR )
        return true;
    return false;
}
