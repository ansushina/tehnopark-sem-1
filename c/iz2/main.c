
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
	el_t *new = malloc(sizeof(el_t));
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
	//printf("[%c]", line[i]);
	while(line[i] >= '0' && line[i] <= '9')
	{
		i++;
		//printf("[=%c=]", line[i]);
	}
	*count = i;
	//printf("[%d]", *count);
	int num = 0;
	for (int j = 0; j < i; j++)
	{
		num += (line[i-j-1] - '0') * pow(10, j);
		//printf("[%c]", line[i-j-1]);
	}
	return num;
}

data_t *create_data(char *line)
{
	size_t count = 0;
	size_t i = 0;
	//printf("dasdasdasd %c ", line[i]);
	while(line[i] != ']')
	{
		if (line[i] >= '0' && line[i] <= '9')
		{
		    while(line[i] >= '0' && line[i] <= '9')
		    {
		    	i++;
	    	}
	    	count++;
			i--;
		}
		i++;
	}
	
	printf("[%d]", count);
	if (count == 0)
		return NULL;
	//([1,2]+[3,4])

	//printf(" 9 %d ", sizeof(struct data)); //16
	data_t *new = (data_t *)malloc(sizeof(struct data));
	//printf(" 9 ");
	int *buf = NULL;
	//printf(" 9 ");
	if (new)
	{
		//printf(" 9 ");
		buf = malloc(count * sizeof(int));
		//printf(" 9 ");
		if (!buf)
		{
			free(new);
			return NULL;
		}
	}
	i = 0;
	count = 0;
	//printf("[%d]", count);
	while(line[i] != ']')
	{
		if (line[i] > '0' && line[i] < '9')
		{
		    int j = 0;
			int a = char_to_int(line + i,&j);
			i += j - 1;
			buf[count] = a;
			//printf("[%d]", count);
			count++;
		}
		i++;
	}
	//printf("[%d]", count);
	new->mas = buf;
	new->n = count;
	for (int i = 0; i < new->n; i++)
	{
		printf("%d+ ", buf[i]);
	}
	return new;
}


char *my_strdup(const char *str);
size_t my_getline(char **line, size_t *n, FILE *stream);

el_t *make_spisok_from_line(char *line, size_t n);

int main(void)
{
	setbuf(stdout, NULL);
	int rc = OK;
	
	char *line;
	size_t size; 
	el_t *sp = NULL;
	
	rc = my_getline(&line, &size, stdin);
	if (rc > 0)
	{
		printf("%s", line);
		sp = make_spisok_from_line(line, size);
		free(line);
	}
	else if ( rc == ERR_MEMORY)
	{
		printf("[Memory error]\n");
	}
	else if (size == 0)
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
	size_t i = 0;
	for (i = 0; i < n ; i++)
	{
		printf("%d - %c\n ", i, line[i]);
		if (line[i] == '(' || line[i] == ')' ||
		    line[i] == '^' || line[i] == '|' ||
			line[i] == 'U')
		{
			data_t *data = NULL;
			//printf("iasodiao %c", line[i+1]);
			if (line[i+1] == '[')
			{
				//printf("  sas ");
				data = create_data(line+i+1);
			}
			//printf("  111 ");
		    el_t *new = create_el(line[i], data);
			if (!new)
			{
				continue;
			}
			el->next = new;
			new->prev = el;
			el = new;
		}
	}
	return head;
}

int is_in(data_t *data, int n)
{
	for(size_t i = 0; i < data->n; i++)
	{
		if (data->mas[i] == n)
			return 1;
	}
	return 0;
}

data_t *alloc_data_t(size_t count)
{
	data_t *new = (data_t *)malloc(sizeof(struct data));
	int *buf = NULL;
	if (new)
	{
		buf = malloc(count * sizeof(int));
		if (!buf)
		{
			free(new);
			return NULL;
		}
	}
	new->mas = buf;
	new->n = count;
	return new;
}

data_t *obed(data_t *d1, data_t *d2)
{
	int count = 0;
	for (size_t i = 0 ; i < d2->n; i++)
	{
		if (!is_in(d1,d2->mas[i]))
			count++;
	}
	data_t *new = alloc_data_t(count + d1->n);
	if (!new)
		return NULL;
	count = 0;
	for (size_t i = 0 ; i < d1->n; i++)
	{
		new->mas[count] = d1->mas[i];
		count++;
	}
	for (size_t i = 0 ; i < d2->n; i++)
	{
		if (!is_in(d1,d2->mas[i]))
		{
			new->mas[count] = d2->mas[i];
			count++;
		}	
	}
	
	return new;
}

data_t *peres(data_t *d1, data_t *d2)
{
	int count = 0;
	for (size_t i = 0 ; i < d2->n; i++)
	{
		if (is_in(d1,d2->mas[i]))
			count++;
	}
	data_t *new = alloc_data_t(count);
	if (!new)
		return NULL;
	count = 0;
	for (size_t i = 0 ; i < d2->n; i++)
	{
		if (is_in(d1,d2->mas[i]))
		{
			new->mas[count] = d2->mas[i];
			count++;
		}	
	}
}

data_t *raz(data_t *d1, data_t *d2)
{
	int count = 0;
	for (size_t i = 0 ; i < d1->n; i++)
	{
		if (!is_in(d2,d1->mas[i]))
			count++;
	}
	data_t *new = alloc_data_t(count);
	if (!new)
		return NULL;
	count = 0;

	for (size_t i = 0 ; i < d1->n; i++)
	{
		if (!is_in(d2,d1->mas[i]))
		{
			new->mas[count] = d1->mas[i];
			count++;
		}	
	}
	
	return new;
}


int process(el_t *head, data_t *otvet)
{
	
}