# SSL RoboIME Firmware

This repository contains the firmware for the SSL (Small Size League) robots, built using the Zephyr RTOS.

## Setup Instructions

Follow these steps to set up the development environment on your PC:

### Prerequisites

- Linux (Ubuntu 24.04 LTS recommended) 
- Python 3.8 or newer
- Git

### Installation Steps

1. Install system dependencies:

```bash
sudo apt update
sudo apt install --no-install-recommends git cmake ninja-build gperf \
ccache dfu-util device-tree-compiler wget \
python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
make gcc gcc-multilib g++-multilib libsdl2-dev
sudo apt install python3-venv
```

2. Install Toolchain:
Create a target directory for the toolchain:
```bash
mkdir -p $HOME/.local/opt
```
Download and unpack the toolchain:
```bash
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz -O - | xz -d -c - | tar xv -C $HOME/.local/opt
```
Run the Zephyr SDK bundle setup script:
```bash
$HOME/.local/opt/zephyr-sdk-0.16.8/setup.sh
```
3. Setup Local:
Create `/roboime folder`:
```bash
mkdir roboime && cd roboime
```
Create Virtual environment:
```bash
python3 -m venv .venv
```
Activate Virtual environment:
```bash
source .venv/bin/activate
```
When you close the shell (or your text editor with the integrated terminal), you must reactivate the virtual Python environment. 

Update pip
```bash
pip install --upgrade pip
```

Install West
```bash
pip install west
sudo apt install tmux

```

4. Clone the project repository:
```bash
mkdir ssl && cd ssl
west init -m https://github.com/roboime/ssl_firmware.git
west config build.board nucleo_h723zg
west update
west zephyr-export
```

5. Install additional Python dependencies:
```bash
pip install -r zephyr/scripts/requirements.txt
```

### Build test
Go to the blinky sample directory:
```bash
cd roboime/ssl_firmware/applications/nexus
```
Build:
```bash
west build
```
Flash:

```bash
west flash
```
Open RoboIME console:

```bash
west console
```
## VSCode configs

Install *nRF Device Tree* Extension

Add this config

![image](https://github.com/user-attachments/assets/a0f7fa28-79cd-4ec5-ba93-87f4c89079b0)



