#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <assert.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3); 
    int expected_result = 8; 

    char actual_str[20];
    sprintf(actual_str, "%d", result);

    char expected_str[20];
    sprintf(expected_str, "%d", expected_result);


    assert(strcmp(actual_str, expected_str) == 0);


    return 0;
}
