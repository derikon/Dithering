# Dithering

Implementations of several dithering algorithms producing monochrom images.

Here are some results:

![Floyd-Steinberg](https://github.com/derikon/Dithering/blob/master/example/results/Floyd-Steinberg.png)
![Ordered](https://github.com/derikon/Dithering/blob/master/example/results/Ordered.png)


## Dependencies

[OpenCV](https://opencv.org/) **version 4.0** is the only dependency. If you are using macOS you can easily install OpenCV via [Homebrew](https://brew.sh).

```
brew install opencv
```


## Build

1. clone this repository with `git clone https://github.com/derikon/Dithering.git` and move into the directory with `cd Dithering`
2. create a directory where to build the binaries with `mkdir build` and move into the directory `cd build`
3. configure CMake with `cmake ../`
4. build binaries with `make`
5. **optional:** run `make install` to install header files and the library into standard directories (`/usr/local/` and `/usr/local/lib`)


## Run Example

You can run the `dither_example` executable from `build/example/` which uses all implemented algorithms to dither a given image.

The executable takes some arguments:

+ `-i` or `--image` to set the image path
+ `-t`or `--threshold` to set the greyscale threshold (0 < t < 255)
  + if you do not set this argument the default value of 100 is used
+ `-n` or `--noise` to set the noise threshold (0 < t < 255)
  + if you do not set this argument the default value of 120 is used
+ use `-h` or `--help` to list all available arguments


## Development

This project creates a shared library which you can link to your executables (see [example/CMakeLists.txt](https://github.com/derikon/Dithering/blob/master/example/CMakeLists.txt)).

The `Dither` class represents an interface for implementations like `MonochromDither`. To implement colored dithering simply inherit from `Dither` and override all virtual functions.
