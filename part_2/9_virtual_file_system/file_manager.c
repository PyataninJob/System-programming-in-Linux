#include <ncurses.h>  // Библиотека для работы с текстовым пользовательским интерфейсом
#include <dirent.h>   // Библиотека для работы с директориями
#include <stdlib.h>   
#include <string.h>  
#include <unistd.h>   // Библиотека для работы с системными вызовами
#include <sys/stat.h> // Библиотека для работы с информацией о файлах

#define MAX_FILES 1024
#define MAX_FILENAME_LEN 256

// Структура для хранения информации о файле
typedef struct {
    char name[MAX_FILENAME_LEN];
    int is_dir;
    long size; // Размер файла
} FileEntry;

// Структура для хранения информации о панели
typedef struct {
    FileEntry files[MAX_FILES];
    int file_count;
    int current_selection;
    char current_path[MAX_FILENAME_LEN];
} Panel;

// Функция для инициализации панели
void init_panel(Panel *panel, const char *path) {
    panel->file_count = 0;
    panel->current_selection = 0;
    strncpy(panel->current_path, path, MAX_FILENAME_LEN); // Копирует путь в текущий путь панели
}

// Функция для сравнения двух файлов
int compare_files(const void *a, const void *b) {
    FileEntry *file_a = (FileEntry *)a; 
    FileEntry *file_b = (FileEntry *)b;

    // Сначала сортируем директории перед файлами
    if (file_a->is_dir != file_b->is_dir) {
        return file_b->is_dir - file_a->is_dir;
    }
    return strcmp(file_a->name, file_b->name);
}

// Функция для сортировки файлов в панели
void sort_files(Panel *panel) {
    qsort(panel->files, panel->file_count, sizeof(FileEntry), compare_files); // Сортирует массив файлов
}

// Функция для загрузки содержимого директории
void load_directory(Panel *panel) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_FILENAME_LEN];

    panel->file_count = 0;
    dir = opendir(panel->current_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Чтение содержимого директории
    while ((entry = readdir(dir)) != NULL) {
        snprintf(full_path, MAX_FILENAME_LEN, "%s/%s", panel->current_path, entry->d_name); // Формирует полный путь к файлу
        if (stat(full_path, &file_stat) == 0) {
            strncpy(panel->files[panel->file_count].name, entry->d_name, MAX_FILENAME_LEN);
            panel->files[panel->file_count].is_dir = S_ISDIR(file_stat.st_mode); // Устанавливает флаг, указывающий, является ли файл директорией
            panel->files[panel->file_count].size = file_stat.st_size; // Сохраняет размер файла
            panel->file_count++;
        }
    }

    closedir(dir);
    sort_files(panel);
}

// Функция для отрисовки панели
void draw_panel(WINDOW *win, Panel *panel, int active) {
    werase(win); // Очищает окно
    box(win, 0, 0); // Рисует рамку вокруг окна
    int width = getmaxx(win); // Получает ширину окна
    mvwprintw(win, 1, 1, "%-20s", "Name"); // Заголовки столбцов
    mvwprintw(win, 1, width - 20, "%10s %s",  "Size", "Type"); // Заголовки столбцов
    for (int i = 0; i < panel->file_count; i++) {
        if (i == panel->current_selection) {
            wattron(win, A_REVERSE); // инвертируем цвет выбранного 
        }
        mvwprintw(win, i + 2, 1, "%-20s", panel->files[i].name); // Выводит имя файла
        mvwprintw(win, i + 2, width - 20, "%10ld %s", panel->files[i].size, panel->files[i].is_dir ? "DIR" : ""); // Выводит размер и тип (директория или файл) с выравниванием по правому краю
        if (i == panel->current_selection) {
            wattroff(win, A_REVERSE); 
        }
    }
    if (active) {
        wattron(win, A_BOLD); // Включает режим жирного шрифта для активной панели
        mvwprintw(win, 0, 1, "Active"); // Выводит слово "Active" в верхней части окна
        wattroff(win, A_BOLD); // Выключает режим жирного шрифта для активной панели
    }
    wrefresh(win); // Обновляет окно, чтобы отобразить изменения
}

// Функция для навигации по панели
void navigate_panel(Panel *panel, int direction) {
    panel->current_selection += direction; // Изменяет текущий выбор в панели в зависимости от направления (вверх или вниз)
    if (panel->current_selection < 0) {
        panel->current_selection = 0; // упираемся вверх
    } else if (panel->current_selection >= panel->file_count) {
        panel->current_selection = panel->file_count - 1; // упираемся внизу
    }
}

// Функция для смены директории
void change_directory(Panel *panel) {
    if (panel->files[panel->current_selection].is_dir) {
        if (strcmp(panel->files[panel->current_selection].name, "..") == 0) {
            chdir(".."); // Если выбранная директория - "..", переходит на уровень выше
        } else {
            chdir(panel->files[panel->current_selection].name); // Иначе переходит в выбранную директорию
        }
        getcwd(panel->current_path, MAX_FILENAME_LEN); // Получает текущий путь
        load_directory(panel); // Загружает содержимое новой директории
    }
}

// Основная функция программы
int main() {
    Panel left_panel, right_panel;
    int active_panel = 0;
    int ch; //ввод пользователя

    initscr(); // Инициализирует библиотеку ncurses
    noecho(); // Отключает отображение вводимых символов
    cbreak(); // Включает режим мгновенного ввода
    keypad(stdscr, TRUE); // Включает обработку специальных клавиш
    curs_set(0); // Отключает отображение курсора

    WINDOW *left_win = newwin(LINES, COLS / 2, 0, 0); // Создает окно для левой панели
    WINDOW *right_win = newwin(LINES, COLS / 2, 0, COLS / 2); // Создает окно для правой панели

    init_panel(&left_panel, "."); // Инициализирует левую панель текущей директорией
    load_directory(&left_panel); // Загружает содержимое текущей директории в левую панель
    init_panel(&right_panel, "."); // Инициализирует правую панель текущей директорией
    load_directory(&right_panel); // Загружает содержимое текущей директории в правую панель

    // Цикл для обработки ввода пользователя
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                navigate_panel(active_panel == 0 ? &left_panel : &right_panel, -1); //  Вверх
                break;
            case KEY_DOWN:
                navigate_panel(active_panel == 0 ? &left_panel : &right_panel, 1); //  Вниз
                break;
            case '\t':
                active_panel = 1 - active_panel;
                break;
            case '\n':
                change_directory(active_panel == 0 ? &left_panel : &right_panel); // Меняет директорию
                break;
        }
        draw_panel(left_win, &left_panel, active_panel == 0); 
        draw_panel(right_win, &right_panel, active_panel == 1); 
    }

    delwin(left_win); // Удаляет окно левой панели
    delwin(right_win); // Удаляет окно правой панели
    endwin(); // Завершает работу с ncurses

    return 0;
}