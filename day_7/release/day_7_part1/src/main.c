#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

static int result = 0;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

#include <string.h>

#define MAX_SIZE 100000
#define MAX_ARG_SIZE 32

typedef enum {
    NONE, CHANGE_DIR, LIST
} command_type;

typedef struct dir_s
{
    struct dir_s *childs;
    struct dir_s *last;
    size_t nbChild; 
    size_t size;
    char name[MAX_ARG_SIZE];
} dir_t;

static dir_t root = {NULL, NULL, 0, 0, "/"};
static dir_t *currentDir = &root;

/**
 * Add a sub directory in a dir_t struct
 * @param  dir       The destination directory
 * @param  childName The directoy to copy in the dir
 * @return           1 if success, else 0
 */
int add_sub_directory(char *childName) 
{
    // try reallocate
    dir_t *p_tmp = realloc(currentDir->childs, sizeof (dir_t) * (currentDir->nbChild+1));
    if(!p_tmp)
        return 0;
    currentDir->childs = p_tmp;

    // add child directory
    currentDir->nbChild ++;
    dir_t *child = &currentDir->childs[currentDir->nbChild-1];
    strcpy(child->name, childName);
    child->last = currentDir;
    child->nbChild = 0;

    return 1;
}

/**
 * Get the command at line and put the pointer at 
 * @param  p_data Point to the code
 * @return        The command type or None if it's a bad command
 */
command_type get_command_type(char **p_data)
{ 
    // Check is a command
    if(*(*p_data) != '$')
        return NONE;
    (*p_data) += 2;

    // Check command
    if(*(*p_data) == 'c' && *((*p_data) + 1) == 'd') {
        // If ChgDir
        (*p_data) += 3;
        return CHANGE_DIR;
    } else if (*(*p_data) == 'l' && *((*p_data) + 1) == 's') {
        // If List
        skip_line(p_data);
        return LIST;
    }

    // If it's an unknown command
    return NONE;
}

/**
 * Get a substring until it reach delim and update data pointer
 * @param destBuffer The buffer to write the string (Must be already allocated)
 * @param p_data     The data input
 * @param delim      The delimiter
 */
void get_string(char *destBuffer, char **p_data, char delim)
{
    // Setup vars
    int idx = 0;

    // Until it reach the delim and is not null
    while(**p_data != delim && **p_data){
        // Copy data
        destBuffer[idx] = **p_data;
        (*p_data)++;
        idx++;
    }
    destBuffer[idx] = 0;
}

/**
 * Get a directory by a name
 * @param  name The name of the directory
 * @return      The directory pointer
 */
dir_t *get_subdir_by_name(char *name)
{
    // Check each child directory
    for (int i = 0; i < currentDir->nbChild; ++i) {
        if (strcmp(currentDir->childs[i].name, name) == 0) {
            return &currentDir->childs[i];
        }
    }

    // If fail
    return NULL;
}

/**
 * Process a CD instruction
 * @param  p_data The pointer to the next char
 * @return        The value
 */
int process_change_dir(char **p_data)
{   
    // Setup vars
    char *data = *p_data;

    // Check is ..
    if(*data == '.' && *(data+1) == '.') {
        skip_line(p_data);
        currentDir = currentDir->last;
        return 1;
    }

    // Get the directory name
    char buffer[MAX_ARG_SIZE];
    get_string(buffer, &data, '\n');
    skip_line(p_data);

    // Get the directory
    dir_t *newDir = get_subdir_by_name(buffer);
    if(newDir == NULL){
        return 0;
    }

    // Change directory and return
    currentDir = newDir;
    return 1;
}

int process_list(char **p_data)
{
    char buffer[MAX_ARG_SIZE];

    while (**p_data) {
        get_string(buffer, p_data, ' ');

        if (*buffer == '$'){
            (*p_data) --;
            return 1;
        }

        (*p_data)++;

        if(strcmp(buffer, "dir") == 0){
            get_string(buffer, p_data, '\n');
            if (!add_sub_directory(buffer))
                return 0;
        } else {
            currentDir->size += atoi(buffer);
        }
        skip_line(p_data);
    }
    return 0;
}

/**
 * Clear the current directory
 * @param dir The directory
 */
void clear_directory(dir_t *dir)
{
    // Clear sub directories
    for (int i = 0; i < dir->nbChild; ++i) {
        clear_directory(&dir->childs[i]);
    }
        
    // Deallocate memory
    free(dir->childs);
}

int process_line(char **p_data)
{
    command_type cmd = get_command_type(p_data);
    if(cmd == NONE) {
        fprintf(stderr, "Fail to read command...\n");
        return 0;
    }

    if(cmd == CHANGE_DIR) {
        if(!process_change_dir(p_data)){
            return 0;
        }
    } else if (cmd == LIST) {
        if(!process_list(p_data)){
            return 0;
        }
    } else {
        printf("Fail to read file data: %s\n", *p_data);
        return 0;
    }
    return 1;
}

int process_dir(dir_t *dir)
{
    int thisSize = dir->size;
    for (int i = 0; i < dir->nbChild; ++i) {
        int childSize = process_dir(&dir->childs[i]);
        if(childSize <= MAX_SIZE)
            result += childSize;
        thisSize += childSize;
    }
    return thisSize;
}

/**
 * The function that start to process the data
 * @param data A c_str that contains the input
 */
void start(char *data)
{
    // Skip the "cd /" command
    skip_line(&data);


    while (process_line(&data))
        ;;

    int size = process_dir(&root);
    if(size <= MAX_SIZE){
        result += size;
    }

    // Clean memory
    end: ;
    clear_directory(&root);
}



/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"