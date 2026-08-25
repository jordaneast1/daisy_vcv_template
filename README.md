## Daisy Set up
#clone with submodules
git clone --recursive https://github.com/electro-smith/DaisyExamples
```

or

```sh
git clone https://github.com/electro-smith/DaisyExamples
git submodule update --init
```

best shell for this on Windows is Git Bash

$ cd into /daisy 

```sh 
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

#rebuild the libs
make -C libDaisy
make -C DaisySP


## VCV set-up
Install VCV Rack 2
Follow the VCV doucmentation setup for Windows carefully.
https://vcvrack.com/manual/Building
https://vcvrack.com/manual/PluginDevelopmentTutorial

Using MinGW 64-bit shell from Msys2
Ensure the VCV Rack-SDK is installed and set on $PATH


```sh
cd /vcv
make
make install
```








### Create new example project

Creates a brand new example project containing a Makefile, compilable source file,
and debug resources for VisualStudio using VisualGDB, and for VS Code using Cortex Debug.

The board option can be any of the following:

field, patch, petal, pod, seed, versio

`./helper.py create pod/MyNewProject --board seed`

# daisy_vcv_template
