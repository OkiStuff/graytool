# graytool
`graytool` is a simple command-line tool to convert colored images to grayscale

## Installation
`graytool` has compiled executables in the Releases tab, or you can easily compile the project using CMake. There are no dependencies that require seperate compilation or installation.

## Getting Started
To use `graytool`, simply pass a filename as a parameter to the `graytool` executable. For example: `graytool dog.png`. use `graytool -h` or `graytool --help` to see all possible options.

## Grayscale methods

### Average
Average method simple takes the average of the red, green, and blue channels. This is the simplest method but doesn't account for the human perception of each color channel. Enabled with `--average`

### Luminance (Rec.601)
This is the most commonly used, perceptually accurate method. It weights each color channel according to human sensitivity (`0.299R + 0.587G + 0.114B`). This method is the default method. Enabled with `--luminance601`

### Luminance (Rec.709)
Adapted version of Luminance (Rec.601) for HD video. Similarly, it weights each color channel according to human sensitivity (`0.2126R + 0.7152G + 0.0722B`). Enabled with `--luminance709`

### Luminance (Custom)
Custom weights per color channel for artistic control. Enabled with `--luminance (Red channel weight),(Green channel weight),(Blue channel weight)`

### Desaturation
Mixes the pixel halfway between its brightest and darkest color channel. Enabled with `--desaturation`

### Value
Takes the maximum color channel. Enabled with `--value`
