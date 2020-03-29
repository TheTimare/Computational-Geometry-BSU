import math
import random
import time
from matplotlib import pyplot as plt
from celluloid import Camera
from Point import Point

# В выпуклом мн-ке простой мн-к, между ними двигаются точки

fig = plt.figure()
camera = Camera(fig)


def init_points_for_big_polygon():
    points = []
    xs = [2, 4, 9, 12, 14, 15, 12, 6, 1, 2]
    ys = [3, 1, 1, 3, 7, 12, 14, 13, 7, 3]
    for i in range(len(xs)):
        x = Point(xs[i], ys[i])
        points.append(x)
    return points


def init_points_for_small_polygon():
    points = []
    xs = [7, 9, 12, 11, 11, 7, 8, 6, 7]
    ys = [4, 7, 7, 9, 11, 10, 8, 6, 4]
    for i in range(len(xs)):
        x = Point(xs[i], ys[i])
        points.append(x)
    return points


def draw_polygon(points: list, color):
    for i in range(len(points) - 1):
        plt.plot([points[i].x, points[i + 1].x], [points[i].y, points[i + 1].y], color=color)


def draw_point(point: Point):
    plt.scatter(point.x, point.y)


def det(a, b, c, d):
    return a * d - b * c


def get_point_position_to_line(p0: Point, p1: Point, p2: Point) -> str:
    d = det(p2.x - p1.x, p2.y - p1.y, p0.x - p1.x, p0.y - p1.y)
    if d > 0:
        return "left"
    elif d < 0:
        return "right"
    else:
        return "on the line"


# Секторный тест для выпуклого многоугольника
def get_point_position_to_convex_polygon(p0: Point, points: list) -> str:
    if get_point_position_to_line(p0, points[0], points[1]) == "right" or \
            get_point_position_to_line(p0, points[0], points[len(points) - 1]) == "left":
        return "out"

    start = 0
    end = len(points) - 1

    while end - start > 1:
        sep = math.floor((start + end) / 2)
        if get_point_position_to_line(p0, points[0], points[sep]) == "left":
            start = sep
        else:
            end = sep

    if get_point_position_to_line(p0, points[start], points[start + 1]) == 'left':
        return 'in'
    else:
        return 'out'


def get_octane(p1: Point, p2: Point):
    x = p2.x - p1.x
    y = p2.y - p1.y

    if 0 <= y <= x:
        return 1
    elif 0 < x <= y:
        return 2
    elif 0 <= -x < y:
        return 3
    elif 0 < y <= -x:
        return 4
    elif 0 <= -y < -x:
        return 5
    elif 0 < -x <= -y:
        return 6
    elif 0 < x < -y:
        return 7
    elif 0 < -y <= -x:
        return 8
    else:
        return 1


# Угловой тест
def get_point_position_to_simple_polygon(p0: Point, points: list) -> str:
    s = 0
    for i in range(len(points) - 1):
        beta1 = get_octane(p0, points[i])
        beta2 = get_octane(p0, points[i + 1])
        delta = beta2 - beta1
        if delta > 4:
            delta -= 8
        if delta < -4:
            delta += 8
        if delta == 4 or delta == -4:
            d = det(points[i].x - p0.x, points[i].y - p0.y, points[i + 1].x - p0.x, points[i + 1].y - p0.y)
            if d > 0:
                delta = 4
            if d < 0:
                delta = -4
            if d == 0:
                return "edge"
        s += delta
    if s == 8 or s == -8:
        return "in"
    elif s == 0:
        return "out"
    else:
        ArithmeticError("s is not right")


def init_moving_points():
    points = []
    xs = [4, 6, 10, 12, 8, 6, 4]
    ys = [8, 4, 4, 12, 12, 8, 4]
    for i in range(len(xs)):
        x = Point(xs[i], ys[i])
        points.append(x)
    return points


def init_vectors_of_moving(points: list):
    vectors = []
    xs = [random.randint(-1, 1) for _ in range(len(points))]
    ys = [random.randint(-1, 1) for _ in range(len(points))]
    for i in range(len(xs)):
        p = Point(xs[i], ys[i])
        while p.x == 0 and p.y == 0:
            p = Point(random.randint(-1, 1), random.randint(-1, 1))
        vectors.append(p)
    return vectors


def draw_moving_points(points: list):
    for i in range(len(points)):
        draw_point(points[i])


def get_reflected_vector(a: Point, p1: Point, p2: Point):
    b = Point(p2.x - p1.x, p2.y - p1.y)
    result = b
    product = ((a * b) / (b * b)) * 2
    result.x *= product
    result.y *= product
    return result - a


def are_intersected(p1: Point, p2: Point, p3: Point, p4: Point) -> bool:
    d1 = det(p4.x - p3.x, p4.y - p3.y, p1.x - p3.x, p1.y - p3.y)
    d2 = det(p4.x - p3.x, p4.y - p3.y, p2.x - p3.x, p2.y - p3.y)
    d3 = det(p2.x - p1.x, p2.y - p1.y, p3.x - p1.x, p3.y - p1.y)
    d4 = det(p2.x - p1.x, p2.y - p1.y, p4.x - p1.x, p4.y - p1.y)

    if d1 * d2 <= 0 and d3 * d4 <= 0:
        return True
    else:
        return False


def get_intersected_edge(p1: Point, p2: Point, polygon_points: list):
    for i in range(len(polygon_points) - 1):
        if are_intersected(p1, p2, polygon_points[i], polygon_points[i + 1]):
            return [polygon_points[i], polygon_points[i + 1]]
    return []


def move(moving_points: list, vectors: list, i):
    moving_points[i] = moving_points[i] + vectors[i]


def has_trapped(p0: Point, v0: Point, moving_points: list, vectors: list):
    moving_points.remove(p0)
    vectors.remove(v0)


def init_motion(moving_points: list, external_polygon: list, internal_polygon: list):
    vectors = init_vectors_of_moving(moving_points)
    while len(moving_points) != 0:
        points_for_big_polygon = init_points_for_big_polygon()
        points_for_small_polygon = init_points_for_small_polygon()
        draw_polygon(points_for_big_polygon, "red")
        draw_polygon(points_for_small_polygon, "green")
        draw_moving_points(moving_points)
        camera.snap()
        for i in range(len(moving_points)):
            if i >= len(moving_points):
                continue
            print("POINT ", moving_points[i].x, moving_points[i].y)
            print ("I: ", i)
            next_point = Point(moving_points[i].x + vectors[i].x, moving_points[i].y + vectors[i].y)
            print ("NEXT POINT ", next_point.x, next_point.y)
            print ("EXTERNAL: ", get_point_position_to_convex_polygon(next_point, external_polygon))
            # time.sleep(0.2)
            while get_point_position_to_convex_polygon(next_point, external_polygon) == "out":
                edges = get_intersected_edge(moving_points[i], next_point, external_polygon)
                if len(edges) == 0:
                    print ("NO EDGES")
                    move(moving_points, vectors, i)
                    continue
                edge_p1 = edges[0]
                edge_p2 = edges[1]

                vectors[i] = get_reflected_vector(vectors[i], edge_p1, edge_p2)
                next_point = Point(moving_points[i].x + vectors[i].x, moving_points[i].y + vectors[i].y)

            if get_point_position_to_simple_polygon(next_point, internal_polygon) == "in":
                has_trapped(moving_points[i], vectors[i], moving_points, vectors)
                continue
            move(moving_points, vectors, i)


def init():
    moving_points = init_moving_points()
    points_for_big_polygon = init_points_for_big_polygon()
    points_for_small_polygon = init_points_for_small_polygon()
    init_motion(moving_points, points_for_big_polygon, points_for_small_polygon)

    plt.grid(True)
    animation = camera.animate(blit=False, interval=300)
    plt.show()


init()
