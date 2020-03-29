import math
import random
import time
from matplotlib import pyplot as plt
from celluloid import Camera
from Point import Point
from Vector import Vector

# Graham's algorithm + animation

fig = plt.figure()
camera = Camera(fig)


def init_points():
    points = []
    xs = [random.randint(0, 10) for _ in range(10)]
    ys = [random.randint(0, 10) for _ in range(10)]
    for i in range(len(xs)):
        x = Point(xs[i], ys[i])
        points.append(x)
    return points


def get_min_y(points: list):
    min_y = points[0].y
    for i in range(len(points)):
        if points[i].y < min_y:
            min_y = points[i].y
    return min_y


def get_init_point(points: list):
    min_y = get_min_y(points)
    init_point = Point(-1, -1)
    for i in range(len(points)):
        if points[i].y == min_y:
            init_point = points[i]
    points.remove(init_point)
    return init_point


def cos(v1: Vector, v2: Vector):
    return (v1 * v2) / (v1.get_length() * v2.get_length())


def get_coses(points: list, init_point: Point):
    coses = []

    # next_point = Point(init_point.x + 1, init_point.y)
    # i_vector = Vector(init_point, next_point)
    i_vector = Vector(Point(0, 0), Point(1, 0))

    for i in range(len(points)):
        v = Vector(init_point, points[i])
        print("v length: ", v.get_length(), " v.x: ", v.x, " v.y: ", v.y)
        cos_value = math.acos(cos(v, i_vector))
        coses.append(cos_value)

    sort_arcs(points, coses, init_point)
    return coses


# selections sort
def sort_arcs(points: list, coses: list, init_point: Point):
    if not len(coses) == len(points):
        raise ValueError("coses and points length are not equal")
    for i in range(len(coses)):
        min_idx = i
        for j in range(i + 1, len(coses)):
            if coses[min_idx] > coses[j]:
                min_idx = j
            elif coses[min_idx] == coses[j]:
                min_idx_vector = Vector(init_point, points[min_idx])
                j_vector = Vector(init_point, points[j])
                if j_vector.get_length() > min_idx_vector.get_length():
                    min_idx = j

        coses[i], coses[min_idx] = coses[min_idx], coses[i]
        points[i], points[min_idx] = points[min_idx], points[i]


def det(a, b, c, d):
    return a * d - b * c


def get_point_position_to_line(p0: Point, p1: Point, p2: Point):
    d = det(p2.x - p1.x, p2.y - p1.y, p0.x - p1.x, p0.y - p1.y)
    if d > 0:
        return "left"
    elif d < 0:
        return "right"
    else:
        return "on the line"


def build_convex_hull(points: list, init_point: Point):
    print("init point: ", init_point.x, ":", init_point.y)
    coses = get_coses(points, init_point)
    for i in range(len(coses)):
        print("cos i ", i, " ", coses[i])
    for i in range(len(points)):
        print("point i ", i, " ", points[i].x, " : ", points[i].y)

    convex_hull_points = [init_point, points[0]]

    for i in range(1, len(points)):
        if points[i] == init_point:
            continue
        while len(convex_hull_points) >= 2 and \
                get_point_position_to_line(points[i], convex_hull_points[-2], convex_hull_points[-1]) == "right":
            del convex_hull_points[-1]
            draw_points(points, init_point)
            draw_convex_hull(convex_hull_points, "green")

        convex_hull_points.append(points[i])
        draw_points(points, init_point)
        draw_convex_hull(convex_hull_points, "green")

    convex_hull_points.append(init_point)
    draw_points(points, init_point)
    draw_convex_hull(convex_hull_points, "green")


def draw_convex_hull(convex_hull_points: list, color: str):
    for i in range(len(convex_hull_points) - 1):
        plt.plot([convex_hull_points[i].x, convex_hull_points[i + 1].x],
                 [convex_hull_points[i].y, convex_hull_points[i + 1].y], color=color)
    camera.snap()


def draw_point(point: Point):
    plt.scatter(point.x, point.y)


def draw_points(points: list, init_point: Point):
    draw_point(init_point)
    for i in range(len(points)):
        draw_point(points[i])


def init():
    points = init_points()
    init_point = get_init_point(points)
    draw_points(points, init_point)
    build_convex_hull(points, init_point)

    plt.grid(True)
    animation = camera.animate(blit=False, interval=300)
    plt.show()


init()
