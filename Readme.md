# Pomme Python API

POMME is a scientific main field model representing the geomagnetic field in the region from the Earth's surface to an altitude of a couple of thousand kilometers. 
The time variations of the internal field are given by a piece-wise linear representation of the spherical harmonic (Gauss) coefficients of the magnetic potential.

The latest version of Pomme is POMME-10. Please see the [POMME-10](https://geomag.colorado.edu/index.php/pomme-10-magnetic-model-of-the-earth) for the detail. Pomme Python API wraps the C code of POMME-10 in python 3.11 to 3.14.

## Installation

### Install from wheel

Please select the wheel based on your python version and platform from the [GitHub Releases](https://github.com/CIRES-Geomagnetism/Pomme/releases). It supports Mac OS and x86_64 architecture for linux and Windows OS.

#### For Windows users

Select the wheels which have the `win_amd64` tag in the name. It is compatible with `Windows 10` and later versions.

Then, install the wheel by
```commandline
pip install pomme-0.1.11-cp313-cp313-win_amd64.whl
```
#### For Linux users

Select the wheels which have the `manylinux` tag in the name. It is compatible with older version of Linux distributions which `glibc >= 2.28` such as `Ubuntu 20.04`, `CentOS 8` and `RHEL 8`.

#### For macOS users

Select the wheels which have the `macosx` tag in the name.


### Build from source

#### Prerequisites

- Python 3.11 to 3.14
- Python build, setuptools and wheel packages
- C compiler (e.g. gcc)

#### Build Python Package
```commandline
python -m build
pip install .
```

## Using the Python interface

The function `get_field` computes the external field contribution for one or more locations, times, and geomagnetic activity index values.

It accepts either `float` or `list[float]` for each input argument. A mixture of scalar and list inputs is allowed as long as all lists have the same length — scalars are broadcast to match.

### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `decimal_year` | float | Date as a decimal year (e.g. `2024.5` for mid-2024) |
| `lat` | float | Geodetic latitude in degrees. Valid range: **[-90, 90]** |
| `lon` | float | Geodetic longitude in degrees. Valid range: **[-180, 360]** |
| `alt_km` | float | Altitude above the WGS-84 ellipsoid in kilometres. Valid range: **[-1, 1000]** |
| `est` | float | Est index — amplitude of the external (magnetospheric ring-current) field, in nT |
| `ist` | float | Ist index — amplitude of the internally-induced ring-current field, in nT |
| `imf_by` | float | Interplanetary Magnetic Field By component, in nT |
| `f107` | float | F10.7 solar radio flux index (proxy for solar EUV activity) |
| `em` | float | Merging electric field Em, in mV/m |

Running the same inputs as above, except with the time value duplicated:
```python
from pomme import pypomme

results = pypomme.get_field([2024.5, 2024.5], 20.0, 30.0, 0.0, 50.5, 25.5, 40.0, 69.99, 0.45)
print(results)
```

A list of dictionaries is returned:
```python
[{'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729},
 {'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729}]
```

## Source codes

### Python Wrapper

- `pomme/pypomme.py`: the main python wrapper for the C codes.
- `pomme/cpomme/pomme_wrapper.c`: The C wrapper for the original C codes. It converts input parameters from Python to C and returns the output results as a Python dict.
- `pomme/cpomme/model.c`: The interface for receiving the input parameters and running the Pomme model.
- `pomme/cpomme/src`: The C source files for the Pomme model.

### pomme_calc

`pomme_calc` is a standalone C executable that exposes the POMME model via command-line flags.

To build it, go to the `pomme_calc` folder and run:
```commandline
make all
```


#### Usage

```commandline
./build/pomme_calc -d <decimal_year> -a <lat> -o <lon> -e <alt_km> \
                   -E <Est> -I <Ist> -B <IMF_By> -R <F107> -m <Em> \
                   [-T X|C]
```
> **Note:** Before the XML/CSV result, the program prints one debug line `x_t y_t z_t` containing the total-field (internal + external) northward, eastward, and downward components in nT.

#### Output fields

##### XML / CSV tags

| Tag | Description |
|-----|-------------|
| `XCOMPONENT` | Northward component of the external field (nT) |
| `YCOMPONENT` | Eastward component of the external field (nT) |
| `ZCOMPONENT` | Downward component of the external field (nT) |

For the following components, `pomme_calc` outputs the **external contribution** — computed as the difference between the total field (internal + external) and the internal-only field:

| Tag | Description |
|-----|-------------|
| `TOTALINTENSITY` | External contribution to total field intensity (nT) |
| `HORINTENSITY` | External contribution to horizontal field intensity (nT) |
| `DECLINATION` | External contribution to declination (degrees) |
| `INCLINATION` | External contribution to inclination (degrees) |

For example, declination is computed as:
```c
results->magc[0][0][0].declination = decl_t - decl_m;
```

where `decl_t` is the declination of the **total** (internal + external) field and `decl_m` is the declination when all **external** field contributions are excluded (internal field only).

## Contact

If you have any questions or issues, please submit an issue on the Github repository or send am email to `geomag.models@noaa.gov`