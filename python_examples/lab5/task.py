import math
import random
import time
from matplotlib import pyplot as plt
from celluloid import Camera
from Point import Point
from Vector import Vector

# Jarvis algorithm + finding the diameter of a flat set + points motion

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


def opposite_vectors_of_moving(vectors: list):
    for i in range(len(vectors)):
        vectors[i] = Point(-vectors[i].x, -vectors[i].y)
    return vectors


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
    return init_point


def get_next_active_point(points: list, current_active_point: Point, convex_hull_points: list):
    if len(convex_hull_points) < 2:
        polar_axis = Vector(Point(0, 0), Point(1, 0))
    else:
        polar_axis = Vector(convex_hull_points[-2], convex_hull_points[-1])
    next_active_point = get_point_with_min_arc(points, current_active_point, polar_axis, convex_hull_points)
    return next_active_point


def cos(v1: Vector, v2: Vector):
    cos_value = (v1 * v2) / (v1.get_length() * v2.get_length())
    if cos_value > 1:
        return 1
    elif cos_value < -1:
        return -1
    else:
        return cos_value


def get_point_with_min_arc(points: list, active_point: Point, polar_axis: Vector, convex_hull_points: list):
    min_arc = 2 * math.pi
    index = -1
    init_point = get_init_point(points)
    for i in range(len(points)):
        if polar_axis.get_length() == 0:
            continue
        if len(convex_hull_points) == 1 and points[i].x == init_point.x and points[i].y == init_point.y:
            continue
        is_on_hull = False
        for j in range(1, len(convex_hull_points)):
            if points[i].x == convex_hull_points[j].x and points[i].y == convex_hull_points[j].y:
                is_on_hull = True
                break
        if is_on_hull:
            continue
        current_arc = math.acos(cos(Vector(active_point, points[i]), polar_axis))
        if min_arc > current_arc >= 0:
            min_arc = current_arc
            index = i
    return points[index]


# Jarvis algorithm
def build_convex_hull(points: list):
    init_point = get_init_point(points)
    active_point = init_point
    convex_hull_points = [active_point]

    while True:
        active_point = get_next_active_point(points, active_point, convex_hull_points)
        convex_hull_points.append(active_point)
        if init_point.x == convex_hull_points[-1].x and init_point.y == convex_hull_points[-1].y:
            break

    return convex_hull_points


def draw_convex_hull(convex_hull_points: list, color: str):
    for i in range(len(convex_hull_points) - 1):
        plt.plot([convex_hull_points[i].x, convex_hull_points[i + 1].x],
                 [convex_hull_points[i].y, convex_hull_points[i + 1].y], color=color)
    camera.snap()


def point_distance(p1: Point, p2: Point):
    return math.sqrt(abs((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)))


def triangle_area(p1: Point, p2: Point, p3: Point):
    v1 = Vector(p2, p1)
    v2 = Vector(p3, p2)
    v3 = Vector(p3, p1)

    semi_perimeter = (v1.get_length() + v2.get_length() + v3.get_length()) / 2

    return math.sqrt(abs(semi_perimeter * (semi_perimeter - v1.get_length()) * (semi_perimeter - v2.get_length()) * \
                         semi_perimeter - v3.get_length()))


def set_diameter(points: list):
    p_index = -1
    q_index = 0
    n = len(points) - 1
    while triangle_area(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) > \
            triangle_area(points[p_index % n], points[(p_index + 1) % n], points[q_index % n]):
        q_index += 1

    q0_index = q_index
    d = 0
    i = 0
    while points[q_index % n] != points[0] and i < n:
        p_index += 1
        while triangle_area(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) > \
                triangle_area(points[p_index % n], points[(p_index + 1) % n], points[q_index % n]):
            q_index += 1
            if point_distance(points[p_index % n], points[q_index % n]) != point_distance(points[q0_index % n], points[0]):
                if point_distance(points[p_index % n], points[q_index % n]) >= d:
                    d = point_distance(points[p_index % n], points[q_index % n])
        if triangle_area(points[p_index % n], points[(p_index + 1) % n], points[(q_index + 1) % n]) == \
                triangle_area(points[p_index % n], points[(p_index + 1) % n], points[q_index % n]):
            if point_distance(points[p_index % n], points[q_index % n]) != point_distance(points[q0_index % n], points[-1]):
                if point_distance(points[p_index % n], points[q_index % n]) >= d:
                    d = point_distance(points[p_index % n], points[q_index % n])
    return d


def diameter_points(points: list):
    for i in range(len(points)):
        for j in range(len(points)):
            if point_distance(points[i], points[j]) == set_diameter(points):
                return [points[i], points[j]]
    print("not found")


def draw_diameter(points: list):
    diameter_points_list = diameter_points(points)
    plt.plot([diameter_points_list[0].x, diameter_points_list[1].x], [diameter_points_list[0].y, diameter_points_list[1].y], "red")


def move(moving_points: list, vectors: list):
    for i in range(len(moving_points)):
        moving_points[i] = moving_points[i] + vectors[i]


def init_motion(points: list):
    vectors = init_vectors_of_moving(points)
    diameter_limit = 40

    i = 0
    while i < 70:
        print(i)
        convex_hull_points = build_convex_hull(points)

        draw_points(points)
        draw_diameter(convex_hull_points)
        draw_convex_hull(convex_hull_points, "blue")
        camera.snap()

        if set_diameter(convex_hull_points) >= diameter_limit:
            vectors = opposite_vectors_of_moving(vectors)

        move(points, vectors)
        i += 1


def draw_point(point: Point):
    plt.scatter(point.x, point.y)


def draw_points(points: list):
    for i in range(len(points)):
        draw_point(points[i])


def init():
    points = init_points()
    init_motion(points)
    # draw_points(points)
    # convex_hull_points = build_convex_hull(points)
    # draw_convex_hull(convex_hull_points, "blue")
    # check(convex_hull_points)

    plt.grid(True)
    animation = camera.animate(blit=False, interval=300)
    plt.show()


init()
