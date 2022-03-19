#include "globalThings.h"

#include <stdio.h>

/*static*/ void GLFW_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);

    // Print to console, too
    printf("GLFW Error: %s\n", description);

    return;
}
