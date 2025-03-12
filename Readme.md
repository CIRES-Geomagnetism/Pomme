# Pomme 

POMME is a scientific main field model representing the geomagnetic field in the region from the Earth's surface to an altitude of a couple of thousand kilometers. 
The time variations of the internal field are given by a piece-wise linear representation of the spherical harmonic (Gauss) coefficients of the magnetic potential.

The latest version of DIFI is POMME-10. Please see the [POMME-10](https://geomag.colorado.edu/index.php/pomme-10-magnetic-model-of-the-earth) for the detail.

## Compile the POMME C Extension

Make sure you have setuptools installed in your Python virtual environment

```commandline
python setup.py build_ext --inplace
```
```commandline
python setup.py install
```

## Using the C Extension in Python
After building the extension, you can import and use the function.

```python
import pomme

map = pomme.compute(2024.5,20,30,0,50.5,25.5,40,69.99,0.45)
print(map)
```
It will return the Python dict object as outputs.
```python
{'Bx': 51.070342325328376, 'By': -19.434705977679094, 'Bz': 5.4209154034177285, 'Bh': 49.5246766288401, 'Bf': 46.928721256037534, 'Bdec': -0.037527083681935025, 'Binc': -0.0244606748919729}
```

