from scipy.optimize import fsolve
import numpy as np

def equations(variables):
    a, b = variables
    eq1 = (b - a) / (1 + np.exp(1e5/ (a - b))) + a - 1e-1
    eq2 = (b - a) / (1 + np.exp(1 / (a - b))) + a - 1e-2
    return [eq1, eq2]

a, b = fsolve(equations, (0, 0))
print("a =", a)
print("b =", b)
