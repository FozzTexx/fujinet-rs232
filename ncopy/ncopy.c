/* FujiNet network file copier
 * Contributed by fozztexx@fozztexx.com
 */

#include "parser.h"
#include "fuji.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>

//#include "../sys/print.h" // debug

char buf[256];

void print_dir();
void get_password(char *password, size_t max_len);

void main(int argc, char *argv[])
{
  char *url = argv[1];
  int err;
  parsed cmd;
  int done = 0;


  if (argc < 2) {
    printf("Usage: %s URL\n", argv[0]);
    exit(1);
  }
  
  err = fuji_open_url(url, NULL, NULL);
  if (err) {
    // Maybe authentication is needed?
    printf("User: ");
    fgets(buf, 128, stdin);
    printf("Password: ");
    fflush(stdout);
    get_password(&buf[128], 128);

    err = fuji_open_url(url, buf, &buf[128]);
    if (err) {
      printf("Err: %i unable to open URL: %s\n", err, url);
      exit(1);
    }
  }

  // Opened succesfully, we don't need it anymore
  err = fuji_close_url();

  // Tell FujiNet to remember it was open
  fuji_chdir(url);
  
  while (!done) {
    printf("ncopy> ");
    fflush(stdout);
    fgets(buf, sizeof(buf), stdin);
    if (!buf[0] || buf[0] == '\n')
      continue;

    cmd = parse_command(buf);
    switch (cmd.cmd) {
    case CMD_DIR:
      print_dir();
      break;

    case CMD_GET:
      printf("get the file\n");
      break;

    case CMD_PUT:
      printf("put the file\n");
      break;

    case CMD_CD:
      fuji_chdir(cmd.args[1]);
      break;

    case CMD_EXIT:
      done = 1;
      break;

    default:
      printf("Unrecognized command\n");
      break;
    }
  }
  
  exit(0);
}

void print_dir()
{
  errcode err;
  size_t len;
  FN_DIRENT *ent;
  struct tm *tm_p;


  err = fuji_opendir();
  if (err) {
    printf("Unable to read directory\n");
    return;
  }

#if 1
  while ((ent = fuji_readdir())) {
    tm_p = localtime(&ent->mtime);
    strftime(buf, sizeof(buf) - 1, "%Y-%b-%d %H:%M", tm_p);
    if (ent->isdir)
      printf("%-14s  <DIR>  %s\n", ent->name, buf);
    else
      printf("%-14s %7li %s\n", ent->name, ent->size, buf);
  }
#else
  for (;;) {
    len = fuji_read(buf, sizeof(buf));
    if (!len)
      break;
    printf("%.*s", len, buf);
  }
#endif
  printf("\n");

  fuji_closedir();

  return;
}

void get_password(char *password, size_t max_len)
{
  size_t idx = 0;
  char ch;


  while (idx < max_len - 1) {
    ch = getch();

    if (ch == '\r' || ch == '\n')
      break;

    // Handle backspace/delete
    if (ch == '\b' || ch == 127) {
      if (idx) {
	// Erase '*' from the screen
        printf("\b \b");
	fflush(stdout);
        idx--;
      }
    }
    else {
      password[idx++] = ch;
      printf("*");
      fflush(stdout);
    }
  }

  password[idx] = 0;
  printf("\n");
  return;
}
