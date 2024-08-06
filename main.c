#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COMMANDS_FILE "~/.TerminalIllness/user_commands"
#define HELP_FILE "~/.TerminalIllness/docs/help"
#define VERSION "Terminal Illness version 202408 Beta"

void print_help();
void store_command(const char *command, const char *keywords);
void find_command(const char *keywords);
void execute_command(const char *command);
void load_commands(const char *filepath, char *buffer, size_t size);
void save_command_to_file(const char *command, const char *keywords);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("%s : store, recall, and execute useful shell commands\n", VERSION);
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
    } else if (strcmp(argv[1], "-s") == 0 && argc > 3) {
        store_command(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-f") == 0 && argc > 2) {
        find_command(argv[2]);
    } else {
        printf("%s : store, recall, and execute useful shell commands\n", VERSION);
    }

    return 0;
}

void print_help() {
    char buffer[1024];
    load_commands(HELP_FILE, buffer, sizeof(buffer));
    printf("%s\n", buffer);
}

void store_command(const char *command, const char *keywords) {
    save_command_to_file(command, keywords);
    printf("Command '%s' stored with keywords '%s'.\n", command, keywords);
}

void save_command_to_file(const char *command, const char *keywords) {
    FILE *file = fopen(COMMANDS_FILE, "a");
    if (file == NULL) {
        perror("Could not open commands file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s %s\n", keywords, command);
    fclose(file);
}

void find_command(const char *keywords) {
    char buffer[1024];
    load_commands(COMMANDS_FILE, buffer, sizeof(buffer));

    char *token = strtok(buffer, "\n");
    while (token != NULL) {
        if (strstr(token, keywords) != NULL) {
            printf("Command found: %s\n", token);
            // In a full implementation, prompt user for which command to execute
            // execute_command(selected_command);
        }
        token = strtok(NULL, "\n");
    }
}

void execute_command(const char *command) {
    // Strip back-ticks and execute the command
    char exec_command[1024];
    snprintf(exec_command, sizeof(exec_command), "%s", command);
    system(exec_command);
}

void load_commands(const char *filepath, char *buffer, size_t size) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Could not open commands file");
        exit(EXIT_FAILURE);
    }

    fread(buffer, sizeof(char), size, file);
    fclose(file);
}
