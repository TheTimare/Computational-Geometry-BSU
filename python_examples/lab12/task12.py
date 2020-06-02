import numpy as np
from typing import List
from data_structures import Point


def construct_curve(points: List[Point], step: float):
    curve = []
    t = 0
    while t <= 1:
        curve.append(define_curve_point(points, t))
        t += step
    print([str(f"{point.x}, {point.y}") for point in curve])
    return curve


def define_curve_point(points: List[Point], t: float) -> Point:
    x = 0
    y = 0
    n = len(points) - 1
    for (i, point) in enumerate(points):
        x += point.x * polynom_berstein(n, i, t)
        y += point.y * polynom_berstein(n, i, t)
    return Point(x, y)


def polynom_berstein(n: int, k: int, t: float) -> float:
    coef = binomial_coef(n, k)
    return coef * (t ** k) * ((1 - t) ** (n - k))


def binomial_coef(n: int, k: int) -> float:
    return np.math.factorial(n) / (np.math.factorial(k) * np.math.factorial(n - k))
