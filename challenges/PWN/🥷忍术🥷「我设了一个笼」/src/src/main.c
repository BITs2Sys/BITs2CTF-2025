#include "./log.h"
#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

void banner() {
  int dirfd = open("./", 0);

  if (dirfd < 0) {
    exit(1);
  }

  puts(CYAN "        |===================|" YELLOW "      ~*~*~*~" CYAN);
  puts("        | 🥷 BITs2CTF  ~ 🥷 |" YELLOW "  ~*~*~*~" CYAN);
  puts("        |===================|================|");
  puts("                      | 忍术：我设了一个笼！ |");
  puts("                      |======================|");
  puts("                                /" END);

  int file = open("./banner.logo", 0);
  struct stat *bstat = malloc(sizeof(struct stat));

  fstat(file, bstat);

  int size = bstat->st_size;
  char *banner = malloc(size);
  read(file, banner, size);
  puts(banner);

  puts(YELLOW "       ------------------------------------");
  puts("        || 🥷 | 🥷 | 🥷 | 🥷 | 🥷 | 🥷 ||");
  puts("        || 🥷 | 🥷 | 🥷 | 🥷 | 🥷 | 🥷 ||");
  puts("        || 🥷 | 🥷 | 🥷 | 🥷 | 🥷 | 🥷 ||");
  puts("       ------------------------------------" END);

  free(banner);
  free(bstat);
  close(file);
}

const char *jail = "../jail";

const char *prompt_n = GREEN "(jail)" PURPLE " $ " END;
const char *prompt_f = RED "(jail)" PURPLE " $ " END;

void *addr = (void *)0x495000;

size_t buf_size = 0x200l;
char buffer[0x200l];
int8_t cwd;

int cmd_cat(char *restrict name) {
  if (name == NULL) {
    return 1;
  }

  char *tmp = strdup(name);
  sprintf(name, "./%s", tmp);
  free(tmp);

  // dbg("%s", name);

  const uint64_t block_sz = 0x1000l;
  off_t off = 0;

  struct stat *stat_buf = malloc(sizeof(struct stat));

  int fd = openat(cwd, name, 0);

  if (fd < 0) {
    return -1;
  }

  fstat(fd, stat_buf);

  uint64_t file_sz = stat_buf->st_size;

  char *buf = malloc(block_sz);
  while (file_sz >= block_sz) {
    if (sendfile(1, fd, &off, block_sz) < 0) {
      return -1;
    };
    file_sz -= block_sz;
    off += block_sz;
  }

  if (file_sz > 0) {
    if (sendfile(1, fd, &off, file_sz) < 0) {
      return -1;
    };
  }

  free(buf);
  free(stat_buf);
  close(fd);

  return 0;
}

int cmd_ls(char *restrict path) {
  if (path == NULL) {
    path = ".";
  }

  struct dirent *entry;
  DIR *dir = opendir(path);

  if (dir <= 0) {
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);

  return 0;
}

int cmd_cd(char *restrict path) {
  if (path == NULL) {
    return -1;
  }

  return chdir(path);
}

int cmd_echo(char *restrict msg) {
  if (msg == NULL) {
    return -1;
  }

  puts(msg);
  return 0;
}

int cmd_help() {

  const char *help = "Command list: ls, cat, echo, help\n\n\
ls: show directory\n\
    Usage: ls [PATH] or ls\n\n\
cat: show file's content\n\
    Usage: cat [FILE]\n\n\
cd: change directory\n\
    Usage: cd [PATH]\n\n\
echo: display a message\n\
    Usage: echo [MSG]\n\n\
help: show this help\n\
    Usage: help\n";
  fputs(help, stderr);
  return 0;
}

int get_cmd(char *cmd, char *args) {
  // dbg("Your cmd: %s\n", cmd);
  // dbg("Your args: %s\n", args);

  if (!strcmp(cmd, "cat")) {
    char *name = strtok(NULL, " ");
    return cmd_cat(name);
  } else if (!strcmp(cmd, "ls")) {
    char *path = strtok(NULL, " ");
    return cmd_ls(path);
  } else if (!strcmp(cmd, "cd")) {
    char *path = strtok(NULL, " ");
    return cmd_cd(path);
  } else if (!strcmp(cmd, "echo")) {
    return cmd_echo(args);
  } else if (!strcmp(cmd, "help")) {
    return cmd_help();
  } else {
    printf("Unknown command: %s\n", cmd);
    return -1;
  };

  return EXIT_SUCCESS;
}

void __attribute__((constructor)) init() {
  setbuf(stdin, 0);
  setbuf(stdout, 0);
  setbuf(stderr, 0);
  cwd = AT_FDCWD;
  banner();
}

int main() {

  if (chroot(jail) < 0) {
    return -1;
  };

  if (chdir("/") < 0) {
    return -1;
  };

  printf("%s", prompt_n);
  while (1) {
    if (fgets(buffer, buf_size, stdin) == NULL) {
      puts("exit");
      return 0;
    };

    int last = strlen(buffer) - 1;

    if (buffer[last] == '\n') {
      buffer[last] = '\0';
    }

    char *cmd = strtok(buffer, " ");

    if (cmd == NULL) {
      continue;
    }

    char *args = buffer + strlen(cmd) + 1;

    if (get_cmd(cmd, args) < 0) {
      printf("%s", prompt_f);
    } else {
      printf("%s", prompt_n);
    };
  }
}
