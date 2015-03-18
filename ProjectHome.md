## About ##

TinyImageLoader is free software written in C++ for loading images.

## Features ##

Other image loading libraries support many more formats. But why support things that are hardly used? TinyImageLoader comes with support for the most widely used formats and can be compiled to support only one.

| Format | DevIL 1.7.8 | FreeImage 3.14.1 | TinyImageLoader 1.7.0 |
|:-------|:------------|:-----------------|:----------------------|
| .bmp | ✓ | ✓ | ✓ |
| .cut | ✓ | ✓ | ✗ |
| .dds | ✓ | ✓ | ✓ |
| .exr | ✓ | ✓ | ✗ |
| .hdr | ✓ | ✓ | ✗¹ |
| .ico | ✓ | ✓ | ✓ |
| .iff | ✓ | ✓ | ✗ |
| .iwi | ✓ | ✓ | ✗ |
| .gif | ✓ | ✓ | ✓ |
| .jpg | ✓ | ✓ | ✗ |
| .png | ✓ | ✓ | ✓ |
| .psd | ✓ | ✓ | ✗ |
| .psp | ✓ | ✗ | ✗ |
| .tga | ✓ | ✓ | ✓ |

¹: planned future support.

## Footprint ##

Footprints matter on mobile devices. When you have a limited amount of space, why waste it on an image loading library? TinyImageLoader has a much smaller featureset than other loaders, and thus a smaller footprint.

| DevIL 1.7.8 | 2.40 MB |
|:------------|:--------|
| FreeImage 3.14.1 | 2.47 MB |
| TinyImageLoader 1.7.0 | 247 kB |