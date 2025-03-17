from pomme import cpomme
from typing import Union,List

def _determine_expected_input_list_length(*input_args):
    """Check that all list-type members of an arbitrary set of inputs
    have the same length as each other, then return that length"""
    input_lens = [len(arg) for arg in input_args if isinstance(arg,list)]
    if not all([l==input_lens[0] for l in input_lens]):
        raise RuntimeError('All list inputs must have the same length')
    else:
        return input_lens[0]

def _broadcast_if_scalar(arg : float, expected_len : int):
    """Check if arg is a scalar type, if it is, duplicate it into a list
    of length expected_len"""
    if isinstance(arg,list):    
        return arg
    elif isinstance(arg, float):
        return [arg for i in range(expected_len)]
    else:
        raise TypeError(f'Expected float or list not {type(arg)}')

def _in_range(values : list, 
                min_value : float, 
                max_value : float, 
                min_exclusive = False, 
                max_exclusive = False):
    """Determine if all values in a list are in the inverval [min_value,max_value], if an
    open inverval is desired, set min_exclusive to True for (min_value,max_value], 
    or max_exclusive to True for [min_value,max_value)"""

    def in_interval(value : float):
        left_condition = value > min_value if min_exclusive else value >= min_value
        right_condition = value < max_value if max_exclusive else value <= max_value
        return left_condition and right_condition

    return all([in_interval(value) for value in values])

def get_field(decimal_year : Union[float,list], 
                lat : Union[float,list],
                lon : Union[float,list],
                alt_km : Union[float,list], 
                est : Union[float,list],
                ist : Union[float,list],
                imf_by : Union[float,list],
                f107 : Union[float,list],
                em : Union[float,list]) -> List[dict]:
    """Compute the Pomme model field for any number of input points. Single
    value (scalar) inputs are duplicated in a list to match length of list inputs"""

    num_points = _determine_expected_input_list_length(decimal_year,
                                                        lat,
                                                        lon,
                                                        alt_km,
                                                        est,
                                                        ist,
                                                        imf_by,
                                                        f107,
                                                        em)

    decimal_year = _broadcast_if_scalar(decimal_year, num_points)
    lat = _broadcast_if_scalar(lat, num_points)
    lon = _broadcast_if_scalar(lon, num_points)
    alt_km = _broadcast_if_scalar(alt_km, num_points)
    est = _broadcast_if_scalar(est, num_points)
    ist = _broadcast_if_scalar(ist, num_points)
    imf_by = _broadcast_if_scalar(imf_by, num_points)
    f107 = _broadcast_if_scalar(f107, num_points)
    em = _broadcast_if_scalar(em, num_points)

    if not _in_range(lat,-90.,90.):
        raise ValueError('Latitude not in [-90.,90.]')

    if not _in_range(lon,-180.,360.):
        raise ValueError('Longitude not in [-180.,360.]')

    if not _in_range(alt_km,-1.,1000.):
        raise ValueError('Altitude in kilometers not in [-1.,1000.]')

    results = []
    for i in range(num_points):
        result = cpomme.compute(decimal_year[i],
                                 lat[i],
                                 lon[i],
                                 alt_km[i],
                                 est[i],
                                 ist[i],
                                 imf_by[i],
                                 f107[i],
                                 em[i])

        result['decimal_year']=decimal_year[i]
        result['lat']=lat[i]
        result['lon']=lon[i]
        result['alt_km']=alt_km[i]

        results.append(result)

    return results




    
    
    
    
