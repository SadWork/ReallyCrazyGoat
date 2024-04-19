import subprocess
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

matplotlib.use('TkAgg')

def run_program(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    (output, error) = process.communicate()
    return output.decode().strip()

def gen_test():
    # теперь можно задавать количество точек без перекомпиляции
    run_program("./GenTest 500")
def gen_approximation():
    run_program("./Fourie_HighD < test.txt")

def show_test():
    with open("correct.txt", "r") as file:
        lines = file.readlines()

    points = [list(map(float, line.strip().split())) for line in lines[1:]]  # Skip the first line

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(*zip(*points))
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.title('Test Points')
    plt.savefig('test_plot.png')
    plt.show()

def show_approximation():
    with open("approximation.txt", "r") as file:
        lines = file.readlines()

    points = [list(map(float, line.strip().split())) for line in lines[1:]]  # Skip the first line

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(*zip(*points))
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.title('Approximation Points')
    plt.savefig('approximation_plot.png')
    plt.show()


gen_test()
gen_approximation()
show_test()
show_approximation()