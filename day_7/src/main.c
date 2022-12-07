#define AOC_IN_FILE "data.in"
#define AOC_OUT_FILE "data.out"
#define AOC_OUT_FORMAT "%d\n"

#include <limits.h>

static int result = INT_MAX;

#include "aocapp.h"

/* ===== AoC INCLUDES ===== */

#include <string.h>

#define DISK_SIZE 70000000
#define UPDATE_SIZE 30000000
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
static int needed_size = 0;

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

/**
 * Process a list request
 * @param  p_data The pointer to the data (will be modified)
 * @return        1 if success, else fail
 */
int process_list(char **p_data)
{
    // Set buffer
    char buffer[MAX_ARG_SIZE];

    // Check will not try to read after end of string
    while (**p_data) {
        // Get first word
        get_string(buffer, p_data, ' ');

        // If is start of a user command, quit
        if (*buffer == '$'){
            (*p_data) --;
            return 1;
        }

        // Increment the data pointer
        (*p_data)++;

        // Check request type
        if(strcmp(buffer, "dir") == 0){ 
            // If is a dir, will add it in system
            get_string(buffer, p_data, '\n');
            if (!add_sub_directory(buffer))
                return 0;
        } else {
            // Else, it is a file, just get size
            currentDir->size += atoi(buffer);
        }

        // Finally, skip line
        skip_line(p_data);
    }

    // In case the ls command is the last
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

/**
 * Process an input line
 * @param  p_data The pointer to the data (Will be modifed)
 * @return        1 if success, else 0
 */
int process_line(char **p_data)
{
    // Get command
    command_type cmd = get_command_type(p_data);
    if(cmd == NONE) {
        fprintf(stderr, "Fail to read command...\n");
        return 0;
    }

    // Check type
    if(cmd == CHANGE_DIR) {
        // If is a 'cd' command
        if(!process_change_dir(p_data)){
            return 0;
        }
    } else if (cmd == LIST) {
        // If is a 'ls' command
        if(!process_list(p_data)){
            return 0;
        }
    } else {
        // If it fail to find the type of command
        // (will normally never reach it)
        printf("Fail to read file data: %s\n", *p_data);
        return 0;
    }
    return 1;
}

/**
 * Get the size of a directory (with his sub directories)
 * @param  dir The directory
 * @return     The size
 */
int get_dir_size(dir_t *dir)
{
    // Set result var
    int thisSize = dir->size;

    // Add size of each child dir
    for (int i = 0; i < dir->nbChild; ++i) {
        thisSize += get_dir_size(&dir->childs[i]);
    }

    // Return
    return thisSize;
}

/**
 * Process a directory to get the result
 * @param  dir The directory
 * @return     [description]
 */
int process_dir(dir_t *dir)
{
    // Set the result var
    // Used to get the current size... THIS IS BAD CODE! 
    int thisSize = dir->size;

    // For each child
    for (int i = 0; i < dir->nbChild; ++i) {
        int childSize = process_dir(&dir->childs[i]);
        // Check if the child size is enouth big to
        // cover the update and is the smallest for
        // the moment
        if(childSize < result && childSize >= needed_size){
            result = childSize;
        }
        thisSize += childSize;
    }

    // Return
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

    // Read input to setup the FS
    while (process_line(&data))
        ;;

    // Calucate result (The logic I used is in the next comment)
    int root_size = get_dir_size(&root);
    needed_size = root_size - (DISK_SIZE - UPDATE_SIZE);
    process_dir(&root);

/*  I think this draw intresting things about the 
    situation but is not needed for this application...

    printf("DISK   = %9d\n", DISK_SIZE);
    printf("UPDATE = %9d\n", UPDATE_SIZE);
    printf("FS     = %9d\n", root_size);
    printf("MAX FS = %9d\n", DISK_SIZE - UPDATE_SIZE);
    printf("FS REM = %9d\n", needed_size);
    printf("REMOVE = %9d\n", result);
*/

    clear_directory(&root);
}

/* ===== MAIN FUNCTION ===== */

#include "aocmain.h"