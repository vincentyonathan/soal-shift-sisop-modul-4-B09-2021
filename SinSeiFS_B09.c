#define FUSE_USE_VERSION 28
#include <stdio.h>
#include <fuse.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirp = "/home/prk/Downloads";
int tempchiper;

void clear(char* b) 
{
    int i;
    for (i = 0; i < BUFSIZ; i++)
        b[i] = '\0';
}

void atcip(char* temp)
{
	int i = 0;

    while (temp[i] != '\0')
    {
        if(temp[i] >= 'A' && temp[i] <= 'Z')
        {
            temp[i] = 'Z'+'A' - temp[i]; 
        }
        else if(temp[i] >= 'a' && temp[i] <= 'z')
        {
            temp[i] = 'z'+'a' - temp[i];
        }
        i++;
    }
//    for(j=0;j<strlen(temp);j++)
//    {
//    	printf("%c",temp[j]);	
//	}
    // char* ret = malloc(sizeof(char) * 1000) ;
    // strcpy(ret,temp);
    // return ret;
}

char* polapath(char* path)
{
 
    char *fpath = malloc(sizeof(char) * 1000);
    char real_path[100];
    strcpy(real_path, path);

    char *str = strstr(real_path, "/AtoZ_");

    if (str)
    {
        tempchiper = 1;
        int index = strlen(real_path) - strlen(str) + 1;
        while (index < strlen(real_path))
        {
            if (real_path[index] == '/')
            {
                atcip(&real_path[index]);
                DIR* dp = opendir(real_path);
                if(dp)
                {
                    break;
                }
                else
                {
                    char* file = strrchr(real_path,'/')+1;
                    char* extensi = strchr(file,'.');

                    if(extensi)
                    {
                        extensi += 1;
                        atcip(extensi);
                    }
                }
                
            }
            index++;
        }
        sprintf(fpath, "%s%s", dirp, real_path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirp, real_path);
    }
    
    return fpath;
}


static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    printf("GETATTR PATH--> %s\n",path);
    char fpath[BUFSIZ];
    char tempp[BUFSIZ];
    strcpy(tempp,path);
    strcpy(fpath,polapath(tempp));
    int res;
    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;
    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    printf("READDIR PATH--> %s\n",path);
    DIR *dp;
    int res;
    char tempp[BUFSIZ];
    tempchiper = 0;
    struct dirent *de;
    (void) offset;
    (void) fi;

    strcpy(tempp,path);
    dp = opendir(polapath(tempp));

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        char temp[BUFSIZ];

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(tempchiper == 1)
        {
            if(de->d_type & DT_DIR)
            {
                clear(temp);
                strcpy(temp,de->d_name);
                atcip(temp);
                res = (filler(buf, temp, &st, 0));
            }
            else
            {
                char* ext = strchr(de->d_name,'.');

                char fname[BUFSIZ];
                clear(fname);
                if(ext)
                {
                    strncpy(fname,de->d_name, strlen(de->d_name) - strlen(ext));
                    atcip(fname);
                    strcat(fname,ext);
                }
                else
                {
                    strcpy(fname,de->d_name);
                }

                res = (filler(buf, fname, &st, 0));
            }
        }
        else
        {
            strcpy(temp,de->d_name);
            res = filler(buf, temp, &st, 0);
        }

        if(res != 0) 
        {
            break;
        }
    }
    closedir(dp);
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
   
    char temppath[BUFSIZ];
    char tempp[BUFSIZ];
    char tempp2[BUFSIZ];
    strcpy(tempp,path);
    strcpy(tempp2,dirp);

    strcat(tempp2,tempp);
    strcpy(temppath,tempp2);

    char fullpath[BUFSIZ];
    int res;
    clear(fullpath);
    strcpy(fullpath, polapath(tempp));

	res = mkdir(fullpath, mode);
	if (res == -1)
		return -errno;

	return 0;

}

static int xmp_rename(const char *lama, const char *baru)
{
    char tempsebelum[BUFSIZ];
    char tempsetelah[BUFSIZ];
    clear(tempsebelum);
    clear(tempsetelah);
    sprintf(tempsebelum, "%s%s", dirp, lama);
    sprintf(tempsetelah, "%s%s", dirp, baru);

	int res;

	res = rename(tempsebelum, tempsetelah);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    (void) fi;

    fd = open(path, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}



static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
    .mkdir = xmp_mkdir,
};



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}