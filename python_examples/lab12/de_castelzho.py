from data_structures import Point
from typing import List
import matplotlib.pyplot as plt
from celluloid import Camera

colors = {
    1: 'm',
    2: "k",
    3: "c",
}

def define_curve(points: List[Point], camera) -> List[Point]:
    step = 0.01
    t = 0
    curve = []
    while t <= 1:
        point = de_castelzho(points, t, camera)
        curve.append(point)
        t += step
    print([str(f"{point.x}, {point.y}") for point in curve])
    return curve


def de_castelzho(points: List[Point], t, camera):
    if len(points) == 1:
        plt.plot(points[0].x, points[0].y, 'om')
        camera.snap()
        return points[0]
    else:
        new_points = []
        for (i, point) in enumerate(points[:-1]):
            x = (1 - t) * point.x + t * points[i + 1].x
            y = (1 - t) * point.y + t * points[i + 1].y
            new_points.append(Point(x, y))
        draw_lines(new_points)
        return de_castelzho(new_points, t, camera)


def draw_lines(points: List[Point]):
    xs = [point.x for point in points]
    ys = [point.y for point in points]
    color = colors[len(points)]
    plt.plot(xs, ys, color)