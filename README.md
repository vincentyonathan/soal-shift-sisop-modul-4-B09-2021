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
*Praktikan* diminta untuk membuat filesystem dimana bisa dilakukan mkdir(pembuatan folder) dimana jika itu dilakukan dan namanya diawali dengan "AtoZ_", maka nama folder beserta seluruh isinya akan terenkripsi dengan metode Atbash Cipher.
#### Source Code & Penjelasan
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
}
    
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
```
Fungsi utama dari soal ini adalah 2 fungsi diatas yaitu polapath() dan atcip()
- Fungsi atcip() diatas merupakan kode yang digunakan untuk melakukan Atbash Cipher yaitu dengan melakukan *mirror* kepada masing - masing `char`. Cara melakukannya adalah looping dari setiap `char` dari kalimat kemudian apabila `char` tersebut ada dalam range A sampai Z atau a sampai z, maka akan char itu akan dikurangkan dengan ('Z'+'A') atau ('z' + 'a')
- Fungsi polapath() berguna sebagai fungsi yang memfilter apakah path tersebut memiliki string "/AtoZ_" didalamnya. Apabila ada, maka akan dicek dengan opendir apakah berupa folder, apabila bukan maka akan dianggap file. File pun terbagi jadi 2 yaitu dengan extension atau tidak. Apabila dengan extension, maka akan ada variable yang mengambil extentiom tersebut yang nantinya akan digunakan pada xmp_readdir. Terakhir adalah menggunakan `sprintf` untuk menghasilkan path yang sudah benar dan terenkripsi.

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

#### 1.b)
*Praktikan* diminta untuk membuat filesystem dimana bisa dilakukan rename(perubahan nama folder) dimana jika itu dilakukan dan namanya diawali dengan "AtoZ_", maka nama folder beserta seluruh isinya akan terenkripsi dengan metode Atbash Cipher.
##### Source Code & Penjelasan
```c
...
static int xmp_rename(const char *lama, const char *baru)
{
    char tempsebelum[BUFSIZ];
    char tempsetelah[BUFSIZ];
    clear(tempsebelum);
    clear(tempsetelah);
    sprintf(tempsebelum, "%s%s", dirp, lama);
    sprintf(tempsetelah, "%s%s", dirp, baru);

    rlog(tempsebelum,tempsetelah,1);

	int res;

	res = rename(tempsebelum, tempsetelah);
	if (res == -1)
		return -errno;

	return 0;
}
...
```
- Fungsi - fungsi sebelumnya seperti xmp_getattr(), xmp_readdir(), polapath() serta atcip() tetap digunakan.
- xmp_rename ini digunakan ketika *user* ingin melakukan rename (mengganti nama folder). Caranya cukup sederhana yaitu menyimpan nama file sebelum dan sesudah ke dalam variable.
- Setelah itu melakukan `sprintf`, pada kedua variable dengan tujuan mendapatkan path full dari keduanya.
- Setelah itu hanya tinggal menggunakan `res` untuk memanggil rename dengan parameter `tempsebelum` yang diganti dengan `tempsetelah`.
- Maka program akan berjalan dan mengganti nama folder yang dituju kemudian mengecek untuk melakukan enkripsi Atbash Cipher.

#### 1.c)
*Praktikan* diminta untuk membuat filesystem dimana bisa dilakukan rename(perubahan nama folder) dimana apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
#### Source Code & Penjelasan
```c
...
static int xmp_rename(const char *lama, const char *baru)
{
    char tempsebelum[BUFSIZ];
    char tempsetelah[BUFSIZ];
    clear(tempsebelum);
    clear(tempsetelah);
    sprintf(tempsebelum, "%s%s", dirp, lama);
    sprintf(tempsetelah, "%s%s", dirp, baru);

    rlog(tempsebelum,tempsetelah,1);

	int res;

	res = rename(tempsebelum, tempsetelah);
	if (res == -1)
		return -errno;

	return 0;
}
...
```
- Pada fungsi rename seperti pada 3.b), itu juga akan berjalan ketika folder direname menjadi nama yang tidak ter-encode atau terenkripsi, fungsinya telah mampu menghandle kejadian tersebut. Hal ini terjadi karena pada dasarnya Atbash Cipher merupakan pencerminan daripada string. Maka cukup menggunakan algoritma yang telah dijelaskan pada 3.b) pada soal ini. Contoh "AtoZ_APA KABAR" di enkripsi menggunakan Atbash Cipher menjadi "ZKZ PZYZI", begitu pula "ZKZ PZYZI" di atbash menjadi "APA KABAR".
 
#### 1.d)
*Praktikan* diminta untuk membuat filesystem dimana bisa setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/USER/Downloads/Nama Direktori → /home/USER/Downloads/AtoZ_Nama Direktori

#### Source Code & Penjelasan
```c
...

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
...
```
- Pada soal ini, diminta membuat log untuk setiap mkdir(pembuatan folder) dan rename(penggantian nama folder). Caranya adalah dengan menambahkan fungsi rlog() dimana tujuan dari fungsi ini adalah membuka file `running.log` ( membuatnya ketika belum ada ) pada path dimana dia akan diletakkan
- Setelah itu ketika pada source code fungsi xmp_mkdir dan xmp_rename diatas, dapat dilihat bahwa ditambahkan rlog() dengan parameter nama filenya serta modenya. Disini, ditentukan bahwa mode `0` berarti mkdir dan mode `1` berarti rename, sehingga nantinya dapat dihandle oleh rlog ketika diketahui modenya. Apabila modenya `0` maka akan dilakukan `fprintf(logge,"[mkdir]: %s\n", dir_baru);`, apabila mode `1` maka akan menjadi `fprintf(logge,"[rename]: %s -> %s\n", dir_lama,dir_baru);`.
- Terakhir adalah menutup file dengan `fclose`.

#### 1.e)
*Praktikan* diminta untuk membuat filesystem dimana metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

#### Source Code & Penjelasan
```c
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
```
- Pada fungsi polapath() yang dijelaskan pada 1.a), fungsi ini telah sanggup melakukan enkripsinya secara rekursif karena dapat dilihat bahwa dalam fungsinya menggunakan while sebagai iterasi sampai `index < strlen(path aslinya)` sehingga akan dijalankan terus menerus sampai folder ataupun path terdalamnya. Semua file juga akan ikut diiterasi dan akan dienkripsi melalui fungsi ini sehingga tidak perlu dilakukan modifikasi lagi.
