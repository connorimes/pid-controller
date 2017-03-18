# Proportional Integral Derivative (PID) Controller

A simple closed-loop PID feedback controller.


## Building

This project uses CMake.

To build, run:

``` sh
mkdir _build
cd _build
cmake ..
make
```


## Installing

To install, run with proper privileges:

``` sh
make install
```

On Linux, installation typically places libraries in `/usr/local/lib` and header files in `/usr/local/include`.


## Uninstalling

Install must be run before uninstalling in order to have a manifest.
To uninstall, run with proper privileges:

``` sh
make uninstall
```


## Linking

Get linker information with `pkg-config`:

``` sh
pkg-config --libs pid-controller
```

Or in your Makefile, add to your linker flags with:

``` Makefile
$(shell pkg-config --libs pid-controller)
```

Depending on your install location, you may also need to augment your compiler flags with:

``` sh
pkg-config --cflags pid-controller
```


## Usage

See the [inc/pid-controller.h](inc/pid-controller.h) and [inc/pid-controller-util.h](inc/pid-controller-util.h) header files for function descriptions.
