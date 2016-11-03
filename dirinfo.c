#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

off_t total_size = 0;

//to print total size (at the end)
void print_size(float size) {
  //approach borrowed from dubx+grace's stat hw
  char units[4][4] = {"B", "KB", "MB", "GB"}; int i = 0;
  while(size / 1024 > 1) {
    size = size / 1024;
    i++;
  }
  printf("Total size: %.1f %s\n", size, units[i]);
}

//get size of an individual file in a directory
off_t get_size(char *dirname, struct dirent *dir) {
  struct stat *buf = (struct stat *)malloc(sizeof(struct stat));
  char filename[256];
  strcpy(filename, dirname);
  strcat(filename, "/");
  strcat(filename, dir->d_name);
  stat(filename, buf);
  return buf->st_size;
}

//print all of the file/dir names in a directory
void print_names(char *dirname) {
  //open directory file & initialize dirent struct
  DIR *d = opendir(dirname);
  struct dirent *dir;

  //print name of current directory
  printf("%s:\n", dirname);

  //while there are things in the directory to be read through
  while( (dir = readdir(d)) != 0 ) {
    //add size of current file to total_size
    total_size += get_size(dirname, dir);

    //print filename
    printf("%s", dir->d_name);
    if(dir->d_type == 4) printf("/");
    printf("\t");
  }

  //close directory file
  closedir(d);

  //for formatting stuffs
  printf("\n\n");
}

//recursively print the contents of a directory (works like ls -R)
void print_dir(char *dirname) {
  //first print all the names at the top level of given directory
  print_names(dirname);

  //then initialize new dirent struct
  DIR *d = opendir(dirname);
  struct dirent *dir;

  //while there are things in the directory to be read through
  while( (dir = readdir(d)) != 0 ) {

    //recursive call: if a thing in a directory is a subdirectory, call print_dir on it (ignore hidden files & dirs)
    if(dir->d_type == 4 && strncmp(dir->d_name, ".", 1) != 0) {
      char new_dir[256] = "";
      strcat(new_dir, dirname);
      strcat(new_dir, "/");
      strcat(new_dir, dir->d_name);
      print_dir(new_dir);
    }
  }

  //close directory file
  closedir(d);
}

int main(){
  printf("-------------------\n");

  char dirname[256] = ".";
  print_dir(dirname);
  print_size((float)total_size);
  return 0;
}
