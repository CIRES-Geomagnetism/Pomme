import pytest
from pathlib import Path
from pomme.pypomme import get_field
import csv  # from Python stdlib

# The data files will be in the same directory as this test script
inputs_file_path = Path(__file__).parent / "pomme_inputs.csv"
outputs_file_path = Path(__file__).parent / "pomme_testvalues.txt"


def loop_test_points():
    """This function is a generator which you can loop through
    to get inputs and outputs for each test point in the files
    (each row in input and output data files).
    Inputs and outputs are returned as dictionaries
    of floats"""
    inputs_file = inputs_file_path.resolve()
    infile_fields = ["date", "lat", "lon", "alt", "est", "ist", "imf_by", "f107", "em"]

    outputs_file = outputs_file_path.resolve()
    outfile_fields = [
        "date",
        "lat",
        "lon",
        "alt",
        "x",
        "y",
        "z",
        "h",
        "f",
        "dec",
        "inc",
    ]


    # Read the inputs file
    with open(inputs_file, "r") as fin:
        in_rows = [row for row in csv.DictReader(fin, fieldnames=infile_fields)]

    # Read the outputs file
    with open(outputs_file, "r") as fout:
        out_rows = [row for row in csv.DictReader(fout, fieldnames=outfile_fields)]

    # Iterate over matching rows (so we can test each point one at a time)
    for irow, (inrow, outrow) in enumerate(zip(in_rows, out_rows)):
        if irow == 0:
            # First row is headers
            continue

        # Skip any NaN values
        for row in (inrow, outrow):

            if any(["nan" in val.lower() for key, val in row.items()]):
                print("Skipping NaN row in:{inrow} out:{outrow}")
                continue

        # Check that the input and output data in this row have the same
        # timestamp
        tol = 1.0 / 365.0 / 86400.0 * 60.0  # 1 minute tolerance
        if abs(float(inrow["date"]) - float(outrow["date"])) > tol:
            raise ValueError(f"Row {irow} fyear mismatch {inrow},{outrow}")

        # Convert everything to float type and organize into inputs and outputs dicts
        # w/ dict keys for inputs being argument names to get_field and the
        # dict keys matching the expected output dictionaries' keys
        inputs = {
            "decimal_year": float(inrow["date"]),
            "lat": float(inrow["lat"]),
            "lon": float(inrow["lon"]),
            "alt_km": float(inrow["alt"]),
            "est": float(inrow["est"]),
            "ist": float(inrow["ist"]),
            "imf_by": float(inrow["imf_by"]),
            "f107": float(inrow["f107"]),
            "em": float(inrow["em"]),
        }

        outputs = {
            "Bx": float(outrow["x"]),
            "By": float(outrow["y"]),
            "Bz": float(outrow["z"]),
            "Bh": float(outrow["h"]),
            "Bdec": float(outrow["dec"]),
            "Binc": float(outrow["inc"]),
        }

        yield inputs, outputs


@pytest.mark.parametrize(
    "inputs,expected_outputs", [(i, o) for i, o in loop_test_points()]
)
def test_get_field_scalar(inputs, expected_outputs):
    test_outputs_list = get_field(**inputs)
    # Always returns a list even for all float inputs
    test_outputs = test_outputs_list[0]
    for key in expected_outputs:
        test_val = test_outputs[key]
        expected_val = expected_outputs[key]
        # Test they match to within 5% or 1e-5 if 5% of expected_val < 1e-5
        assert test_val == pytest.approx(
            expected_val, rel=0.05, abs=1e-5
        ), f"mismatched output: {key}"


if __name__ == "__main__":

    inputs, outputs = loop_test_points()

    print("inputs: ",inputs)


    for inputs, expected_outputs in loop_test_points():
        test_outputs_list = get_field(**inputs)
        # Always returns a list even for all float inputs
        test_outputs = test_outputs_list[0]
        mismatched = []
        for key, expected_val in expected_outputs.items():
            test_val = test_outputs[key]
            if abs(test_val - expected_val) > abs(0.05 * expected_val):
                mismatched.append(key)

        if len(mismatched) > 0:
            print(f"Output(s) {mismatched} do not match")
            print(f"inputs:{inputs}")
            print(f"outputs:{test_outputs}")
            print(f"expected outputs:{expected_outputs}")
