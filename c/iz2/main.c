
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define START_BUFFER 16
#define BUF_SIZE 128
#define STEP 2

#define OK 0
#define ERR_IO 1
#define ERR 2
#define ERR_MEMORY 3
#define ERR_NO_CORRECT 4

typedef struct data data_t;
struct data
{
	int *mas;
	size_t n;
};

typedef struct element el_t;
struct element 
{
	char el;
	data_t *data;
	el_t *next;
	el_t *prev;
};

el_t *create_el(char el, data_t *data)
{
	el_t *new = malloc(sizeof(data_t));
	if (new)
	{
		new->data = data;
		new->next = NULL;
		new->prev = NULL;
	}
	return new;
}

int char_to_int(char *line, int *count)
{
	int i = 0;
	while(line[i] >= '0' && line[i] <= '9')
	{
		i++;
	}
	*count = i;
	int num = 0;
	for (int j = 0; j < i; j++)
	{
		num += (line[i-j] - '0') * pow(10, j);
	}
	return num;
}

data_t *create_data(char *line)
{
	size_t count = 0;
	size_t i = 0;
	while(line[i] != ']')
	{
		if (line[i] >= '0' && line[i] <= '9')
		{
		    while(line[i] >= '0' && line[i] <= '9')
		    {
		    	i++;
	    	}
	    	count++;
		}
		i++;
	}
	
	if (count == 0)
		return NULL;
	
	data_t * new = malloc(sizeof(data_t));
	if (new)
	{
		int *buf = malloc(count * sizeof(int));
		if (!buf)
		{
			free(new);
			return NULL;
		}
	}
	i = 0;
	count = 0;
	while(line[i] != ']')
	{
		if (line[i] > '0' && line[i] < '9')
		{
		    int j;
			int a = char_to_int(line + i,&j);
			i += j;
			buf[count] = a;
			count++;
		}
		i++;
	}
	
	new->data = buf;
	new->n = count;
	return new;
}

char *my_strdup(const char *str);
size_t my_getline(char **line, size_t *n, FILE *stream);

int main(void)
{
	setbuf(stdout, NULL);
	int rc = OK;
	
	char *line;
	size_t n; 
	
	rc = my_getline(&line, &size, stdin);
	if (rc > 0)
	{
		
		free(line);
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

el_t *make_spisok_from_line(char *line, size_t n)
{
	el_t *head = create_el('$',NULL);
	el_t *el = head;
	int i = 0;
	for (i = 0; i < n ; i++)
	{
		if (line[i] == '(' || line[i] == ')' ||
		    line[i] == '*' || line[i] == '\' ||
			line[i] == '+')
		{
			data_t *data = NULL;
			if (line[i+1] == '[')
			{
				data = create_data(line+i+1);
			}
		    el_t *new = create_el(line[i], data);
			el->next = new;
			new->prev = el;
			el = new;
		}
	}
	return head;
}