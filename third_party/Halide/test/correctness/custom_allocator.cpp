#include "Halide.h"
#include <stdio.h>

using namespace Halide;

// Override Halide's malloc and free

bool custom_malloc_called = false;
bool custom_free_called = false;

void *my_malloc(JITUserContext *user_context, size_t x) {
    custom_malloc_called = true;
    void *orig = malloc(x + 32);
    void *ptr = (void *)((((size_t)orig + 32) >> 5) << 5);
    ((void **)ptr)[-1] = orig;
    return ptr;
}

void my_free(JITUserContext *user_context, void *ptr) {
    custom_free_called = true;
    free(((void **)ptr)[-1]);
}

int main(int argc, char **argv) {
    if (get_jit_target_from_environment().arch == Target::WebAssembly) {
        printf("[SKIP] WebAssembly JIT does not support custom allocators.\n");
        return 0;
    }

    Func f, g;
    Var x;

    f(x) = x;
    g(x) = f(x);
    f.compute_root();

    g.jit_handlers().custom_malloc = my_malloc;
    g.jit_handlers().custom_free = my_free;

    Buffer<int> im = g.realize({100000});

    assert(custom_malloc_called);
    assert(custom_free_called);

    printf("Success!\n");
    return 0;
}
