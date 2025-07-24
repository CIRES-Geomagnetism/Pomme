import pprint
from pomme.pypomme import get_field

decimal_year = 2024.5
lat = [-90.,-45.,0.,45.,90.]
lon = [-180.,-90.,0.,180.,360.]
alt_km = [-1.,0.,250.,500.,1000.]
est = 50.5
ist = 25.5
imf_by = 40.
f107 = 69.99
em = 0.45

results = get_field(decimal_year,lat,lon,alt_km,est,ist,imf_by,f107,em)

pprint.pp(results)
