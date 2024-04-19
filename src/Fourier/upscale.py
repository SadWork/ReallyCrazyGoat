import subprocess
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os
import tempfile
from threading import Thread
import threading
import hashlib

matplotlib.use('TkAgg')

# Пока что из доп библиотек нужна только matplotlib: pip install matplitlib



def run_program(command):
    # Преобразование команды в строку для создания уникального имени файла
    command_str = subprocess.list2cmdline(command)
    hash_object = hashlib.md5(command_str.encode())
    output_file = f"tmp/{hash_object.hexdigest()}.txt"

    with open(output_file, 'w') as output_file_handle:
        process = subprocess.Popen(
            command,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            shell=True,
            text=True,
            bufsize=1,
            universal_newlines=True
        )
        for line in process.stdout:
            print(line, end='')  # Печать вывода в реальном времени
            output_file_handle.write(line)  # Запись вывода в файл
        process.communicate()  # Дождитесь завершения выполнения команды
        return process.returncode, output_file  # Возвращает код завершения и имя файла



def gen_test():
    run_program("python img.py")

def gen_approximation(input_file, output_file):
    run_program("./Fourie_IMG_SFML2 < "+input_file +" "+output_file)


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

def restore_img():
    run_program("python restorve.py")

def run_in_thread(function, *args, **kwargs):
    thread = threading.Thread(target=function, args=args, kwargs=kwargs)
    thread.start()
    return thread

def main():
    gen_test()

    thread1 = run_in_thread(gen_approximation, "points_blue.txt", "approximation_blue.txt")
    thread2 = run_in_thread(gen_approximation, "points_green.txt", "approximation_green.txt")
    thread3 = run_in_thread(gen_approximation, "points_red.txt", "approximation_red.txt")

    # Дождитесь завершения всех потоков
    thread1.join()
    thread2.join()
    thread3.join()

    restore_img()

if __name__ == "__main__":
    main()