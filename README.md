# Information as Bits

A Qt 6 and C++20 desktop application with three independent tools:

1. **ASCII to Decimal** uses the assignment's extended definition of ASCII:
   the complete Latin-1 range from 0 through 255.
2. **Number Base Converter** converts binary, octal, decimal, and hexadecimal
   values in unsigned or two's-complement form.
3. **Pixel Image Codec** imports images or pixel-text files, previews the
   result, and exports lossless RGBA8 pixel text.

All non-GUI logic is tested from the single tests/UnitTests.cpp source file.

## Build and run

Requirements:

- CMake 3.21 or newer
- A C++20 compiler
- Qt 6 with the Widgets component

From the project directory:

    cmake -S . -B cmake-build-debug
    cmake --build cmake-build-debug --parallel
    ctest --test-dir cmake-build-debug --output-on-failure

The executables are:

- cmake-build-debug/Information_As_Bits
- cmake-build-debug/UnitTests

In CLion, reload the CMake project after replacing these files, then build
either the Information_As_Bits or UnitTests target.

## Latin-1 conversion

The converter accepts Unicode characters from U+0000 through U+00FF.
Examples include ordinary ASCII, accented Latin-1 characters such as é, and
byte values 128 through 255. Characters above U+00FF are rejected with their
position in the input.

The overload taking std::string_view interprets every byte as an unsigned
Latin-1 value. The UTF-32 overload is used by the Qt interface so characters
outside Latin-1 can be detected instead of silently replaced.

## Base conversion

Unsigned values range from zero through UINT64_MAX. Whitespace separates
multiple values. Two's-complement mode supports 8-, 16-, 32-, and 64-bit word
sizes. Non-decimal signed input is treated as a bit pattern of the selected
width.

## Pixel-text format

Pixel values use uppercase #RRGGBBAA notation in a versioned, row-major text
format:

    IAB-IMAGE 1
    WIDTH 2
    HEIGHT 1
    FORMAT RGBA8
    PIXELS
    #548AF7FF #191A1CFF

Images are limited to 1,000,000 pixels and pixel-text files to 12 MB.

## Styling

style/style.qss adapts the Islands Dark interface colors for Qt. It uses
macOS-safe font families to avoid missing-font alias warnings.
