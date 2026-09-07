# Information as Bits

A Qt 6/C++20 desktop application for the Module 1 converter and pixel-system
assignment. The program is organized into three independent features:

1. **ASCII to Decimal** converts strict 7-bit ASCII text to decimal codes.
2. **Number Base Converter** converts unsigned 64-bit values among binary,
   octal, decimal, and hexadecimal.
3. **Pixel Image Codec** imports an image or pixel-text file, previews the
   result, and exports lossless 8-bit RGBA pixel values.

The assignment's boundary tests are in the separate
`tests/UnitTests.cpp` source file.

## Build and run

Requirements:

- CMake 3.21 or newer
- A C++20 compiler
- Qt 6.2 or newer with the Gui and Widgets components

From the project directory:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

If CMake cannot locate Qt, pass the Qt installation directory explicitly:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/macos
```

The backend and tests can also be built on a machine without Qt:

```sh
cmake -S . -B build -DIAB_BUILD_GUI=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

## Base-converter behavior

Unsigned mode follows the useful behavior of `hexc`: input and output bases are
selected independently, whitespace separates multiple values, lowercase input
is accepted, uppercase hexadecimal is produced, and values range from zero
through `18446744073709551615` (`UINT64_MAX`). Prefixes such as `0x` are not
accepted because the selected input base already supplies that information.

Two's-complement mode is an explicit extension for the assignment. Select an
8-, 16-, 32-, or 64-bit word size. Negative input uses decimal notation; input
in another base is interpreted as a bit pattern of the selected width. Output
in binary, octal, or hexadecimal is padded to that width. This makes values
such as `11111111` unambiguous: it is 255 in unsigned mode and -1 in signed
8-bit mode.

## Pixel-text format

Pixel values use a small versioned, row-major text format. Every pixel is
`#RRGGBBAA`, which preserves transparency and matches web color ordering.

```text
IAB-IMAGE 1
WIDTH 2
HEIGHT 2
FORMAT RGBA8
PIXELS
#ED6A5AFF #808F85FF
#595959FF #EBF8B8FF
```

`Import Image` creates this text from an image. `Import Text` validates the
headers, dimensions, format, pixel count, and hexadecimal values before it
creates a preview image. `Export Text` saves the canonical uppercase form.
The page accepts images up to 1,000,000 pixels. Source images are normalized to
8-bit RGBA; color profiles, high-bit-depth channels, animation, and metadata are
not stored in the text format.

## Source organization

- `AsciiConverter`, `BaseConverter`, and `PixelCodec` contain testable logic
  with no GUI dependencies.
- `AsciiWidget`, `BaseConverterWidget`, and `PixelWidget` contain the three Qt
  pages.
- `HomePage` and `MainWindow` provide navigation.
- `style/style.qss` applies the assignment palette consistently.
- `tests/UnitTests.cpp` tests normal input, validation failures, zero,
  `UINT64_MAX`, and negative two's-complement values.

## Reference

The independent base-conversion implementation was informed by the interface
and standard positional-conversion approach demonstrated by
[sdarre/hexc](https://github.com/sdarre/hexc) (GPL-3.0). No source code from
`hexc.c` is copied into this project.

## Collaboration

OpenAI Codex assisted with the refactor, and automated test design.
