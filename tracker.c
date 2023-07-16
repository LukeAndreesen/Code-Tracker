#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cJSON.h>

int MAX_LINE_LENGTH = 1000;
int MAX_ENTRY_LENGTH = 500;
int MAX_FILES = 1000;
int MAX_DIRECTORY_LENGTH = 100;
int MAX_FILE_NAME_LENGTH = 50;
int MAX_COMMAND_LENGTH = 150;

int LANGUAGE_COLUMN = 0;
int FILENAME_COLUMN = 1;
int LINES_COLUMN = 4;
struct str_int{

    char *language;
    char *filename;
    int num_lines;
};
typedef struct str_int str_int;

str_int *construct(char *language, char *filename, int num_lines){
    str_int *result = (str_int *)malloc(sizeof(str_int));
    result -> language = language;
    result -> filename = filename;
    result -> num_lines = num_lines;
    return result;
}

int header_row(char *filename)
{
    FILE *file;    
    file = fopen(filename, "r");
    int row_count = 0;
    char row[MAX_LINE_LENGTH];
    char *first_column = (char *)malloc(MAX_ENTRY_LENGTH);

    while(!feof(file)){
        fgets(row, MAX_LINE_LENGTH, file);
        first_column = strtok(row, ",");
        if (strcmp("language", first_column) == 0) return row_count;
        row_count++;
    }
    fclose(file);
    return 0;
}

str_int **language_lines(char *data_file, int *size){
    str_int **result = (str_int **)malloc(sizeof(str_int *) * MAX_FILES);
    FILE *file;
    file = fopen(data_file, "r");
    int header = header_row(data_file);

    char *token = (char *)malloc(MAX_ENTRY_LENGTH);
    char row[MAX_LINE_LENGTH];
    
    int row_count = 0;
    while (!feof(file)){
        if (row_count <= header){
            row_count++;
            fgets(row, MAX_LINE_LENGTH, file);
            continue; //ignore header
        }   
        fgets(row, MAX_LINE_LENGTH, file);
        token = strtok(row, ",");
        int column_count = 0;
        char *language;
        char *filename;

        int lines;

        while(token != NULL){
            if (column_count == LANGUAGE_COLUMN) language = strdup(token);
            else if (column_count == FILENAME_COLUMN) filename = strdup(token);
            else if (column_count == LINES_COLUMN) lines = atoi(token); 
            token = strtok(NULL, ",");
            column_count++;
        }
        result[row_count-header -1 ] = construct(language, filename, lines);
        row_count++;
    }
    *size = row_count - header -1; //out parameter 
    fclose(file);
    free(token);
    return result;

}

void display_data(str_int **data, int size){
    for(int i = 0; i<size; i++)
    {
        printf("Filename: %s\n", data[i] -> filename);
        printf("Language: %s\n", data[i] -> language);
        printf("Number of lines: %d\n", data[i] -> num_lines);
    }
}

void data_to_json(str_int **data, int size){
    cJSON *root = cJSON_CreateObject();

    for (int i = 0; i < size; i++) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "language", data[i] -> language);
        cJSON_AddStringToObject(item, "filename", data[i] -> filename);
        cJSON_AddNumberToObject(item, "lines", data[i] -> num_lines);
        cJSON_AddItemToArray(root, item);
    }

    char *json_str = cJSON_Print(root);
    cJSON_Delete(root);

    FILE *file = fopen("data.json", "w");
    if (file != NULL) {
        fprintf(file, "%s", json_str);
        fclose(file);
        printf("JSON data written to data.json file.\n");
    } else {
        printf("Error opening data.json file for writing.\n");
    }
    free(json_str);
}

void display_json_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *json_str = (char *)malloc(file_size + 1);
        if (json_str != NULL) {
            fread(json_str, file_size, 1, file);
            json_str[file_size] = '\0';

            printf("JSON file contents:\n%s\n", json_str);

            free(json_str);
        } else {
            printf("Memory allocation error.\n");
        }

        fclose(file);
    } else {
        printf("Error opening %s file for reading.\n", filename);
    }
}

int main() {
    char *command = (char *)malloc(MAX_COMMAND_LENGTH);
    strcat(command, "cloc --by-file -csv ");
    char *directory = (char *)malloc(MAX_DIRECTORY_LENGTH);
    printf("Enter a directory to scan \n");
    scanf("%99s", directory);

    strcat(command, directory);
    strcat(command, " > result.csv");
    system(command);

    int arr_size;
    str_int **result = language_lines("result.csv", &arr_size);
    display_data(result, arr_size);
    data_to_json(result, arr_size);
 //   display_json_file("data.json");


    free(command);
    free(directory);

    return 0;
}
