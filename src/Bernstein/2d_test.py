import subprocess
import argparse
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

matplotlib.use("TkAgg")


def run_program(command):
    process = subprocess.Popen(
        command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True
    )
    (output, error) = process.communicate()
    return output.decode().strip()


def gen_test():
    print(run_program("./GenTest.exe 100000 1"))


def gen_approximation(apr_program):
    gradient_steps = 1000
    bernstein_size = 2
    run_program(f"{apr_program} {gradient_steps} {bernstein_size} < ~test.txt")


def show_test():
    with open("~correct.txt", "r") as file:
        lines = file.readlines()

    points = [
        list(map(float, line.strip().split())) for line in lines[1:]
    ]  # Skip the first line

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.scatter(*zip(*points))
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.title("Test Points")
    plt.savefig("~test_plot.png")
    plt.show()


def show_approximation():
    with open("~approximation.txt", "r") as file:
        lines = file.readlines()

    points = [
        list(map(float, line.strip().split())) for line in lines[1:]
    ]  # Skip the first line

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.scatter(*zip(*points))
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.title("Approximation Points")
    plt.savefig("~approximation_plot.png")
    plt.show()


if __name__ == "__main__":
    # Get program path from the arguments
    parser = argparse.ArgumentParser(
        description="Generate and visualize data using approximation program."
    )
    parser.add_argument(
        "--program_path",
        type=str,
        required=True,
        help="Path to the approximation program executable",
    )
    args = parser.parse_args()

    gen_test()
    gen_approximation(args.program_path)
    show_test()
    show_approximation()
