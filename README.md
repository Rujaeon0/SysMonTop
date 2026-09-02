# SysMonTop


<p align="center">
  <img src="icons/SysMonTop.svg" alt="SysMonTop logo" width="120">
</p>

<h1 align="center">SysMonTop</h1>


A lightweight GUI system monitor app built with C and GTK4 for linux.

---

## Features

- CPU Usage
- RAM Usage
- Swap Usage
- System Usage
- Process Usage
- Process Memory Sorting
- Auto Refresh


---

## Requirements

- GTK4
- Meson
- Ninja
- GCC or Clang

---

## Installation

### Step 1) Clone into the repository and cd

```bash
git clone https://github.com/Rujaeon0/SysMonTop.git
cd SysMonTop
```

### Build

```bash
meson setup build
meson compile -C build
```

### Run

```bash
./build/SysMonTop
```



### Future Updates pending...

- Process Termination
- Network package rates
- Graphs for usage
- Temparature/sensors
- Better style and design

Made this project for myself to finish up my basics for C and do open source contribution...my codes may not be as good yet but if you installed this...thanks~

## License

MIT License