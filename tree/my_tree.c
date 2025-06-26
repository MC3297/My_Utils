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

/*
Copies all dirent entries from readdir and puts it in an array of pointers
Modifies `n` to store array size
Note: array is malloc-ed so remember to free both contents and itself
*/
struct dirent** get_entries_array(char *path, int *n) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }
    
    int cnt = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        cnt++;
    }
    *n = cnt;
    
    struct dirent **arr = malloc(cnt * sizeof(struct dirent *));
    if (arr == NULL) {
        perror("malloc");
        exit(1);
    }
    
    int ind = 0;
    dir = opendir(path);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        size_t sz = sizeof(struct dirent) + strlen(entry->d_name) + 1;
        arr[ind] = malloc(sz);
        memcpy(arr[ind], entry, sz);
        ind++;
    }
    
    return arr;
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
    // recurse(".", 0);
    int n = 0;
    struct dirent **arr = get_entries_array(".", &n);
    for (int i = 0; i < n; i++) {
        printf("%s\n", arr[i]->d_name);
    }
}