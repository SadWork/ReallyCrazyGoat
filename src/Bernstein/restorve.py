from PIL import Image

def convert_coordinates(normalized_x, normalized_y, width, height):
    # Преобразование координат изображения от -1 до 1
    x = round((normalized_x) * width)
    y = round((normalized_y) * height)
    return int(x), int(y)

def reconstruct_image(file_red, file_green, file_blue, output_path):
    # Читаем размеры из первых строк файлов
    with open(file_red, 'r') as f_red, open(file_green, 'r') as f_green, open(file_blue, 'r') as f_blue:
        width, height = map(int, f_red.readline().split())

        # Пропускаем первую строку в каждом файле
        f_green.readline()
        f_blue.readline()

        # Читаем остальные строки
        data_red = [list(map(float, line.split())) for line in f_red.readlines()]
        data_green = [list(map(float, line.split())) for line in f_green.readlines()]
        data_blue = [list(map(float, line.split())) for line in f_blue.readlines()]
        
    # Создаем пустое изображение
    reconstructed_image = Image.new('RGB', (width, height))

    # Перебираем данные и устанавливаем цвет для каждого пикселя
    for i in range(len(data_red)):
        x, y = convert_coordinates(data_red[i][0], data_red[i][1], width, height) 
        color = (
            int(data_red[i][2] * 255),
            int(data_green[i][2] * 255),
            int(data_blue[i][2] * 255)
        )
        reconstructed_image.putpixel((x, y), color)

    # Сохраняем восстановленное изображение
    reconstructed_image.save(output_path)
    print(f"Восстановление завершено. Изображение сохранено в {output_path}")

# Пример использования
reconstruct_image('points_red.txt', 'points_green.txt', 'points_blue.txt', 'original_image.png')
reconstruct_image('approximation_red.txt', 'approximation_green.txt', 'approximation_blue.txt', 'approximation_image.png')
