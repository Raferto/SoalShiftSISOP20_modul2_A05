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
source code: [soal2.c](https://github.com/Raferto/SoalShiftSISOP20_modul2_A05/blob/master/soal2/soal2.c)
### Penjelasan soal dan penyelesaian
a. Pertama-tama, dibuat sebuah folder khusus, di dalamnya dibuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss]. Untuk membuat folder setiap 30 detik digunakan daemon yang melakukan fork dengan fungsi child membuat folder dengan menggunakan execv. Sedangkan parent di sleep selama 30 detik `sleep(30)` sehingga daemon dijalankan setiap 30 detik sekali. Untuk mengambil timestamp sendiri digunakan fungsi sebagai berikut, perlu diingat bahwa untuk setiap process (sampai seterusnya) dibutuhkan timestamp ini untuk mengetahui nama folder yang di process, sehingga disimpan di variabel date.
```c++
while (1){
	child_id = fork();
	if(child_id == 0){
		time(&t);
		tmp = localtime(&t); 
		strftime(date, sizeof(date), "%F_%X", tmp);

		sprintf(date,"%s/",date);
	    	char *argv[] = {"mkdir", date, NULL};
		execv("/bin/mkdir", argv);
		
	else{
		sleep(30);
	}
}
```
b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss]. Untuk mendownload 20 gambar kita perlu melakukan looping sebanyak 20 kali dan kemudian di fork, dengan fungsi child untuk mendownload sebuah gambar yang menggunakan execv dan parent yang di sleep selama 5 detik `sleep(5)` agar loop berlangsung setiap 5 detik. untuk mengambil ukuran piksel sendiri digunakan fungsi sebagai berikut, Karena nama tiap gambar tidak diperlukan maka kita tidak perlu menyimpan nama tiap gambar. Dan karena kita perlu 2.a selesai terlebih dahulu (membuat folder) maka kita menggunakan wait

```c++
while ((wait(&status)) > 0);
for(i=0;i<20;i++){
	child_id = fork();
	if(child_id == 0){
		epoch = time(NULL);
		e = (epoch%1000)+100;
		sprintf(download,"https://picsum.photos/%ld/%ld",e,e);
		time(&t);
		tmp = localtime(&t); 
		strftime(temp, sizeof(temp), "%F_%X", tmp);

		sprintf(simpan,"%s/%s",date,temp);

		char *argv[] = {"wget",download,"-O",simpan, NULL};
		execv("/usr/bin/wget", argv);
	}
	else
		sleep(5);
}
```

c. Setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip). Untuk Men-zip folder tentunya kita harus menunggu semua child selesai mendownload sehingga digunakan fungsi wait. Selanjutnya kita perlu melakukan fork lagi dengan fungsi child untuk menzip seluruh folder dengan nama date (nama folder) dan parent untuk menghapus folder dengan nama date. Perlu diingat kita perlu menunggu proses men-zip selesai baru menghapus folder. Untuk melakukannya digunakan fungsi sebagai berikut:
```c++
while ((wait(&status)) > 0);
child_id = fork();

if(child_id == 0){
	sprintf(temp,"%s.zip",date);
	char *argv[] = {"zip","-r",temp,date, NULL};
	execv("/usr/bin/zip", argv);
}

while ((wait(&status)) > 0);
char *argv[] = {"rmv","-r",date, NULL};
execv("/bin/rm", argv);
```

d. Program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri. Untuk men-generate program killer yang siap di run, kita perlu membuat file c baru yang kemudian di compile dan selanjutnya menghapus file c tersebut sehingga hanya tersisanya program yang siap di run. Untuk melakukan hal tersebut pertama-tama dilakukan fork untuk memisahkan parent sebagai process utama (2.a-2.c) dan process child untuk mengenerate program Killer. Selanjutnya child di fork lagi dengan child membuat file baru "Killer.c" dan parent yang mengcompile "Killer.c" setelah child selesai. Isi "Killer.c" pertama di fork dengan child meng execv pkill nama program utama (soal2) dan parent yang menghapus file "Killer" sendiri. Berikut isi dari "Killer.c"
:
```c++
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/types.h> 
#include<sys/stat.h> 
#include<fcntl.h> 
#include<errno.h> 
#include<wait.h> 
#include<unistd.h> 
#include<syslog.h> 
#include<string.h>

int main(){ 
	int status; 
	pid_t child_id = fork(); 
	if(child_id){ 
		while ((wait(&status)) > 0); 
		char *argv[] = { "rmv","./Killer", NULL}; 
		execv("/bin/rm", argv); 
	} 
	else{ 
		char *argv[] = { "pkill","soal2", NULL}; 
		execv("/usr/bin/pkill", argv); 
	} 
} 
```
Perlu diingat program Killer hanya perlu dibuat sekali,Maka fungsi yang digunakan berikut dijalankan diluar `while(1)`.
```c++
child_id = fork();
if(child_id == 0){
	child_id = fork();
	if(child_id==0){
		FILE *f;
		f = fopen ("Killer.c", "w");
		fprintf(f, "#include<stdio.h>\n");
		fprintf(f, "#include<stdlib.h>\n");
		fprintf(f, "#include<sys/types.h>\n");
		fprintf(f, "#include<sys/stat.h>\n");
		fprintf(f, "#include<fcntl.h>\n");
		fprintf(f, "#include<errno.h>\n");
		fprintf(f, "#include<wait.h>\n");
		fprintf(f, "#include<unistd.h>\n");
		fprintf(f, "#include<syslog.h>\n");
		fprintf(f, "#include<string.h>\n\n");
		fprintf(f, "int main(){\n");
		fprintf(f, "int status;\n");
		fprintf(f, "pid_t child_id = fork();\n");
		fprintf(f, "if(child_id){\n");
		fprintf(f, "while ((wait(&status)) > 0);\n");
		fprintf(f, "char *argv[] = { \"rmv\",\"./Killer\", NULL};\n");
		fprintf(f, "execv(\"/bin/rm\", argv);\n");
		fprintf(f, "}\n");
		fprintf(f, "else{\n");
		fprintf(f, "char *argv[] = { \"pkill\",\"soal2\", NULL};\n");
		fprintf(f, "execv(\"/usr/bin/pkill\", argv);\n");
		fprintf(f, "}\n");
		fprintf(f, "}\n");
		fclose(f);

		char *argv[] = { "gcc","./Killer.c","-o","./Killer", NULL };
		execv("/usr/bin/gcc", argv);
	}
	else{
		while ((wait(&status)) > 0);
		char *argv[] = { "rmv","./Killer.c", NULL };
		execv("/bin/rm", argv);	
	}
}
   
while(1){...	    
```

e. Program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete). Untuk melakukan ini pertama kita perlu membuat program menerima argumen lalu  sedikit memodifikasi bagian d, yaitu dengan menambahkan pengecekan -a atau -b pada argumen. apabila -a fungsi seperti bagian d dan apabila -b kita perlu merubahnya sedikit. Bagian yang dirubah adalah adalah pada bagian penulisan di program "Killer.c" dimana yang sebelumnya adalah `pkill soal2` diganti menjadi `kill` dan diikuti sid. Karena Mode_A (-a) sama dengan bagian d maka kita hanya perlu mengecek apabila input yg diterima adalah -b atau bukan. Sehingga menjadi sebagai berikut.
```c++
child_id = fork();
if(child_id == 0){
	child_id = fork();
	if(child_id==0){
		FILE *f;
		f = fopen ("Killer.c", "w");
		fprintf(f, "#include<stdio.h>\n");
		fprintf(f, "#include<stdlib.h>\n");
		fprintf(f, "#include<sys/types.h>\n");
		fprintf(f, "#include<sys/stat.h>\n");
		fprintf(f, "#include<fcntl.h>\n");
		fprintf(f, "#include<errno.h>\n");
		fprintf(f, "#include<wait.h>\n");
		fprintf(f, "#include<unistd.h>\n");
		fprintf(f, "#include<syslog.h>\n");
		fprintf(f, "#include<string.h>\n\n");
		fprintf(f, "int main(){\n");
		fprintf(f, "int status;\n");
		fprintf(f, "pid_t child_id = fork();\n");
		fprintf(f, "if(child_id){\n");
		fprintf(f, "while ((wait(&status)) > 0);\n");
		fprintf(f, "char *argv[] = { \"rmv\",\"./Killer\", NULL};\n");
		fprintf(f, "execv(\"/bin/rm\", argv);\n");
		fprintf(f, "}\n");
		fprintf(f, "else{\n");

		if(argc > 1){
			if(argv[1][1]=='b'){
				fprintf(f, "char *argv[] = { \"kill\",\"%d\", NULL};\n",sid);
				fprintf(f, "execv(\"/bin/kill\", argv);\n");
			}
			else{
				fprintf(f, "char *argv[] = { \"pkill\",\"soal2\", NULL};\n");
				fprintf(f, "execv(\"/usr/bin/pkill\", argv);\n");
			}
		}
		else{
			fprintf(f, "char *argv[] = { \"pkill\",\"soal2\", NULL};\n");
			fprintf(f, "execv(\"/usr/bin/pkill\", argv);\n");
		}
		fprintf(f, "}\n");
		fprintf(f, "}\n");
		fclose(f);

		char *argv[] = { "gcc","./Killer.c","-o","./Killer", NULL };
		execv("/usr/bin/gcc", argv);
	}
	else{
		while ((wait(&status)) > 0);
		char *argv[] = { "rmv","./Killer.c", NULL };
		execv("/bin/rm", argv);	
	}
}
while(1){...
```
Kendala selama mengerjakan soal:

- Kesulitan pada bagian d karena sangat sulit untuk mencari ide bagaimana cara mengerjakannya, sehingga bagian e juga tidak
  bisa dikerjakan

**Screenshot Hasil**
![5](https://user-images.githubusercontent.com/57932819/77176966-6740fc80-6af7-11ea-8e8e-e7c55e8a4881.png)
![6](https://user-images.githubusercontent.com/57932819/77177031-79bb3600-6af7-11ea-8144-b549ed7fae26.png)
![7](https://user-images.githubusercontent.com/57932819/77177038-7aec6300-6af7-11ea-9ec7-2064cbbc3a18.png)
![8](https://user-images.githubusercontent.com/57932819/77177042-7c1d9000-6af7-11ea-863c-3484e8e35fee.png)

## Soal 3
source code: [soal3.c](https://github.com/Raferto/SoalShiftSISOP20_modul2_A05/blob/master/soal3/soal3.c)
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









