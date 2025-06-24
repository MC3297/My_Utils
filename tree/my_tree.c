#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>


#define RECURSION_DEPTH 200

int tab_ind = 0;
int tabs[RECURSION_DEPTH];

/*
Generates new path for subdirectories
Ex: ("./dir0", "dir1") -> "./dir0/dir1"
Note: Remember to free()
*/
char* path_concat(const char *path, const char *dir) {
    char *res = malloc(strlen(path) + strlen(dir) + 2);//+1 for null char
    strcpy(res, path);
    strcat(res, "/");
    strcat(res, dir);
    return res;
}

/*
Prints the prefix of tabs per entry
*/
void print_tabs() {
    for (int i = 0; i < tab_ind; i++) {
        switch (tabs[i])
        {
        case 0:
            printf("    ");
            break;
        case 1:
            printf("│   ");
            break;
        case 2:
            printf("├───");
        case 3:
            printf("└───");
            break;
        }
    }
}

void recurse(const char *path, int depth) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        if (entry->d_type == DT_REG) {
            printf("file: %s\n", entry->d_name);
        }
        
        if (entry->d_type == DT_DIR) {
            printf("dir: %s\n", entry->d_name);
            
            char *subpath = path_concat(path, entry->d_name);
            recurse(subpath, depth+1);
            free(subpath);
        }
    }
    
    closedir(dir);
}

int main() {
    recurse(".", 0);
}