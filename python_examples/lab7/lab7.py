import random
import time
import numpy as np
import matplotlib.pyplot as plt


points = []
convex_hull = []


def point_position(p1, p2, p0):
    v = np.cross(p2 - p1, p0 - p1)
    if v > 0:
        return "left"
    elif v < 0:
        return "right"
    else:
        return "on the line"


def middle_point(p1, p2, p3):
    if np.dot(p3 - p1, p3 - p2) < 0:
        return p3
    if np.dot(p2 - p1, p2 - p3):
        return p2
    return p1


def dynamic_convexhull(point, ch):
    new_ch = []
    if len(ch) == 0:
        ch.append(point)
        return ch
    if len(ch) == 1:
        ch.append(point)
        return ch
    if len(ch) == 2:
        if point_position(ch[0], ch[1], point) == "on the line":
            ch.clear()
            mid_point = middle_point(points[0], points[1], points[2])

            if mid_point == points[0]:
                ch.append(points[1])
                ch.append(points[2])
            elif mid_point == points[1]:
                ch.append(points[0])
                ch.append(points[2])
            else:
                ch.append(points[1])
                ch.append(points[2])

        elif point_position(ch[0], ch[1], point) == "left":
            ch.append(point)
        else:
            ch.clear()
            ch.append(points[0])
            ch.append(points[2])
            ch.append(points[1])
        return ch
    else:
        point_right_num = 0
        point_left_num = 0
        right = []
        for i in range(0, len(ch) - 1):
            if point_position(ch[i], ch[i + 1], point) == "right":
                right.append(i)
                right.append(i + 1)

        if point_position(ch[-1], ch[0], point) == "right":
            i = 0
            while point_position(ch[i], ch[i + 1], point) == "right":
                i += 1
            point_right_num = i

            i = len(ch) - 1
            while point_position(ch[i - 1], ch[i], point) == "right":
                i -= 1
            point_left_num = i
            if len(ch) - 1 not in right:
                right.append(len(ch) - 1)
            if 0 not in right:
                right.append(0)

        if not right:
            return ch

        if point_position(ch[-1], ch[0], point) == "left":
            new_ch.extend(ch[0:right[0] + 1])
            new_ch.append(point)
            new_ch.extend(ch[right[-1]:len(ch)])

        elif point_position(ch[-1], ch[0], point) == "right":
            new_ch.append(point)
            new_ch.extend(ch[point_right_num:point_left_num + 1])

    return new_ch


if __name__ == '__main__':
    points = []
    convex_hull = []

    plt.ion()
    for _ in range(100):
        new_point = np.array([random.randint(-20, 20), random.randint(-20, 20)])
        points.append(new_point)

        convex_hull = dynamic_convexhull(new_point, convex_hull)

        print(convex_hull)

        points_x = [p[0] for p in points]
        points_y = [p[1] for p in points]

        plt.clf()

        plt.scatter(points_x, points_y, marker='o', linestyle='-', color='blue')

        points_x = [p[0] for p in convex_hull]
        points_y = [p[1] for p in convex_hull]

        points_x.append(convex_hull[0][0])
        points_y.append(convex_hull[0][1])

        for index, item in enumerate(convex_hull):
            plt.annotate(f'p{index}', (item[0], item[1]))

        plt.plot(points_x, points_y, marker='o', color='red')
        plt.draw()
        plt.gcf().canvas.flush_events()

        time.sleep(0.01)
