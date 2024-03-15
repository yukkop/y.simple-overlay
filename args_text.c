#include <stdio.h>

// int main(int argc, char *argv[]) {
//     for (int i = 0; i < argc; i++) {
//         printf("Argument %d: %s\n", i, argv[i]);
//     }
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Calculate the total length needed for the concatenated string
    int totalLength = 0;
    for (int i = 1; i < argc; i++) {
        totalLength += strlen(argv[i]) + 1; // +1 for space or null terminator
    }

    if (totalLength == 0) {
        printf("No arguments to concatenate.\n");
        return 0;
    }

    // Allocate memory for the concatenated string
    char *allArgs = malloc(totalLength * sizeof(char));
    if (allArgs == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    allArgs[0] = '\0'; // Initialize the string with a null terminator

    // Concatenate all arguments
    for (int i = 1; i < argc; i++) {
        strcat(allArgs, argv[i]);
        if (i < argc - 1) {
            strcat(allArgs, " "); // Add a space between arguments, not after the last one
        }
    }

    // Print the concatenated string
    printf("All arguments: %s\n", allArgs);

    // Free the allocated memory
    free(allArgs);

    return 0;
}
