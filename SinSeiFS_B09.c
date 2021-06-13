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
int temprx = 0;
char* key = "SISOP";
static const char *myLOG = "/home/prk/Praktikum4/SinSeiFS.log";

void WarningLog(char* cmd_desc, char* path) {
	FILE *txt;
	txt = fopen(myLOG, "a");
 
	time_t rwtm = time(NULL);
 
	struct tm tm = *localtime(&rwtm);
 
	int theYear = tm.tm_year+1900, theMonth = tm.tm_mon+1,
	theDay = tm.tm_mday, theHour = tm.tm_hour,
	theMinunte = tm.tm_min, theSecond = tm.tm_sec;
 
	fprintf(txt, "WARNING::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", theDay, theMonth, theYear, theHour, theMinunte, theSecond, cmd_desc, path);
	fclose(txt);
}

void InfoLog(char* cmd_desc, char* path) {
	FILE *txt;
	txt = fopen(myLOG, "a");
 
	time_t rwtm = time(NULL);
 
	struct tm tm = *localtime(&rwtm);
 
	int theYear = tm.tm_year+1900, theMonth = tm.tm_mon+1,
	theDay = tm.tm_mday, theHour = tm.tm_hour,
	theMinunte = tm.tm_min, theSecond = tm.tm_sec;
 
	fprintf(txt, "INFO::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", theDay, theMonth, theYear, theHour, theMinunte, theSecond, cmd_desc, path);
	fclose(txt);
}

void ViginereE(char *temp)
{
    int i = 0;
    int j = 0;
    while (temp[i] != '\0' && temp[i] != '.')
    {  
        if(temp[i] == '/')
        {
            i++;
            j = 0;
            continue;
        }
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
        {
            temp[i] = ((temp[i] - 'A') + (key[j % strlen(key)] - 'A')) % 26 + 'A';
        }
        else if (temp[i] >= 'a' && temp[i] <= 'z')
        {
            temp[i] = ((temp[i] - 'a') + (key[j % strlen(key)] - 'A')) % 26 + 'a';
        }

        j++;
        i++;
    }
}

void ViginereD(char *temp)
{
    int i = 0;
    int j = 0;
    while (temp[i] != '\0' && temp[i] != '.')
    {
        if(temp[i] == '/')
        {
            i++;
            j = 0;
            continue;
        }
        else if (temp[i] >= 'A' && temp[i] <= 'Z')
        {
            temp[i] = ((temp[i] - 'A') - (key[j % strlen(key)] - 'A') + 26) % 26 + 'A';
            j++;
        }
        else if (temp[i] >= 'a' && temp[i] <= 'z')
        {
            temp[i] = ((temp[i] - 'a') - (key[j % strlen(key)] - 'A') + 26) % 26 + 'a';
            j++;
        }
        i++;
    }
}

int crlog2(char *str)
{
    char line[1000];
    
    FILE *fp = fopen("running2.log", "r");

    if(!fp) 
    {
        return 0;
    }

    while (fgets(line, sizeof(line), fp))
    {
        printf("Ini Line --> %s",line);
        printf("ini str --> %s\n",str);
        if (strncmp(line, "[rename]", 8))
        {
            continue;
        }

        line[strcspn(line, "\n")] = '\0';
        char *path = strrchr(line, ' ') + 1;
        char *name = strstr(path, "/RX_");
        printf("ini name --> %s\n",name);

        if (!name)
        {
            continue;
        }

        char name2[1000];
        strcpy(name2, str);

        printf("ini name2 --> %s\n",name2);

        char* temptok1 = strtok(name, "/") ;
        char* temptok2 = strtok(name2, "/");
        printf("ini temptok1 --> %s\n",temptok1);
        printf("ini temptok2 --> %s\n",temptok2);
        if (!strcmp(temptok1, temptok2))
        {
            return 1;
        }
    }

    return 0;
}

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

void rot13(char* temp)
{
    int i=0;
    while(temp[i]!= '\0' && temp[i]!= '.')
    {
        if(temp[i] >= 'A' && temp[i] <= 'Z' )
    	{
    	   if(temp[i]+13 >= 'A' && temp[i]+13 <= 'Z' )
    	   {
    	    //    printf("temp[i] --> %c\n",temp[i]+13);
    	       temp[i] = temp[i]+13;
    	   }
    	   else
    	   {
    	    //    printf("ini else\n");
    	        temp[i] = ((temp[i] - 'A') + 13) % 26;
    	        temp[i] = 'A' + temp[i];
    	   }
    	}
    	else if (temp[i] >= 'a' && temp[i] <= 'z' )
    	{
    	    if(temp[i]+13 >= 'a' && temp[i]+13 <= 'z' )
    	   {
    	       temp[i] = temp[i]+13;
    	   }
    	   else
    	   {
    	        temp[i] = ((temp[i] - 'a') + 13) % 26;
    	        temp[i] = 'a' + temp[i];
    	   }
    	}
    	// printf("ini temp [i] %c\n", temp[i]);
        i++;
    }
}

void rlog (char dir_lama[], char dir_baru[], int tipe)
{
    FILE* logge = fopen("running.log","a");

    if(tipe == 0)
    {
        fprintf(logge,"[mkdir]: %s\n", dir_baru);
    }
    else
    {
        fprintf(logge,"[rename]: %s -> %s\n", dir_lama,dir_baru);
    }

    fclose(logge);
}

void rlog2 (char dir_lama[], char dir_baru[], int tipe)
{
    FILE* logge2 = fopen("running2.log","a");

    if(tipe == 0)
    {
        fprintf(logge2,"[mkdir]: %s\n", dir_baru);
    }
    else
    {
        fprintf(logge2,"[rename]: %s -> %s\n", dir_lama,dir_baru);
    }

    fclose(logge2);
}

char* polapath(char* path)
{
 
    char *fpath = malloc(sizeof(char) * 1000);
    char real_path[1000];
    strcpy(real_path, path);

    char *str = strstr(real_path, "/AtoZ_");
    char *rex = strstr(real_path, "/RX_");

    if(rex)
    {
        temprx = 1;
        int index = strlen(real_path) - strlen(rex) + 1;
        int templog = crlog2(&real_path[index]);
        while (index < strlen(real_path))
        {
            if (real_path[index] == '/')
            {
                if(templog)
                {
                    ViginereD(&real_path[index]);
                    atcip(&real_path[index]);
                }
                else
                {
                    rot13(&real_path[index]);
                    atcip(&real_path[index]);
                }
                
                // printf("ini index --> %d\n ",index);
                char dirp_temp[1000];
                sprintf(dirp_temp, "%s%s", dirp, real_path);
                // printf("Ini folder dengan path --> %s\n",dirp_temp);
                DIR* dp = opendir(dirp_temp);
                if(dp)
                {
                    break;
                }
                else
                {
                    char* file = strrchr(real_path,'/')+1;
                    char* extensi = strchr(file,'.');
                    //printf("Ini file--> %s%s\n",real_path,file);
                    if(extensi)
                    {
                        extensi += 1;
                        atcip(extensi);
                    }
                    break;
                }
                closedir(dp);
            }
            index++;
        }
        sprintf(fpath, "%s%s", dirp, real_path);
    }

    else if (str)
    {
        tempchiper = 1;
        int index = strlen(real_path) - strlen(str) + 1;
        while (index < strlen(real_path))
        {
            if (real_path[index] == '/')
            {
                // printf("ini index --> %d\n ",index);
                atcip(&real_path[index]);
                char dirp_temp[1000];
                sprintf(dirp_temp, "%s%s", dirp, real_path);
                // printf("Ini folder dengan path --> %s\n",dirp_temp);
                DIR* dp = opendir(dirp_temp);
                if(dp)
                {
                    break;
                }
                else
                {
                    char* file = strrchr(real_path,'/')+1;
                    char* extensi = strchr(file,'.');
                    printf("Ini file--> %s%s\n",real_path,file);
                    if(extensi)
                    {
                        extensi += 1;
                        atcip(extensi);
                
                    }
                    break;
                }
                closedir(dp);
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
    //printf("GETATTR PATH--> %s\n",path);
    char fpath[BUFSIZ];
    char tempp[BUFSIZ];
    strcpy(tempp,path);
    // printf("ini tempp getattr %s\n",tempp);
    strcpy(fpath,polapath(tempp));
    int res;
    res = lstat(fpath, stbuf);

    InfoLog("LS", fpath);
    if (res == -1) return -errno;
    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    // printf("READDIR PATH--> %s\n",path);
    DIR *dp;
    int res;
    char tempp[BUFSIZ];
    tempchiper = 0;
    temprx = 0;
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
        
        if(strcmp (de->d_name,".")==0|| strcmp(de->d_name,"..") == 0)
        {
            res = filler(buf,de->d_name,&st,0);
        }
        else if(tempchiper == 1)
        {
            if(de->d_type & DT_DIR)
            {
                printf("[chiper]Ini folder --> %s\n",de->d_name);
                clear(temp);
                strcpy(temp,de->d_name);
                atcip(temp);
                res = (filler(buf, temp, &st, 0));
            }
            else
            {
                char* ext = strchr(de->d_name,'.');

                char fname[BUFSIZ];
                bzero(fname,BUFSIZ);
                if(ext)
                {
                    printf("[chiper]Ini file dengan ext --> %s\n",de->d_name);
                    strncpy(fname,de->d_name, strlen(de->d_name) - strlen(ext));
                    atcip(fname);
                    strcat(fname,ext);
                }
                else
                {
                    printf("[cipher]File without extention --> %s\n",de->d_name);
                    strcpy(fname,de->d_name);
                    atcip(fname);
                }

                res = (filler(buf, fname, &st, 0));
            }
        }
        
        else if(temprx == 1)
        {
            if(de->d_type & DT_DIR)
            {
                // printf("Ini folder --> %s\n",de->d_name);
                clear(temp);
                strcpy(temp,de->d_name);
                if(crlog2(tempp))
                {
                    atcip(temp);
                    ViginereE(temp);
                }
                else
                {
                    atcip(temp);
                    rot13(temp);
                }
                printf("[rx]ini temp di rx --> %s\n", temp);
                res = (filler(buf, temp, &st, 0));
            }
            else
            {
                char* ext = strchr(de->d_name,'.');

                char fname[BUFSIZ];
                bzero(fname,BUFSIZ);
                if(ext)
                {
                    printf("[rx]Ini file dengan ext --> %s\n",de->d_name);
                    strncpy(fname,de->d_name, strlen(de->d_name) - strlen(ext));
                    if(crlog2(tempp))
                    {
                        atcip(fname);
                        ViginereE(fname);
                    }
                    else
                    {
                        atcip(fname);
                        rot13(fname);
                    }
                    printf("[rx]Ini file dengan ext terencode --> %s\n",de->d_name);
                    strcat(fname,ext);
                }
                else
                {
                    printf("[rx]File without extention --> %s\n",de->d_name);
                    strcpy(fname,de->d_name);
                    if(crlog2(tempp))
                    {
                        atcip(fname);
                        ViginereE(fname);
                    }
                    else
                    {
                        atcip(fname);
                        rot13(fname);
                    }
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
    // closedir(dp);
    InfoLog("CD", tempp);
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

    if(strstr(temppath, "/AtoZ_"))
    {
        rlog("none",temppath,0);
    }
    if(strstr(temppath,"/RX_"))
    {
        rlog2("none",temppath,0);
    }

    char fullpath[BUFSIZ];
    int res;
    clear(fullpath);
    strcpy(fullpath, polapath(tempp));

	res = mkdir(fullpath, mode);
	if (res == -1)
    {
        return -errno;
    }

    InfoLog("MKDIR", temppath);
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

    if(strstr(tempsetelah, "/AtoZ_"))
    {
        rlog(tempsebelum,tempsetelah,1);
    }
    if(strstr(tempsetelah,"/RX_"))
    {
        rlog2(tempsebelum,tempsetelah,1);
    }

	int res;

	res = rename(tempsebelum, tempsetelah);
	if (res == -1)
    {
        return -errno;
    }
    InfoLog("RENAME", tempsebelum);
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

static int xmp_rmdir(const char *path) {
	char fpath[1000];
	if (strcmp(path, "/") == 0) 
    {
        	path = dirp;
        	sprintf(fpath, "%s", path);
    	} else {
		sprintf(fpath, "%s%s", dirp, path);
	}
 
	int res;
	res = rmdir(fpath);
	if (res == -1) return -errno;
 
	WarningLog("RMDIR", fpath);
 
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) 
{
	char fpath[1000];
 
	if (strcmp(path, "/") == 0) {
        	path = dirp;
        	sprintf(fpath, "%s", path);
    	} else {
		sprintf(fpath, "%s%s", dirp, path);
	}
 
	int res;
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0) {
			res = close(res);
		}
	} else if (S_ISFIFO(mode)) {
		res = mkfifo(fpath, mode);
	} else {
		res = mknod(fpath, mode, rdev);
	}
 
	if (res == -1) return -errno;
	InfoLog("CREATE", fpath);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
	char fpath[1000];
 
	if (strcmp(path, "/") == 0) {
        	path = dirp;
        	sprintf(fpath, "%s", path);
    	} else {
		sprintf(fpath, "%s%s", dirp, path);
	}
 
    int fd;
	int res;
 
	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1) return -errno;
 
	res = pwrite(fd, buf, size, offset);
	if (res == -1) res = -errno;
 
	close(fd);
 
    	// logs(0, "WRITE", fpath, "");
	InfoLog("WRITE", fpath);
	return res;
}

static int xmp_unlink(const char *path) {
	char fpath[1000];
 
	if (strcmp(path, "/") == 0) {
        	path = dirp;
        	sprintf(fpath, "%s", path);
    	} else {
		sprintf(fpath, "%s%s", dirp, path);
	}
 
	int res;
	res = unlink(fpath);
	if (res == -1) return -errno;
 
	WarningLog("UNLINK", fpath);
 
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) 
{
	char fpath[1000];
 
	if (strcmp(path, "/") == 0) {
	        path = dirp;
	        sprintf(fpath, "%s", path);
    	} else {
		sprintf(fpath, "%s%s", dirp, path);
	}
 
	int res;
	res = open(fpath, fi->flags);
	if (res == -1) return -errno;
 
	close(res);
 
	InfoLog("OPEN", fpath);
 
	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
	.open = xmp_open,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir	    = xmp_rmdir,
};



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
