/*
 * Сушина А АПО-13
Задача B-1. Калькулятор для числовых множеств
Time limit:	14 s
Memory limit:	64 M
Разработайте программу-калькулятор, умеющую вычислять арифметические 
выражения над множествами натуральных чисел. Входные данные 
(выражение с описанием множеств и производимых над ними операций) 
подаются на стандартный поток ввода программы, результат вычислений 
должен подаваться на стандартный поток вывода. 

Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' 
- разность множеств, '()' - задание приоритета вычислений.
Множества задаются в виде [ el1, el2, ... ], где элементами являются
 обычные натуральные числа.
Множества могут быть пустыми.
Между операциями, множествами и элементами множеств может быть любое 
количество пробелов.

При вычислениях должны учитываться приоритеты операций (в том числе,
 заданные вручную при помощи круглых скобочек).

Результирующее множество должно выводиться без пробелов, в 
отсортированном порядке.
В случае, если введенное выражение содержит ошибку, необходимо 
вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы. 

ВАЖНО! Программа в любом случае должна возвращать 0. Не пишите 
return -1, exit(1) и т.п. Даже если обнаружилась какая-то ошибка, 
все равно необходимо вернуть 0! (и напечатать [error] в stdout).
*/

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


//Элемент двусвязного списка, в котором хранятся элементы ()U^\ и $
//каждому элементу соответсвует идущее после него множество или NULL
typedef struct element el_t;
struct element 
{
	char el;
	data_t *data;
	el_t *next;
	el_t *prev;
};

int char_to_int(const char *line, int *count);
void sort_data_t(data_t *data);
void print_data_t(data_t *data);
void free_data_t(data_t *data);
size_t count_nums(const char *line);
data_t *alloc_data_t(const size_t count);
data_t *create_data(const char *line);

char *my_strdup(const char *str);
size_t my_getline(char **line, size_t *n, FILE *stream);

int is_normal(const char *line);
int iscorrect(const char *line);
void delete_spaces(char *str);

int is_in(data_t *data, const int n);
data_t *do_union(data_t *d1, data_t *d2);
data_t *do_intersection(data_t *d1, data_t *d2);
data_t *do_subtraction(data_t *d1, data_t *d2);

el_t *make_spisok_from_line(const char *line);
el_t *create_el(char el, data_t *data);
el_t *delete_el_t(el_t *el);
void free_el_t(el_t *el);
void free_sp(el_t *sp);

int check_line(char *line);
int find_bracets(el_t *head, el_t **open, el_t **close);
int make_action_in_brackets(el_t *open, el_t *close, char action);
int process(el_t *head, data_t **otvet);

int main(void)
{
	setbuf(stdout, NULL);
	int rc = OK;
	
	char *line;
	size_t size; 
	el_t *sp = NULL;
	rc = my_getline(&line, &size, stdin);
	if (rc > 1)
	{
        //проверим проавильность строки
		if (check_line(line))
		{
            // создадим список с значищами элементами из строки
		    sp = make_spisok_from_line(line);
	    	if (sp)
	    	{
                data_t *otvet = NULL;
	    		rc = process(sp, &otvet);
                if (rc == OK)
				{
					print_data_t(otvet);
				}
                else
                {
                    printf("[error]\n");
                }
                free_sp(sp);
	    	}
	    	else 
	    	{
	    		printf("[error]\n");
	    	}
		}
		else 
		{
			printf("[error]\n");
		}
	}
	else
	{
		printf("[error]\n");
	}
	free(line);
    return OK; 	
}
/////////////////////////////////////////////////////////////////////////////////
//io
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
		if (feof(stream) || ferror(stream))
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
////io
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
////string changes
/**
  Проверяет правильность заполнения квадратных скобок
 * @brief is_normal
 * @param line[in] - строка
 * @return Возвращает количество прочитанных символов, \
 0 если строка не подходит
 */
int is_normal(const char *line)
{
	int i = 1;
	int flag = 0; //флаг найденного числа
	while (line[i] != ']')
	{
		//printf("j [%c] ", line[i]);
	    if (line[i] == '(' || line[i] == ')' || line[i] == '[')
		    return 0;
		if (line[i] >= '0' && line[i] <= '9' && !flag)
		{
			//если найдено число читаем его до конца
		    while(line[i] >= '0' && line[i] <= '9')
		    {
		    	i++;
	    	}
			flag = 1;
		}
	    //если только что найдено число, но мы снова встретили число
		//выходим из проверки
		else if (flag && line[i] >= '0' && line[i] <= '9')
		{
			return 0;
		}
		//если найдена запятая, можно искать следующее число
		if (line[i] == ',' && flag) 
			flag = 0;
		else if (line[i] == ']')
			break;
		else if (line[i] != ' ') //если непредвиденный символ
			return 0;
		i++;
	}
	return i;
}

/**
  Проверяет правильность заполнения строки
 * @brief iscorrect
 * @param line[in] - строка
 * @return 1 - верная строка, 0 - неверная
 */
int iscorrect(const char *line)
{
    if (!line)
	{
        return 0;
	}
	size_t i = 0;
	int flag = 0;//флаг только что открытой скобки 
	//равен нулю, когда встречает множество
	int action_count = 0;//счетчий действий 
	int set_count = 0;//счетчик множеств
	size_t c_open = 0;// (
	size_t c_close = 0; // )
	char sym[] = "^U\\";
	while (line[i] != '\0')
	{
		//printf("i[%c] ", line[i]);
		if (line[i] == '[' ) //если встретили множество
		{
			flag = 0;
			int k = is_normal(line+i);
			if (k < 1)
				return 0;
			i += k; //пропускаем прочитанную часть
			set_count++;
		}
		else if (line[i] == '(')
		{
			c_open++;
			flag = 1;
		}
		else if (line[i] == ')')
		{
			if (flag == 1)
				return 0;
			c_close++;
			if (c_close > c_open)
			{
				printf("here");
				return 0;
			}
		}
		else if (line[i] == ']')// закрывающая скобка до открывающей
			return 0;
			// если встречено действие, но оно идет сразу за скобкой
		else if ((line[i] == sym[0] || line[i] == sym[1] || line[i] == sym[2]) && flag)
			return 0;
		else if (line[i] == 13 || line[i] == '\n') 
			break;
		// если какой-то непредвиденный символ
		else if (line[i] != ' ' && !(line[i] == sym[0] || line[i] == sym[1] || line[i] == sym[2]))
			return 0;
		// если встречено действие
		else if (line[i] == sym[0] || line[i] == sym[1] || line[i] == sym[2])
		{
			action_count++;
			if (action_count < set_count)
				return 0;
		}
		i++;
	}
	if (c_open != c_close)
		return 0;
	return 1;
}

/**
  Удаление пробелов из строки
 * @brief delete_spaces
 * @param str[in, out] - строка
 */
void delete_spaces(char *str)
{
	if (str == NULL)
		return;
	
	int i = 0;
	int j = 0;
	while (str[i] == ' ')
	{
		i++;
	}
	while (str[i] != '\0')
	{
		while (str[i] != ' ' && str[i] != '\0')
		{
			str[j] = str[i];
			j++;
			i++;
		}
		while (str[i] == ' ')
		{
			i++;
		}
	}
	if (str[j - 1] == ' ')
	    str[j - 1] = '\0';
	else 
		str[j] = '\0';
}

/**
  Проверка, что строка подходит для вычислений
 * @brief check_line
 * @param line [in] - строка
 * @return 1 - подходит, 0 - не подходит
 */
int check_line(char *line)
{
    if (!line)
    {
        return 0;
    }
	int rc = iscorrect(line);
	if (rc)
	{
		delete_spaces(line);
		return rc;
	}
	return rc;
}
////string changes
////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//DATA
/**
  Сортировка элементов множества по возрастанию
 * @brief sort_data_t
 * @param data [in] - множество
 */
void sort_data_t(data_t *data)
{
    for (int i = 0; i < data->n; i++)
    {
        for (int j = 0; j < data->n - 1; j++)
        {
            if (data->mas[j] > data->mas[j+1])
            {
                int t = data->mas[j];
                data->mas[j] = data->mas[j+1];
                data->mas[j+1] = t;
            }
        }
    }
}

/**
  Печать множества в отсортированном виде
 * @brief print_data_t
 * @param data[in] - множество
 */
void print_data_t(data_t *data)
{
	if (!data)
		return;
	if (data->n == 0)
	{
		printf("[]\n");
		return;
	}
    sort_data_t(data);
    printf("[");
    for (int i = 0; i < data->n - 1; i++)
    {
        printf("%d,", data->mas[i]);
    }
    printf("%d",data->mas[data->n-1]);
    printf("]\n");
}

/**
  Переводит число в символах в формат int
 * @brief char_to_int
 * @param line[in] - строка
 * @param count[out] - количество прочитанных символов
 * @return полученное число
 */
int char_to_int(const char *line, int *count)
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
		num += (line[i-j-1] - '0') * pow(10, j);
	}
	return num;
}

/**
  Выделение памяти под множество размера count
 * @brief alloc_data_t
 * @param count [in] - размер множества
 * @return Возвращает новый элемент-множество
 */
data_t *alloc_data_t(const size_t count)
{
	data_t *new = (data_t *)malloc(sizeof(struct data));

    if (count == 0) //если пустое множество
	{
		new->mas = NULL;
		new->n = 0;
		return new;
    }

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

/**
  Освобождение памяти из-под множества
 * @brief free_data_t
 * @param data[in] - множество
 */
void free_data_t(data_t *data)
{
	if (!data)
		return;
	if (data->mas)
		free(data->mas);
	free(data);
}

/**
  Подсчет количества чисел между скобками
 * @brief count_nums
 * @param line[in] - строка
 * @return Возвращает количество чисел между []
 */
size_t count_nums(const char *line)
{
	size_t count = 0;
	size_t i = 0;
    if ( !line )
    {
        return 0;
    }

	while(line[i] != ']')
	{
        //если нашли число
		if (line[i] >= '0' && line[i] <= '9')
		{
            //считаем число до конца
		    while(line[i] >= '0' && line[i] <= '9')
		    {
		    	i++;
	    	}
	    	count++;
			i--;
		}
		i++;
	}
	return count;
}

/**
  Создание множества из строки
 * @brief create_data
 * @param line[in] - строка
 * @return ВОзвращает указатель на множество
 */
data_t *create_data(const char *line)
{
	size_t count = count_nums(line);
    if (!line)
        return NULL;
    data_t *new = alloc_data_t(count); //count = 0 проверяется внутри функции
	if (!new)
		return NULL;
	
	size_t i = 0;
    count = 0;
	while(line[i] != ']')
	{
		if (line[i] >= '0' && line[i] <= '9')
		{
		    int j = 0;
            int a = char_to_int(line + i, &j);
			i += j - 1;
			new->mas[count] = a;
			count++;
		}
		i++;
	}
	return new;
}
///DATA
////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/// actions

/**
  Проверка, находится ли элемент в множестве
 * @brief is_in
 * @param data[in] - множество
 * @param n[in] - элемент
 * @return 1 - находится, 0 - не находится
 */
int is_in(data_t *data, const int n)
{
	for(size_t i = 0; i < data->n; i++)
	{
		if (data->mas[i] == n)
			return 1;
	}
	return 0;
}

/**
  Объединение
 * @brief do_union
 * @param d1[in] - 1 множество
 * @param d2[in] - 2 множество
 * @return результат операции
 */
data_t *do_union(data_t *d1, data_t *d2)
{
    if (!d1 || !d2)
    {
        return NULL;
    }
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

/**
  Пересечение
 * @brief do_intersection
 * @param d1[in] - 1 множество
 * @param d2[in] - 2 множество
 * @return результат операции
 */
data_t *do_intersection(data_t *d1, data_t *d2)
{
    if (!d1 || !d2)
    {
        return NULL;
    }
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
	return new;
}

/**
  Вычитание
 * @brief do_subtraction
 * @param d1[in] - 1 множество
 * @param d2[in] - 2 множество
 * @return результат операции
 */
data_t *do_subtraction(data_t *d1, data_t *d2)
{
    if (!d1 || !d2)
    {
        return NULL;
    }
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

///actions
//////////////////////////////////////////////////

/////////////////////////////////////////////////
/////EL_T
/**
  Освобождение памяти из-под списка
 * @brief free_sp
 * @param sp[in] - голова списка
 */
void free_sp(el_t *sp)
{
    if (!sp)
        return;

    while (sp)
    {
        free_data_t(sp->data);
        el_t *tmp = sp;
        sp = sp->next;
        free(tmp);
    }
}

/**
  Выделение памяти под один элемент списка
 * @brief create_el
 * @param el[in] - элемент
 * @param data[in] - указатель на множество
 * @return указатель на созданный элемент
 */
el_t *create_el(char el, data_t *data)
{
	el_t *new = malloc(sizeof(el_t));
	if (new)
	{
		new->el = el;
		new->data = data;
		new->next = NULL;
		new->prev = NULL;
	}
	return new;
}

/**
  Освобождение одного элемента списка
 * @brief free_el_t
 * @param el[in] - элемент списка
 */
void free_el_t(el_t *el)
{
	if (!el)
		return;
	free(el);
}

/**
  Удаление элемента из списка
 * @brief delete_el_t
 * @param el [in] - удаляемый элемент
 * @return следующий элемент
 */
el_t *delete_el_t(el_t *el)
{
    if (el->next)
    {
        el->next->prev = el->prev;
    }
    el->prev->next = el->next;
    el_t *tmp = el;
    el = el->next;
    free_data_t(tmp->data);
    free_el_t(tmp);
    return el;
}


/**
  Создание списка из строки
 * @brief make_spisok_from_line
 * @param line [in] строка
 * @return указатель на началло списка
 */
el_t *make_spisok_from_line(const char *line)
{
	el_t *head = create_el('$',NULL);
	el_t *el = head;
	size_t i = 0;
	char sym[] = "^U\\()";
	size_t l = strlen(sym);
	size_t n = strlen(line);
	if (line[0] == '[')
	{
		data_t *data = NULL;
		if (line[0] == '[')
		{
		    data = create_data(line);
	    }
	    el->data = data;
	    i++;
	}
	for (i=i; i < n ; i++)
	{
		for (size_t j = 0; j < l; j++)
		{
		    if (line[i] == sym[j])
		    {
		    	data_t *data = NULL;
				if (i+1 > n)
					break;
		    	if (line[i+1] == '[')
		    	{
		     		data = create_data(line+i+1);
	    		}
	    	    el_t *new = create_el(line[i], data);
	    		if (!new)
	    		{
                    free_sp(head);
                    return NULL;
	    		}
	    		el->next = new;
	    		new->prev = el;
	    		el = new;
				break;
	    	}
		}
	}
	return head;
}

////EL_T
///////////////////////////////////


///////////////////////////////////////////
///process

/**
  Поиск круглых скобочек
 * @brief find_bracets
 * @param head[in] - начало списка
 * @param open[out] - указатель на открывающую скобку
 * @param close[out] - указатель на закрывающую скобку
 * @return 1 - если найдены, 0 - не найдены
 */
int find_bracets(el_t *head, el_t **open, el_t **close)
{
	if (!head)
		return 0;
	
	while(head != NULL)
	{
		if (head->el == '(')
		{
			*open = head;
		}
		else if (head->el == ')')
		{
			*close = head;
			return 1;
		}
		head = head->next;
	}
	return 0;
}

/**
  Сделать действие в скобках
 * @brief make_action_in_brackets
 * @param open [in] - открывающая скобка
 * @param close [in] - закрывающая скобка
 * @param action [in] - действие, которое нужно выполнить
 */
int make_action_in_brackets(el_t *open, el_t *close, char action)
{
	el_t *el = open;
	data_t *buf;
	while (el != close)
	{
		if (el->el == action)
		{
			if (action == 'U')
			{
				buf = do_union(el->prev->data, el->data);
			}
			else if (action == '^')
			{
				buf = do_intersection(el->prev->data, el->data);
			}
			else
			{
				buf = do_subtraction(el->prev->data, el->data);
			}
			if (!buf)
                return ERR_MEMORY;
			free_data_t(el->prev->data);
			el->prev->data = buf; 
            el = delete_el_t(el);
		}
		else
		{
			el = el->next;
		}
	}
    return OK;
}

/**
  Выполняет вычисление ответа
 * @brief process
 * @param head[in]
 * @param otvet[out]
 * @return
 */
int process(el_t *head, data_t **otvet)
{
	el_t *open = NULL;
	el_t *close = NULL;
	char sym[] = "^U\\";
	size_t l = strlen(sym);
    int rc = OK;
	
	while(find_bracets(head, &open, &close))
	{
		for (size_t i = 0; i < l; i++)
		{
            rc = make_action_in_brackets(open, close, sym[i]);
            if (rc != OK)
                 return rc;
		}
		open->prev->data = open->data;
		open->prev->next = close->next;
		if (close->next)
		{
		     close->next->prev = open->prev;
		}
		free_el_t(open);
		free_el_t(close);
		open = NULL;
		close = NULL;
	}
	
	for (size_t i = 0; i < l; i++)
	{
		make_action_in_brackets(head, NULL, sym[i]);
	}
	*otvet = head->data;
    //free_el_t(head);
    return OK;
}
///process
////////////////////////////////
