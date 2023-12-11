from PIL import Image

def convert_coordinates(x, y, width, height):
    # Преобразование координат изображения от -1 до 1
    normalized_x = 2 * (x / width) - 1
    normalized_y = 2 * (y / height) - 1
    return normalized_x, normalized_y

def main():
    # Открываем изображение
    image_path = '071223/img.jpg'
    image = Image.open(image_path)
    
    # Получаем размеры изображения
    width, height = image.size

    # Открываем файлы для записи точек
    with open('points_red.txt', 'w') as f_red, \
         open('points_green.txt', 'w') as f_green, \
         open('points_blue.txt', 'w') as f_blue:

        # Итерируемся по пикселям изображения
        for y in range(height):
            for x in range(width):
                # Получаем цвет пикселя
                pixel_color = image.getpixel((x, y))
                
                # Преобразуем координаты
                normalized_x, normalized_y = convert_coordinates(x, y, width, height)

                # Записываем в соответствующий файл
                f_red.write(f"{normalized_x} {normalized_y} {pixel_color[0] / 255.0}\n")
                f_green.write(f"{normalized_x} {normalized_y} {pixel_color[1] / 255.0}\n")
                f_blue.write(f"{normalized_x} {normalized_y} {pixel_color[2] / 255.0}\n")

    print("Преобразование завершено. Результат записан в файлы points_red.txt, points_green.txt и points_blue.txt")

if __name__ == "__main__":
    main()
