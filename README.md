## Daisy Set up
clone with submodules
```
git clone --recursive https://github.com/jordaneast1/daisy_vcv_template
```

or

```sh
git clone https://github.com/jordaneast1/daisy_vcv_template
git submodule update --init
```

shell for this on Windows is Git Bash

$ cd into daisy 

```sh
cd daisy 
#first build the project
make
#then flash the hardware
# using USB (after entering bootloader mode)
make program-dfu
```

## Updating the submodules

To pull everything for the repo and submodules:

```sh
git pull --recurse-submodules
```

rebuild the libs
```
make -C libDaisy
make -C DaisySP
```

## VCV set-up
Install VCV Rack 2
Follow the VCV doucmentation setup for Windows carefully.
https://vcvrack.com/manual/Building
https://vcvrack.com/manual/PluginDevelopmentTutorial

Using MinGW 64-bit shell from Msys2
Ensure the VCV Rack-SDK is installed and set on $PATH


```sh
cd vcv
make
make install
```
