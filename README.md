# soal-shift-sisop-modul-4-B09-2021

* Vincent Yonathan    - 05111940000186
* Aprilia Annisa      - 05111940000199
* Pramudityo Prabowo  - 05111940000210
---
### Soal Modul 3
[Soal Shift Modul 4](https://docs.google.com/document/d/1KlAG2thAMm42so8BZMylw216EETVGu_DAM5PfwJzl5U/edit)

---

### Daftar isi
* [Soal 1](#soal-1)
* [Soal 2](#soal-2)
* [Soal 3](#soal-3)
* [Soal 4](#soal-4)

---
### Soal 1
*Praktikan* diminta membantu mengerjakan pembuatan filesystem dengan metode enkripsi (encode).
#### 1.a)
*Praktikan* diminta untuk membuat filesystem dimana bisa dilakukan mkdir(pembuatan folder) atau rename (penamaan ulang) dimana jika kedua itu dilakukan dan namanya diawali dengan "AtoZ_", maka nama folder beserta seluruh isinya akan terenkripsi dengan metode Atbash Cipher.
#### Source Code 
---
```c
...
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
    
char* polapath(char* path)
{
 
    char *fpath = malloc(sizeof(char) * 1000);
    char real_path[1000];
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
                printf("ini index --> %d\n ",index);
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
...
}
```
Fungsi utama dari soal ini adalah 2 fungsi diatas yaitu polapath() dan atcip()
- Fungsi atcip() diatas merupakan kode yang digunakan untuk melakukan Atbash Cipher yaitu dengan melakukan *mirror* kepada masing - masing `char`. Cara melakukannya adalah looping dari setiap `char` dari kalimat kemudian apabila `char` tersebut ada dalam range A sampai Z atau a sampai z, maka akan char itu akan dikurangkan dengan ('Z'+'A') atau ('z' + 'a')
- Fungsi polapath()

```c
...
static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    // printf("GETATTR PATH--> %s\n",path);
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
    // printf("READDIR PATH--> %s\n",path);
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
        
        if(strcmp (de->d_name,".")==0|| strcmp(de->d_name,"..") == 0)
        {
            res = filler(buf,de->d_name,&st,0);
        }
        else if(tempchiper == 1)
        {
            if(de->d_type & DT_DIR)
            {
                printf("Ini folder --> %s\n",de->d_name);
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
                    printf("Ini file dengan ext --> %s\n",de->d_name);
                    strncpy(fname,de->d_name, strlen(de->d_name) - strlen(ext));
                    atcip(fname);
                    strcat(fname,ext);
                }
                else
                {
                    printf("File without extention --> %s\n",de->d_name);
                    strcpy(fname,de->d_name);
                    atcip(fname);
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

    rlog("none",temppath,0);

    char fullpath[BUFSIZ];
    int res;
    clear(fullpath);
    strcpy(fullpath, polapath(tempp));

	res = mkdir(fullpath, mode);
	if (res == -1)
		return -errno;

	return 0;

}
...
```
Berikut adalah 2 fungsi yang digunakan untuk mkdir (membuat folder) yaitu xmp_readdir, xmp_getattr dan xmp_mkdir: 
- Sesuai dengan namanya fungsi xmp_mkdir digunakan untuk membuat folder atau direktori baru dengan cara mengassign `path` ke suatu variable kemudian di gabungkan (strcat) dengan folder yang menampung direktori penuh yang dideclare pada global (home/prk/Downloads) agar kedua folder dapat terhubung. Terakhir menggunakan `res` untuk `mkdir` atau membuat folder dengan parameter `fullpath` dan `mode`.
- Fungsi kedua adalah getattr yang berfungsi untuk mendapatkan attribut `path` dimana kita berada dan fungsi ini cukup sederhana yaitu mengassign path ke suatu variable dan memanggil fungsi polapath() yaitu fungsi yang menghandle apabila file tersebut harus dienkripsi ataupun tidak dan akhirnya menggunakan `res` untuk mengirim dengan syntax `res = lstat(fpath, stbuf);`
- Fungsi ketiga adalah xmp_readdir dimana fungsi ini menghandle pemisahan antara folder dengan file. Seperti yang dilihat bahwa terdapat `tempchiper` yang merupakan variable penyimpan apakah fungsi ini harus dienrkipsi, variable ini diletakkan dalam scope global. Apabila readdir mendapatkan bahwa file harus dienkripsi, fungsi ini akan melewati kondisi apabila fungsi tersebut merupakan folder ataupun file. Apabila folder maka akan di enkripsi dan dikirim dengan `res = (filler(buf, temp, &st, 0));`. Apabila file maka akan dibedakan lagi menjadi 2 yaitu dengan extensi ataupun tidak. Apabila dengan ekstensi, pada akhir akan digabung lagi dengan extensinya yang disimpan dalam variable menggunakan syntax `strcat`, setelah itu dienkripsi baru dikirim dengan `res = (filler(buf, temp, &st, 0));`. Apabila tanpa ekstensi, maka akan langsung dienkripsi dan dikirim.
