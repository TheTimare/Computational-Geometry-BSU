from matplotlib import pyplot as plt
from Point import Point


# Constructing cutoff of a polygon by segments using the Cirus-Beck algorithm


def init_points_for_polygon():
    points = []
    xs = [2, 4, 9, 12, 14, 15, 12, 6, 1]
    ys = [3, 1, 1, 3, 7, 12, 14, 13, 7]
    for i in range(len(xs)):
        x = Point(xs[i], ys[i])
        points.append(x)
    return points


def init_segments():
    return [(Point(4, 0), Point(14, 6)), (Point(12, 0), Point(16, 2)), (Point(4, 12), Point(16, 13)),
            (Point(14, 6), Point(14, 14)), (Point(1, 7), Point(4, 1)), (Point(6, -2), Point(0, 12)),
            (Point(11, 16), Point(0, 8)), (Point(12, 15), Point(3, -2)), (Point(10, 0), Point(0, 12.5))]


def print_points(points: list):
    for i in range(len(points)):
        print("i: ", i, ": ", points[i].x, ":", points[i].y)


def get_cross_point(p1: Point, p2: Point, q1: Point, q2: Point):
    if (p1.x == p2.x and p2.y == p1.y) or (q1.x == q2.x and q1.y == q2.y):
        print("Not lines")
        return

    if p1.x > p2.x:
        p1.x, p2.x = p2.x, p1.x
        p1.y, p2.y = p2.y, p1.y
    if q1.x > q2.x:
        q1.x, q2.x = q2.x, p1.x
        q1.y, q2.y = q2.y, p1.y

    k1 = 0
    k2 = 0

    if p1.x != p2.x and p2.y != p1.y:
        k1 = (p2.y - p1.y) / (p2.x - p1.x)
    if q1.x != q2.x and q2.y != q1.y:
        k2 = (q2.y - q1.y) / (q2.x - q1.x)

    if k2 == k1:
        print("k1 == k2: Lines are parallel or collinear")
        return

    b1 = p1.y - k1 * p1.x
    b2 = q1.y - k2 * q1.x

    if p1.x == p2.x:
        x = p1.x
    elif q1.x == q2.x:
        x = q1.x
    else:
        x = (b2 - b1) / (k1 - k2)

    y = k1 * x + b1

    return Point(x, y)


def get_current_parameter(segment: tuple, vertex1: Point, vertex2: Point):
    normal_vector = Point(vertex2.y - vertex1.y, -(vertex2.x - vertex1.x))
    segment_vertex_vector = Point(vertex1.x - segment[0].x, vertex1.y - segment[0].y)
    segment_vector = Point(segment[1].x - segment[0].x, segment[1].y - segment[0].y)
    return (segment_vertex_vector * normal_vector) / (segment_vector * normal_vector)


def determine_intersection_type(p1: Point, p2: Point, segment: tuple):
    normal_vector = Point(p2.y - p1.y, -(p2.x - p1.x))
    segment_vector = Point(segment[1].x - segment[0].x, segment[1].y - segment[0].y)
    if segment_vector * normal_vector > 0:
        return 1
    elif segment_vector * normal_vector < 0:
        return -1
    else:
        return 0  # no intersection


def cirus_beck(polygon_points: list, segment: tuple):
    t0_values = [0]
    t1_values = [1]
    n = len(polygon_points)

    for i in range(len(polygon_points)):
        intersection_type = determine_intersection_type(polygon_points[i], polygon_points[(i + 1) % n], segment)
        t = get_current_parameter(segment, polygon_points[i], polygon_points[(i + 1) % n])
        if intersection_type == -1:
            t0_values.append(t)
        elif intersection_type == 1:
            t1_values.append(t)
        else:
            continue

    t0 = max(t0_values)
    t1 = min(t1_values)

    print(t0, t1)

    if t0 <= t1:
        x1 = segment[0].x + (segment[1].x - segment[0].x) * t0
        x2 = segment[0].x + (segment[1].x - segment[0].x) * t1
        y1 = segment[0].y + (segment[1].y - segment[0].y) * t0
        y2 = segment[0].y + (segment[1].y - segment[0].y) * t1
        plt.plot([x1, x2], [y1, y2], color="red")


def draw_polygon(points: list, color):
    for i in range(len(points)):
        plt.plot([points[i].x, points[(i + 1) % len(points)].x], [points[i].y, points[(i + 1) % len(points)].y], color=color)


def draw_segments(segments: list, color):
    for i in range(len(segments)):
        plt.plot([segments[i][0].x, segments[i][1].x], [segments[i][0].y, segments[i][1].y], color=color)


def draw_point(point: Point):
    plt.scatter(point.x, point.y)


def init():
    polygon_points = init_points_for_polygon()
    draw_polygon(polygon_points, "blue")
    segments = init_segments()
    draw_segments(segments, "green")

    for i in range(len(segments)):
        cirus_beck(polygon_points, segments[i])

    plt.grid(True)
    plt.show()


init()
