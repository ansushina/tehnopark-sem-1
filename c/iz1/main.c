/*
Составить программу построчной обработки текста. Суть обработки - отбор строк,
содержащих одинаковое количество открывающих и закрывающих круглых скобок. 

Программа считывает входные данные со стандартного ввода, и печатает результат
в стандартный вывод.
 
Процедура отбора нужных строк должна быть оформлена в виде отдельной функции,
которой на вход подается массив строк (который необходимо обработать), 
количество переданных строк, а также указатель на переменную, в которой
необходимо разместить результат - массив отобранных строк. 

В качестве возвращаемого значения функция должна возвращать количество строк,
содержащихся в результирующем массиве. 

Программа должна уметь обрабатывать ошибки - такие как неверные входные данные
(отсутствие входных строк) или ошибки выделения памяти и т.п.
В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и 
завершать выполнение программы. 

ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите return -1, 
exit(1) и т.п. Даже если обнаружилась какая-то ошибка, все равно необходимо 
вернуть 0! (и напечатать [error] в stdout).
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define START_BUFFER 16
#define BUF_SIZE 128
#define STEP 2

#define OK 0
#define ERR_IO 1
#define ERR 2
#define ERR_MEMORY 3
#define ERR_NO_CORRECT 4

void free_text_mas(char **text_mas, const size_t n);
char *my_strdup(const char *str);
size_t my_getline(char **line, size_t *n, FILE *stream);
int read_text(char ***text_mas, size_t *n);
void print_text_mas(char **mas, const size_t n);
int is_correct(const char *str);
size_t count_correct_lines(char *text_mas[], const size_t n);
size_t process(char *text_mas[], const size_t n, char ***new_mas);

int main(void)
{
	setbuf(stdout, NULL);
	int rc = OK;
	
	char **text_mas;
	size_t n; 
	char **new_mas;
	size_t size;
	
	rc = read_text(&text_mas, &n);
	if (rc == OK && n != 0)
	{
		size = process(text_mas, n, &new_mas);
		if (size > 0)
		{
			print_text_mas(new_mas, size);
			free_text_mas(new_mas, size);
		}
		else if (size < 0)
		{
			printf("[memory error]\n");
		}
		else if (size == 0)
		{
			printf("[error no answer]\n");
		}
		free_text_mas(text_mas, n);
	}
	else if ( rc == ERR_MEMORY)
	{
		printf("[Memory error]\n");
	}
	else if (n == 0)
	{
		printf("[error no data]\n");
	}
	else
	{
		printf("[error]\n");
	}
    return OK; 	
}


/**
  Освобождение памятя из-под массива строк
 * @brief free_text_mas
 * @param mas[in] - массив строк
 * @param n[in] - размер массива
 */
void free_text_mas(char **text_mas, const size_t n)
{
    if (!text_mas)
        return;
    for (size_t i = 0; i < n; i++)
    {
        free(text_mas[i]);
    }
    free(text_mas);
}

/**
   Создает дубликат строки(требуется освобождение памяти после вызова)
 * @brief my_strdup
 * @param str [in] - строка
 * @return Возвращает новую строку
 */
char *my_strdup(const char *str)
{
	if (!str)
		return NULL;
	
	size_t len = strlen(str);
	
	char *new = malloc((len + 1) * sizeof(char));
	if (!new)
		return NULL;
	
	char *buf = memcpy(new, str, len + 1);
	if (!buf)
	{
		free(new);
		return NULL;
	}
	new = buf;
	
	return new;
}

/**
  Считывание одной строки и выделение под нее необходимой памяти
 * @brief my_getline
 * @param line[in,out] - указатель на строку
 * @param n[in,out] - длина строки
 * @param stream[in] - поток ввода
 * @return Возвращает длину считанной строки
 */
size_t my_getline(char **line, size_t *n, FILE *stream)
{
    if (!line || !stream || !n)
    {
        return -1;
    }
	
    size_t size = 0;//требуемый размер памяти
    size_t len = 0;
	size_t alloc_size = 0;//выделенный размер памяти - 1
    char buf[BUF_SIZE + 1];
    char *str = NULL;
	char *b = NULL;
	
	//Считываем в буфер часть строки, пока не конец файла
    while (fgets(buf, BUF_SIZE + 1, stream))
	{
		len = strlen(buf);
		if (!len)
			break;
		size += len;
		
		// если необходимо выделить еще память
		if (size > alloc_size)
		{
			b = realloc(str, size + 1);
		    if (!b)
		    {
		    	if (str)
		    	    free(str);
	    		return -1;
	    	}
		    str = b;
			alloc_size = size;
		}
		
		strncpy(str + size - len, buf, len * sizeof(char));
		//Если считалась вся строка, заканчиваем считывание.
		if (str[size - 1] == '\n')
		{
			break;
		}
	}
    if (!size)
	{
		if (str)
		    free(str);
        return -1;
	}
	str[size] = '\0';
	*line = str;
	*n = size;
	return size;
}

/**
  Считывание всех строк из стандарного потока ввода
 * @brief read_text
 * @param text_mas[out] - массив строк
 * @param n[out] - количество считанных строк
 * @return Возвращает код ошибки
 */
int read_text(char ***text_mas, size_t *n)
{
	if (!text_mas || !n)
		return ERR;
	
    char **text = malloc(START_BUFFER * sizeof(char*));
	if (!text)
		return ERR_MEMORY;
	
	char *buf = NULL;
	size_t buf_size = 0;
	size_t size = START_BUFFER;
	int i = 0;
	
	while (my_getline(&buf, &buf_size, stdin) > 1)
	{
		if (!buf || strlen(buf) == 0)
		{
			break;
		}
		text[i] = buf;
		buf = NULL;
		i++;
		//Выделим дополнительную память, если парять закочилась
		if (i == size)
		{
			char **b = realloc(text, size * STEP * sizeof(char*));
			if (!b)
			{
				free_text_mas(text, i);
				return ERR_MEMORY;
			}
			text = b;
			size *= STEP;
		}
	}
	if (buf)
	{
		free(buf);
	}
	*text_mas = text;
	*n = i;
	return OK;
}
/**
  Печатать массива строк
 * @brief print_text_mas
 * @param mas[in] - массив строк
 * @param n[in] - колиличество строк
 */
void print_text_mas(char **mas, const size_t n)
{
	for (size_t i =0; i < n; i++)
	{
		printf("%s", mas[i]);
	}
}

/**
  Проверка строки на корректность(колво открывающих скобок = колво закрывающих)
 * @brief is_correct
 * @param str[in] - строка
 * @return 1 - строка корректна, 0 - строка некорректна или не существует
 */
int is_correct(const char *str)
{
	if (!str)
		return 0;
	size_t l = strlen(str);
	size_t open = 0, close = 0;
	for (size_t i = 0; i < l; i++)
	{
		if (str[i] == '(')
			open++;
		if (str[i] == ')')
			close++;
	}
	if (open == close)
		return 1;
	return 0;
}

/**
  Подсчет количества строк, удовлетворяющих условию
 * @brief count_correct_lines
 * @param text_mas[in] - массив строк
 * @param n[in] - количество исходных строк
 * @return Возвращает количество строк, удовлетворяющих условию
 */
size_t count_correct_lines(char *text_mas[], const size_t n)
{
    if (!text_mas || !n)
		return 0;
	
	size_t count = 0;
	for (size_t i = 0; i < n; i++)
	{
	    if (is_correct(text_mas[i]))
			count++;
	}
	return count;
}

/**
  Создание массива из строк, удовблетворяющих условию
 * @brief process
 * @param text_mas[in] - исходный массив строк
 * @param n[in] - количество строк в исходном массиве
 * @param new_mas[out] - новый массив строк
 * @return Возвращает количество строк, записанных в новый массив
 */
size_t process(char *text_mas[], const size_t n, char ***new_mas)
{
	if (!text_mas || !n || !new_mas)
		return -1;
	
	size_t count = count_correct_lines(text_mas, n);
	if (count == 0)
		return 0;
	
	char **new = malloc(sizeof(char*) * count);
	if (!new)
		return -1;
	
	count = 0;
	for (size_t i = 0; i < n; i++)
	{
		if (is_correct(text_mas[i]))
		{
			new[count] = my_strdup(text_mas[i]);
			if (!(new[count]))
			{
				free_text_mas(new, count);
				return -1;
			}
			count++;
		}
	}
	*new_mas = new;
	return count;
}