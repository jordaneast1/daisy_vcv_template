An experimental project to see if VCV Rack can be used to prototype Daisy hardware modules with DaisySP

Claude used to generate boilerplate.

## Project layout

```
shared/     SharedDSP.{h,cpp}  -- the voice. All DSP, no front panel.
daisy/      template.cpp       -- Daisy hardware front panel (ADC knob, button, LED)
vcv/        src/DaisyModule.cpp -- VCV front panel (knob, button, light, output)
```

`shared/` is compiled into both builds, so a change to the DSP lands on the
hardware and in Rack at once. Prototype in VCV, then flash. Anything that reads
a knob or drives an LED belongs in the host file, not in `shared/`.

Both hosts call the same two functions:

```cpp
voice.Init(sample_rate);                 // and again if the rate changes
float sig = voice.Process(knob, gate);   // knob 0..1, gate true while held
```

Note that `make -C DaisySP` builds DaisySP for the Daisy's Cortex-M7. The VCV
plugin can't link those objects, so `vcv/Makefile` builds its own x86-64 copy of
DaisySP into `vcv/build/`. You don't need to do anything for that -- `cd vcv && make`
handles it -- but it does mean DaisySP gets compiled twice, once per target.

## Set up
clone with submodules
```
git clone --recursive https://github.com/jordaneast1/daisy_vcv_template
```

or

```sh
git clone https://github.com/jordaneast1/daisy_vcv_template
git submodule update --init
```

## Set up and build Daisy
For all Daisy commands use Git Bash.
Ensure submodules are installed.

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

For all VCV commands use MinGW 64-bit shell from Msys2
Ensure the VCV Rack-SDK is installed and set on $PATH


```sh
cd vcv
make
make install
```
