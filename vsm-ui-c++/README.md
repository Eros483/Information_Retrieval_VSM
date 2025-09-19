# Setup External Dependencies

To set up the required external dependencies, run the following commands:

```bash
mkdir -p external
cd external
git clone https://github.com/Skycoder42/QHotkey.git
```
---
Build application for windows
`From root dir`
Run the following commands (make sure QHotkey is cloned in root dir of vsm):
```bash
cmake -B build/windows -S . -G "Ninja" ^
  -DCMAKE_C_COMPILER=cl.exe ^
  -DCMAKE_CXX_COMPILER=cl.exe ^
  -DCMAKE_PREFIX_PATH="C:\Qt\6.9.2\msvc2022_64"

cmake --build build/windows --config Release
```
`Here we assume you have Qt 6.9.2 installed in C:\Qt\6.9.2\msvc2022_64`
To bundle the required Qt libraries with your application, run the following command:
```bash
"C:\Qt\6.9.2\msvc2022_64\bin\windeployqt.exe" build/windows/Release/vsm.exe
```

