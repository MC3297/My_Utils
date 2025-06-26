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

#define E 0
#define I 1
#define T 2
#define L 3

int tab_ind = 0;
int tabs[RECURSION_DEPTH];

/*
Generates new path for subdirectories
Ex: ("./dir0", "dir1") -> "./dir0/dir1"
Note: Remember to free()
*/
char* path_concat(const char *path, const char *dir) {
    char *res = malloc(strlen(path) + strlen(dir) + 2);//+1 for null char
    if (res == NULL) {
        perror("malloc");
        exit(1);
    }
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
        case E:
            printf("    ");
            break;
        case I:
            printf("│   ");
            break;
        case T:
            printf("├───");
            break;
        case L:
            printf("└───");
            break;
        }
    }
}

void print_dir(const char *s) {
    printf("\x1b[34m%s\x1b[0m\n", s);
}

void print_file(const char *s) {
    printf("\x1b[0m%s\x1b[0m\n", s);
}

/*
Copies all dirent entries from readdir and puts it in an array of pointers
Modifies `n` to store array size
Note: array is malloc-ed so remember to free both contents and itself
*/
struct dirent** get_entries_array(const char *path, int *n) {
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
    closedir(dir);
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
    closedir(dir);
    
    return arr;
}

void recurse(const char *path) {
    if (tab_ind >= RECURSION_DEPTH) {
        perror("recursion depth exceeded");
        exit(1);
    }
    
    int n;
    struct dirent **entries = get_entries_array(path, &n);
    
    for (int i = 0; i < n; i++) {
        
        struct dirent *entry = entries[i];
        
        if (i == n-1) tabs[tab_ind] = L;
        else tabs[tab_ind] = T;
        tab_ind++;
        
        print_tabs();
        
        if (entry->d_type == DT_REG) {
            print_file(entry->d_name);
        }
        
        if (entry->d_type == DT_DIR) {
            print_dir(entry->d_name);
            
            if (i == n-1) tabs[tab_ind-1] = E;
            else tabs[tab_ind-1] = I;
            
            char *subpath = path_concat(path, entry->d_name);
            recurse(subpath);
            free(subpath);
        }
        
        tab_ind--;
    }

    for (int i = 0; i < n; i++) {
        free(entries[i]);
    }
    free(entries);
}

int main(int argc, char *argv[]) {
    char *path = ".";
    if (argc == 2) 
        path = argv[1];
    
    printf("%s\n", path);
    recurse(path);
}