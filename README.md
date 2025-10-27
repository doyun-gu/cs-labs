# University of Manchester Concurrent Systems Lab

## C++ Set up Requirement
### Windows

As this modules uses `#include<thread>` general c++ compiler does not understand this library. This method is how I used and compiled all the following code.

#### 1. Install MSYS2
Copy and paste the following code in the powershell.
```sh
winget install MSYS2.MSYS2
```

#### 2. Run `MSYS2 UCRT64` Terminal
> [!NOTE]
> This is not your terminal/powershell!

#### 3. Check update & toolchain at `MSYS2` Terminal

```sh
pacman -Syu        
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain 
```

> [!NOTE]
> You may need to reopen your terminal!

#### 4. Check installation

```sh
g++ --version
```

#### 5. Compile using thread

```sh
g++ -std=c++17 -pthread main.cpp -o main.exe
```