# C++ Compilation Deep Dive

This project is a technical exploration of the **four distinct stages** of the C++ compilation process. Instead of treating the compiler as a "black box," this repository breaks down how a simple `Hello World` program is transformed from human-readable code into a machine-executable binary.

---

## 🛠 The Workflow

Standard C++ compilation (using `g++`) follows a four-stage relay. Below are the commands to manually stop the process at each stage to inspect the intermediate files.

### 1. Pre-processing (`.i` file)

The pre-processor handles directives (lines starting with `#`). It expands macros and copies the contents of header files into the source.

```bash
g++ -E main.cpp -o main.i

```

### 2. Compilation (`.s` file)

The compiler translates the pre-processed code into **Assembly language**, which is specific to the target processor architecture.

```bash
g++ -S main.i -o main.s

```

### 3. Assembly (`.o` file)

The assembler converts assembly code into **Object code** (binary machine code). These files are not yet executable as they lack library links.

```bash
g++ -c main.s -o main.o

```

### 4. Linking (Executable)

The linker combines object files and resolves references to external libraries (like `iostream`) to create the final executable.

```bash
g++ main.o -o main

```

---


## 🚀 How to Run

If you just want to compile and run the final product in one go:

```bash
g++ -o main main.cpp && ./main

```

---

## 📝 Key Takeaways

* **Pre-processor:** Expand `#include` and `#define`.
* **Compiler:** Logic → Assembly.
* **Assembler:** Assembly → Machine Code (Binary).
* **Linker:** Bridges code with system libraries.

---

Would you like me to add a section explaining how to use a **Makefile** to automate generating all these intermediate files at once?