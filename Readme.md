# Pomme 

POMME is a scientific main field model representing the geomagnetic field in the region from the Earth's surface to an altitude of a couple of thousand kilometers. 
The time variations of the internal field are given by a piece-wise linear representation of the spherical harmonic (Gauss) coefficients of the magnetic potential.

The latest version of Pomme is POMME-10. Please see the [POMME-10](https://geomag.colorado.edu/index.php/pomme-10-magnetic-model-of-the-earth) for the detail.

## Build requirements
This package requires a C compiler to build the Pomme C code (e.g. gcc), and that the Python setuptools, wheel and build packages are installed. Running tests requires pytest.

## Compile and install

Ensure you have the `build` installed.
```commandline
pip install build
```
Build Python Package
```commandline
python -m build
```
```commandline
pip install .
```

## Using the C Extension in Python
After building the extension, you can import and use the function.

```python
from pomme import cpomme

result = cpomme.compute(2024.5,20,30,0,50.5,25.5,40,69.99,0.45)
print(result)
```
It will return the Python dictionary.
```python
{'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729}
```

## Using the Python interface
The function `get_field` provides a way to run Pomme for multiple locations, times and/or geomagnetic activity index values. 

It accepts either floats or lists of floats for each input, returning a list of dictionaries (as returned by the C interface above). A mixture of float inputs and list inputs is allowed as long as all lists are the same length.

Running the same inputs as above, except with the time value duplicated:
```python
from pomme import pypomme

results = pypomme.get_field([2024.5,2024.5],20,30,0,50.5,25.5,40,69.99,0.45)
print(results)
```

A list of dictionaries is returned:
```python
[{'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729},{'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729}]
```

