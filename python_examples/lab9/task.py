import random
import math
from matplotlib import pyplot as plt
from celluloid import Camera
from Point import Point

# Algorithm for finding the closest pair (divide and conquer) + movement of circles and their collision

fig = plt.figure()
camera = Camera(fig)
ax = fig.gca


def init_points():
    points = []
    x = random.randint(0, 8)
    y = random.randint(0, 8)
    p = Point(x, y)
    points.append(p)
    x = random.randint(13, 21)
    y = random.randint(13, 21)
    p = Point(x, y)
    points.append(p)
    x = random.randint(26, 34)
    y = random.randint(26, 34)
    p = Point(x, y)
    points.append(p)
    x = random.randint(39, 47)
    y = random.randint(39, 47)
    p = Point(x, y)
    points.append(p)
    x = random.randint(52, 60)
    y = random.randint(52, 60)
    p = Point(x, y)
    points.append(p)
    x = random.randint(65, 72)
    y = random.randint(65, 72)
    p = Point(x, y)
    points.append(p)
    x = random.randint(77, 85)
    y = random.randint(77, 85)
    p = Point(x, y)
    points.append(p)
    x = random.randint(90, 98)
    y = random.randint(90, 98)
    p = Point(x, y)
    points.append(p)
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


def opposite_vectors_of_moving(vector: Point):
    vector = Point(-vector.x, -vector.y)
    return vector


def det(a, b, c, d):
    return a * d - b * c


def sort_by_x(points: list):
    sorted_points = []
    sorted_points.extend(points[0: len(points)])
    for i in range(len(sorted_points)):
        min_idx = i
        for j in range(i + 1, len(sorted_points)):
            if sorted_points[min_idx].x >= sorted_points[j].x:
                min_idx = j
        sorted_points[i], sorted_points[min_idx] = sorted_points[min_idx], sorted_points[i]
    return sorted_points


def sort_by_y(points: list):
    sorted_points = []
    sorted_points.extend(points[0: len(points)])
    for i in range(len(sorted_points)):
        min_idx = i
        for j in range(i + 1, len(sorted_points)):
            if sorted_points[min_idx].y >= sorted_points[j].y:
                min_idx = j
        sorted_points[i], sorted_points[min_idx] = sorted_points[min_idx], sorted_points[i]
    return sorted_points


def get_middle_point_by_x(points_sorted_by_x: list):
    return points_sorted_by_x[math.floor(len(points_sorted_by_x) / 2)]


def point_distance(p1: Point, p2: Point):
    return math.sqrt(abs((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)))


def min_set_distance(points: list):
    min_distance = point_distance(points[0], points[1])
    for i in range(len(points) - 1):
        for j in range(i + 1, len(points)):
            if point_distance(points[i], points[j]) <= min_distance:
                min_distance = point_distance(points[i], points[j])
    return min_distance


# divide and conquer
def closest_pair(points_sorted_by_x: list, points_sorted_by_y: list):
    if 1 < len(points_sorted_by_x) <= 3:
        min_distance = min_set_distance(points_sorted_by_x)
        return min_distance

    x_sep_point = get_middle_point_by_x(points_sorted_by_x)
    sep_index = points_sorted_by_x.index(x_sep_point)
    x_left_set = points_sorted_by_x[0: sep_index]
    x_right_set = points_sorted_by_x[sep_index: len(points_sorted_by_x)]

    y_left_set = []
    y_right_set = []
    for i in range(len(points_sorted_by_y)):
        if points_sorted_by_y[i].x < x_sep_point.x:
            y_left_set.append(points_sorted_by_y[i])
        else:
            y_right_set.append(points_sorted_by_y[i])

    min_distance_left = closest_pair(x_left_set, y_left_set)
    min_distance_right = closest_pair(x_right_set, y_right_set)
    min_distance = min(min_distance_left, min_distance_right)

    delta_set = []
    for i in range(len(points_sorted_by_y)):
        if abs(points_sorted_by_y[i].x - x_sep_point.x) <= min_distance:
            delta_set.append(points_sorted_by_y[i])

    mythical_number = 7
    n = len(delta_set) if len(delta_set) <= mythical_number else mythical_number
    for i in range(n - 1):
        for j in range(i + 1, n):
            if point_distance(delta_set[i], delta_set[j]) < min_distance:
                min_distance = point_distance(delta_set[i], delta_set[j])

    return min_distance


def draw_circle(point: Point, radius: int):
    circle = plt.Circle((point.x, point.y), radius, color="blue")
    plt.gcf().gca().add_artist(circle)


def draw_circles(points: list, radius: int):
    for i in range(len(points)):
        draw_circle(points[i], radius)


def get_closest_pair_indexes(points: list, closest_pair_diameter):
    for i in range(len(points) - 1):
        for j in range(i + 1, len(points)):
            if point_distance(points[i], points[j]) == closest_pair_diameter:
                return (i, j)
    print("not found")


def draw_min_distance(points: list, closest_pair_diameter):
    closest_pair_indexes = get_closest_pair_indexes(points, closest_pair_diameter)
    plt.plot([points[closest_pair_indexes[0]].x, points[closest_pair_indexes[1]].x],
             [points[closest_pair_indexes[0]].y, points[closest_pair_indexes[1]].y], "red")


def move(moving_points: list, vectors: list):
    for i in range(len(moving_points)):
        moving_points[i] = moving_points[i] + vectors[i]


def init_motion(points: list):
    radius = 4
    vectors = init_vectors_of_moving(points)
    i = 0
    while i < 70:
        print(i)
        draw_circles(points, radius)

        points_sorted_by_x = sort_by_x(points)
        points_sorted_by_y = sort_by_y(points)

        closest_pair_diameter = closest_pair(points_sorted_by_x, points_sorted_by_y)
        closest_pair_indexes = get_closest_pair_indexes(points, closest_pair_diameter)
        draw_min_distance(points, closest_pair_diameter)
        camera.snap()

        if closest_pair_diameter <= 2 * radius:
            vectors[closest_pair_indexes[0]] = opposite_vectors_of_moving(vectors[closest_pair_indexes[0]])
            vectors[closest_pair_indexes[1]] = opposite_vectors_of_moving(vectors[closest_pair_indexes[1]])
        move(points, vectors)

        i += 1


def init():
    points = init_points()
    init_motion(points)
    plt.grid(True)
    plt.gca().set_xlim((0, 100))
    plt.gca().set_ylim((0, 100))
    animation = camera.animate(blit=False, interval=300)
    plt.show()


init()
