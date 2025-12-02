# Genetic Algorithm Maze Project

## 🚀 Cara Compile dan Menjalankan Program

### **1. Compile Program (Linux / macOS)**

Gunakan script `cc.sh` untuk melakukan kompilasi:

```bash
./cc.sh
```

Perintah di dalam `cc.sh`:

```bash
g++ *.cpp -o main -Wall -Wextra -Wshadow -std=c++20
```

Output: file executable **`main`**.

### **1b. Compile Program (Windows)**

Gunakan script batch `cc.bat`:

```bat
cc.bat
```

Isi perintah:

```bat
g++ *.cpp -o main.exe -Wall -Wextra -Wshadow -std=c++20
```

Output: **`main.exe`**.

### **2. Menjalankan Program****

Setelah proses kompilasi berhasil, jalankan program dengan perintah:

```bash
./main
```

---

## 📁 Struktur Direktori Proyek

```
cc.sh
main
main.cpp
Maze.cpp
Maze.h
Util.cpp
Util.h
```

Penjelasan masing-masing file:

### **`main.cpp`**

File ini merupakan titik utama eksekusi program. Berisi alur lengkap algoritma genetika, termasuk:

* Inisialisasi parameter
* Pembentukan population awal
* Loop generasi
* Evaluasi fitness
* Proses seleksi, crossover, mutasi
* Menentukan hasil terbaik

### **`Maze.cpp` & `Maze.h`**

Berisi definisi objek **Maze (labirin)** dan seluruh property pentingnya, seperti:

* Representasi struktur maze
* Fungsi pembangkitan maze secara random
* Perhitungan *fitness function* terhadap maze
* Fungsi utilitas terkait maze

### **`Util.cpp` & `Util.h`**

Mengandung fungsi-fungsi pendukung untuk algoritma genetika, antara lain:

* Generate population
* Seleksi individu
* Crossover antar maze
* Mutasi
* Elitism
* Penyimpanan ringkasan data hasil eksperimen

---

## 📊 Output dan Eksperimen

Kode ini mendukung berbagai konfigurasi eksperimen algoritma genetika, antara lain:

### **1. Crossover**

* **Simple Arithmetic Crossover**
* **Uniform Biased Crossover**

### **2. Selection**

* **Roulette Wheel Selection**
* **Tournament Selection**

### **3. Elitism**

* Nilai default: **2 individu terbaik** disalin ke generasi berikutnya
* Set ke **0** untuk menjalankan GA tanpa elitism

### **4. Fitness Function**

* **DFS-based fitness** → Meniru proses *random attempt* menjelajahi maze
* **BFS-based fitness** → Menghitung berbagai faktor seperti:

  * Jarak terpendek
  * Jumlah jalur yang memungkinkan
  * Banyak jalan buntu (*dead ends*)
  * Banyak percabangan (*branching*)
    (placeholder — tambahkan cara membaca output, format log, dan interpretasi hasil)

---

## 📌 Catatan

Pastikan sistem memiliki compiler **g++** yang mendukung standar **C++20**.  
Gunakan opsi kompilasi tambahan pada `cc.sh` bila diperlukan untuk debugging atau profiling.

---

## 🧬 Deskripsi Ekstra 

Tugas akhir Mata Kuliah **Genetika Algoritma**, Semester Genap 2025.  
Dosen Pembimbing: Aina Musdholifah, S.Kom., M.Kom., Ph.D.  
Link Paper: https://github.com/david-irvinne/maze-generator-using-genetic-algorithm  

**Anggota Kelompok:**

1. David Neilleen Irvinne (23/517639/PA/22199)
2. Bagus Cipta Pratama (23/516539/PA/22097)
3. Syafran Abdillah Erdin (23/521752/PA/22444)
4. Muhammad Dhiwaul Akbar (23/523237/PA/22513)

---
