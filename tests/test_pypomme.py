import pytest
from pomme import pypomme


def test_determine_expected_input_list_length():
    A = [1.0, 2.0, 3.0]
    B = [4.0, 5.0, 6.0]
    C = 7.0
    expected_length = 3
    test_length = pypomme._determine_expected_input_list_length(A, B, C)
    assert test_length == expected_length


def test_exceptional_determine_expected_input_list_length():
    A = [1.0, 2.0, 3.0]
    B = [4.0, 5.0]
    C = 6.0
    # Should fail because A and B different lengths
    with pytest.raises(RuntimeError):
        pypomme._determine_expected_input_list_length(A, B, C)


def test_broadcast_if_scalar_on_scalar():
    value = 1.0
    expected_length = 3
    expected_list = [1.0, 1.0, 1.0]
    test_list = pypomme._broadcast_if_scalar(value, expected_length)
    assert test_list == expected_list


@pytest.mark.parametrize("non_float_value", [1, "1", None])
def test_expectional_broadcast_if_scalar_on_scalar(non_float_value):
    with pytest.raises(TypeError):
        pypomme._broadcast_if_scalar(non_float_value, 3)


def test_broadcast_if_scalar_on_list():
    # should short-circuit and just return list
    value = [1.0, 1.0]
    expected_length = 3.0  # ignores this if list input
    expected_list = [1.0, 1.0]
    test_list = pypomme._broadcast_if_scalar(value, expected_length)
    assert test_list == expected_list


@pytest.mark.parametrize("list_with_non_float", [[1, 1.0], ["1", 1.0], [None, 1.0]])
def test_expectional_broadcast_if_scalar_on_list(list_with_non_float):
    with pytest.raises(TypeError):
        pypomme._broadcast_if_scalar(list_with_non_float, 3)


def test_in_range():
    values = [2.0, 3.0, 4.0]
    assert pypomme._in_range(values, 2.0, 4.0)


def test_execptional_in_range():
    values = [2.0, 3.0, 4.0]
    assert not pypomme._in_range(values, 3.0, 4.0)
