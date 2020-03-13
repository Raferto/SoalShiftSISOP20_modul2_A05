# SoalShiftSISOP20_modul2_A05
**Intan Kusuma R  (05111840000020)**

**Rafid Ferdianto (05111840000032)**

## Soal 1
source code: [soal1.c](https://github.com/Raferto/SoalShiftSISOP20_modul2_A05/blob/master/soal1/soal1.c)

### Penjelasan soal
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:

a. Program menerima 4 argumen berupa:

	i. Detik: 0-59 atau * (any value)
 	ii. Menit: 0-59 atau * (any value)
 	iii. Jam: 0-23 atau * (any value)  	
	iv. Path file.sh
  
b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai

c. Program hanya menerima 1 config cron

d. Program berjalan di background (daemon)

e. Tidak boleh menggunakan fungsi system()

Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.
### Penyelesaian
* menggunakan template daemon yang sudah ada di modul2
* akan ada 5 argumen, yaitu nama program, detik, menit, jam, dan path script bash yang akan dijalankan
* jika jumlah argumen bukan 5, maka akan mengeluarkan "Input salah" dan keluar dari program
```c++
if(argc!=5)
{
	printf("Input salah\n");
	exit(EXIT_FAILURE);
}
```
* dilakukan pengecekan pada detik, menit, dan jam. Jika input argumen berupa `*`, buat sebuah array dan isi dengan -1
```c++
if(strncmp(argv[i],"*",1)==0)
{
	arr[i]=-1;
}
```
* jika argumen bukan `*`, masukkan argumen ke sebuah array kemudian cek apakah hanya terdiri dari angka
* jika hanya terdiri dari angka, dilakukan konversi dari string ke int dengan `atoi` dan masukkan hasilnya ke `arr[i]`
```c++
char str[sizeof(argv[i])+1];
strcpy(str,argv[i]);
for(int k=0; k<strlen(str);k++)
{
	if(str[k]>='0' && str[k]<='9')
	f++;
}
if(f==strlen(str))
{
	arr[i]=atoi(argv[i]);
}
```
* jika bukan angka atau `*` maka output "Input salah" dan keluar dari program
* kemudian cek apakah input angka benar
```c++
if(arr[1]<-1 || arr[1]>59 || arr[2]<-1 || arr[2]>59 || arr[3]<-1 || arr[3]>23)
{
	printf("Input salah\n");
	exit(EXIT_FAILURE);
}
```
* mengambil waktu
```c++
time_t waktu;
struct tm *jam;        
waktu=time(NULL);
jam=localtime(&waktu);
```
* cek apakah waktu sekarang sesuai dengan input, jika sesuai maka bash script akan dijalankan
```c++
if ((jam->tm_sec == arr[1] || arr[1] == -1) && (jam->tm_min == arr[2] || arr[2] == -1) && (jam->tm_hour == arr[3] || arr[3] == -1))
```
* dilakukan `fork()`, child akan menjalankan bash
```c++
if(pid==0)
{
	char *a[]={"bash", argv[4], NULL};
	execv("/bin/bash", a);
}
```
## Soal 2
### Penjelasan soal dan penyelesaian
a. Pertama-tama, dibuat sebuah folder khusus, di dalamnya dibuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].

b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].

c. Setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).

d. Program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.

e. Program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

## Soal 3
### Penjelasan soal dan penyelesaian
a. Program buatan jaya harus bisa membuat dua direktori di "/home/[USER]/modul2/". Direktori yang pertama diberi nama "indomie", lalu lima detik kemudian membuat direktori yang kedua bernama "sedaap".
* dilakukan `fork()`. Child process akan menjalankan 3a.
```c++
while ((wait(&status)) > 0);
pid7= fork();
if (pid7 == 0) 
{
	char *argv[] = {"mkdir", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie", NULL};
	execv("/bin/mkdir", argv);
}
else
{
	while ((wait(&status)) > 0);
	sleep(5);
	char *argv[] = {"mkdir", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/sedaap", NULL};
	execv("/bin/mkdir", argv);
}
```
Untuk menyelesaikan soal 3a, dilakukan `fork()` pada `pid7`. Child process akan melakukan `mkdir /home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie` dan parent process akan melakukan `mkdir /home/bluejake3/SISOP/Modul2/Praktikum/soal3/sedaap`. `sleep(5)` digunakan agar direktori "sedaap" dibuat 5 detik setelah direktori "indomie" dibuat.

b. Program tersebut harus meng-ekstrak file jpg.zip di direktori "/home/[USER]/modul2/".
```c++
char *argv[] = {"unzip", "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg.zip", NULL};
execv("/usr/bin/unzip", argv);
```
* `unzip` digunakan untuk mengekstrak file "jpg.zip"

c. Setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori "home/[USER]/modul2/jpg/") harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke "/home/[USER]/modul2/sedaap/" dan semua direktori harus dipindahkan ke "/home/[USER]/modul2/indomie/".
```c++
DIR *dir=opendir("/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/");
struct dirent *entry;
if (dir)
{
	while ((entry = readdir(dir)) != NULL)
	{
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		continue;
		if(entry->d_type==DT_DIR)
		{
			pid3=fork();
			if(pid3== 0)
			{
				char folder[1000];
				sprintf(folder, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/%s", entry->d_name);
				char *argv[] = {"mv", folder, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie", NULL};
				execv("/bin/mv", argv);
			}
```
* Jika direktori 'jpg' belum kosong, dilakukan loop untuk mengecek isinya
* `(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)` digunakan agar tidak menyertakan direktori "." dan ".."
* `entry->d_type==DT_DIR` digunakan untuk mengecek apakah direktori atau bukan
* dilakukan `fork()` pada `pid3`, child process akan memindahkan direktori dari direktori "jpg" ke direktori "indomie"
* `char folder[1000]` digunakan untuk menyimpan path dari direktori yang akan dipindahkan
```c++
else
{
	while ((wait(&status)) > 0);
	pid6=fork();
	if(pid6 == 0)
	{
		while ((wait(&status)) > 0);
		char file[1000];
		sprintf(file, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/jpg/%s", entry->d_name);
		char *argv[] = {"mv", file,"/home/bluejake3/SISOP/Modul2/Praktikum/soal3/sedaap", NULL};
		execv("/bin/mv", argv);
	}
}
```
* Jika file, maka dilakukan `fork()` pada `pid6`. Child process akan memindahkan file ke direktori "sedaap"
* `char file[1000]` digunakan untuk menyimpan path dari file yang akan dipindahkan

d. Untuk setiap direktori yang dipindahkan ke "/home/[USER]/modul2/indomie/" harus membuat dua file kosong. File yang pertama diberi nama "coba1.txt", lalu 3 detik kemudian membuat file bernama "coba2.txt".
(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
```c++
pid5=fork();
if(pid5 == 0)
{
	char coba1[1000];
	FILE *cb1;
	sprintf(coba1, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie/%s/coba1.txt", entry->d_name);
	cb1 = fopen(coba1, "w");
	fclose(cb1);
}
else
{
	while ((wait(&status)) > 0);
	sleep(3);
	char coba2[1000];
	FILE *cb2;
	sprintf(coba2, "/home/bluejake3/SISOP/Modul2/Praktikum/soal3/indomie/%s/coba2.txt", entry->d_name);
	cb2 = fopen(coba2, "w");
	fclose(cb2);
	exit(0);
}
```
* dilakukan `fork()` pada `pid5`, child process akan membuat file "coba1.txt" dan parent akan membuat file "coba2.txt"
* `char coba1[1000]` dan `char coba2[1000]` digunakan untuk menyimpan path dari file "coba1.txt" dan "coba2.txt"
* `cb1 = fopen(coba1, "w")` dan `cb2 = fopen(coba2, "w")` digunakan untuk membuat file "coba1.txt" dan "coba2.txt" kosong
* `sleep(3)` digunakan agar file "coba2.txt" dibuat 3 detik setelah file "coba1.txt" dibuat









