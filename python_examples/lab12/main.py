from task12 import Point, construct_curve
from de_castelzho import define_curve
import matplotlib.pyplot as plt
from celluloid import Camera

fig = plt.figure()
camera = Camera(fig)

points = [Point(0, 0), Point(1, 2), Point(4, -2), Point(6, 0)]
curve = define_curve(points, camera)

xs = [point.x for point in points]
ys = [point.y for point in points]

plt.plot(xs, ys, "o")
plt.plot(xs, ys)
plt.plot([point.x for point in curve], [point.y for point in curve])


animation = camera.animate(50)
plt.show()
