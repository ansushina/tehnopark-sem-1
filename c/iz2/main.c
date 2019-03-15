/*
 * Сушина А АПО-13
Задача B-1. Калькулятор для числовых множеств
Time limit:    14 s
Memory limit:    64 M
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

#define START_BUFFER 16
#define BUF_SIZE 128
#define STEP 2

#define OK 0
#define ERR_IO 1
#define ERR 2
#define ERR_MEMORY 3
#define ERR_NO_CORRECT 4

typedef struct set set_t;
struct set
{
    int *mas;
    size_t n;
};


//Элемент двусвязного списка, в котором хранятся элементы ()U^\ и $
//каждому элементу соответсвует идущее после него множество или NULL
typedef struct element element_t;
struct element 
{
    char element;
    set_t *set;
    element_t *next;
    element_t *prev;
};

int char_to_int(const char *line, int *count);
void sort_set_t(set_t *set);
void print_set_t(set_t *set);
void free_set_t(set_t *set);
size_t count_nums(const char *line);
set_t *alloc_set_t(const size_t count);
set_t *create_set(const char *line);

char *my_strdup(const char *str);
size_t my_getline(char **line, size_t *n, FILE *stream);

int is_correct_in_square_brackets(const char *line);
int is_correct_line(const char *line);
void delete_spaces(char *str);

int is_in_set(set_t *set, const int n);
set_t *do_union(set_t *set1, set_t *set2);
set_t *do_intersection(set_t *set1, set_t *set2);
set_t *do_subtraction(set_t *set1, set_t *set2);

element_t *make_spisok_from_line(const char *line);
element_t *create_element(char element, set_t *set);
element_t *delete_element_t(element_t *element);
void free_element_t(element_t *element);
void free_spisok(element_t *spisok);

int check_line(char *line);
int find_brackets(element_t *head, element_t **open, element_t **close);
int make_action_in_brackets(element_t *open, element_t *close, char action);
int calculate(element_t *head, set_t **otvet);

int main(void)
{
    setbuf(stdout, NULL);
    int rc = OK;

    char *line = NULL;
    size_t line_size = 0;
    rc = my_getline(&line, &line_size, stdin);
    if (rc < 1)
    {
        printf("[error]");
        return OK;
    }
    if (rc == 1)
    {
        free(line);
        printf("[error]\n");
        return OK;
    }

    //проверим проавильность строки
    if (!check_line(line))
    {
        free(line);
        printf("[error]\n");
        return OK;
    }
    // создадим список с значищами элементами из строки
    element_t *spisok = NULL;
    spisok = make_spisok_from_line(line);
    if (!spisok)
    {
        printf("[error]\n");
        free(line);
        return OK;
    }
	
    set_t *otvet = NULL;
    rc = calculate(spisok, &otvet);
    if (rc == OK)
    {
        print_set_t(otvet);
    }
    else
    {
        printf("[error]\n");
    }
    free_spisok(spisok);
   
    free(line);
    return OK;
}

//io

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

    size_t   required_memory_size = 0;
    int readed_len = 0;
    size_t alloc_size = 0;//выделенный размер памяти - 1
    char buf[BUF_SIZE + 1];
    char *str = NULL;

    //Считываем в буфер часть строки, пока не конец файла
    while (fgets(buf, BUF_SIZE + 1, stream))
    {
        readed_len = strlen(buf);
        if (!readed_len)
            break;
        required_memory_size += readed_len;

        // если необходимо выделить еще память
        if (  required_memory_size > alloc_size)
        {
            char *buffer = realloc(str, required_memory_size + 1);
            if (!buffer)
            {
                if (str)
                    free(str);
                return -1;
            }
            str = buffer;
            alloc_size = required_memory_size;
        }

        strncpy(str + required_memory_size - readed_len, buf, readed_len * sizeof(char));
        //Если считалась вся строка, заканчиваем считывание.
        if (str[required_memory_size - 1] == '\n')
        {
            break;
        }
        if (feof(stream) || ferror(stream))
        {
            break;
        }
    }
    if (!required_memory_size)
    {
        if (str)
            free(str);
        return -1;
    }
    str[required_memory_size] = '\0';
    *line = str;
    *n = required_memory_size;
    return required_memory_size;
}



////string changes
/**
  Проверяет правильность заполнения квадратных скобок
 * @brief is_correct_in_square_brackets
 * @param line[in] - строка
 * @return Возвращает количество прочитанных символов, \
 0 если строка не подходит
 */
int is_correct_in_square_brackets(const char *line)
{
    int i = 1;
    int num_is_finded = 0; //флаг найденного числа
    while (line[i] != ']')
    {
        //printf("j [%c] ", line[i]);
        if (line[i] == '(' || line[i] == ')' || line[i] == '[')
            return 0;
        if (line[i] >= '0' && line[i] <= '9' && !num_is_finded)
        {
            //если найдено число читаем его до конца
            while(line[i] >= '0' && line[i] <= '9')
            {
                i++;
            }
            num_is_finded = 1;
        }
        //если только что найдено число, но мы снова встретили число
        //выходим из проверки
        else if (num_is_finded && line[i] >= '0' && line[i] <= '9')
        {
            return 0;
        }
        //если найдена запятая, можно искать следующее число
        if (line[i] == ',' && num_is_finded)
            num_is_finded = 0;
        else if (line[i] == ']')
            break;
        else if (line[i] != ' ') //если непредвиденный символ
            return 0;
        i++;
    }
    return i;
}

/**
  Проверяет находится ли символ в строке actions
 * @brief is_action
 * @param actions[in] - строка
 * @param symbol[in] - символ 
 * @return 1 - находится, 0 - не находится
 */
int is_action(const char *actions, char symbol)
{
    int i = 0;
    while (actions[i] != '\0')
    {
        if (actions[i] == symbol)
            return 1;
        i++;
    }
    return 0;
}

/**
  Проверяет правильность заполнения строки
 * @brief is_correct_line
 * @param line[in] - строка
 * @return 1 - верная строка, 0 - неверная
 */
int is_correct_line(const char *line)
{
    if (!line)
    {
        return 0;
    }

    size_t i = 0;
    int open_finded = 0;
    size_t action_count = 0;
    size_t set_count = 0;
    size_t count_open = 0;
    size_t count_close = 0;
    char actions[] = "^U\\";

    while (line[i] != '\0')
    {
        //printf("i[%c] ", line[i]);
        if (line[i] == '[' )
        {
            open_finded = 0;
            int readed_count = is_correct_in_square_brackets(line+i);
            if (readed_count < 1)
                return 0;
            i += readed_count;
            set_count++;
        }
        else if (line[i] == '(')
        {
            count_open++;
            open_finded = 1;
        }
        else if (line[i] == ')')
        {
            if (open_finded == 1)
                return 0;
            count_close++;
            if (count_close > count_open)
            {
                printf("here");
                return 0;
            }
        }
        else if (line[i] == ']')
            return 0;
        else if (is_action(actions, line[i]) && open_finded)
            return 0;
        else if (line[i] == 13 || line[i] == '\n')
            break;
        else if (line[i] != ' ' && !is_action(actions, line[i]))
            return 0;
        else if (is_action(actions, line[i]))
        {
            action_count++;
            if (action_count < set_count)
                return 0;
        }
        i++;
    }
    if (count_open != count_close)
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
    int rc = is_correct_line(line);
    if (rc)
    {
        delete_spaces(line);
        return rc;
    }
    return rc;
}

//set
/**
  Сортировка элементов множества по возрастанию
 * @brief sort_set_t
 * @param set [in] - множество
 */
void sort_set_t(set_t *set)
{
    for (int i = 0; i < set->n; i++)
    {
        for (int j = 0; j < set->n - 1; j++)
        {
            if (set->mas[j] > set->mas[j+1])
            {
                int t = set->mas[j];
                set->mas[j] = set->mas[j+1];
                set->mas[j+1] = t;
            }
        }
    }
}

/**
  Печать множества в отсортированном виде
 * @brief print_set_t
 * @param set[in] - множество
 */
void print_set_t(set_t *set)
{
    if (!set)
        return;
    if (set->n == 0)
    {
        printf("[]\n");
        return;
    }
    sort_set_t(set);
    printf("[");
    for (int i = 0; i < set->n - 1; i++)
    {
        printf("%d,", set->mas[i]);
    }
    printf("%d",set->mas[set->n-1]);
    printf("]\n");
}

/**
  Выделение памяти под множество размера count
 * @brief alloc_set_t
 * @param count [in] - размер множества
 * @return Возвращает новый элемент-множество
 */
set_t *alloc_set_t(const size_t count)
{
    set_t *new = (set_t *)malloc(sizeof(struct set));
    if (!new)
        return NULL;

    if (count == 0) //если пустое множество
    {
        new->mas = NULL;
        new->n = 0;
        return new;
    }

    int *buf = malloc(count * sizeof(int));
    if (!buf)
    {
        free(new);
        return NULL;
    }
    new->mas = buf;
    new->n = count;
    return new;
}

/**
  Освобождение памяти из-под множества
 * @brief free_set_t
 * @param set[in] - множество
 */
void free_set_t(set_t *set)
{
    if (!set)
        return;
    if (set->mas)
        free(set->mas);
    free(set);
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
 * @brief create_set
 * @param line[in] - строка
 * @return ВОзвращает указатель на множество
 */
set_t *create_set(const char *line)
{
    size_t count = count_nums(line);
    if (!line)
        return NULL;
    set_t *new = alloc_set_t(count); //count = 0 проверяется внутри функции
    if (!new)
        return NULL;

    size_t i = 0;
    count = 0;
    while(line[i] != ']')
    {
        if (line[i] >= '0' && line[i] <= '9')
        {
            int digit = atoi(line+i);
            while(line[i] >= '0' && line[i] <= '9')
            {
                i++;
            }
            i--;
            new->mas[count] = digit;
            count++;
        }
        i++;
    }
    return new;
}

/// actions

/**
  Проверка, находится ли элемент в множестве
 * @brief is_in_set
 * @param set[in] - множество
 * @param n[in] - элемент
 * @return 1 - находится, 0 - не находится
 */
int is_in_set(set_t *set, const int n)
{
    for(size_t i = 0; i < set->n; i++)
    {
        if (set->mas[i] == n)
            return 1;
    }
    return 0;
}

/**
  Объединение
 * @brief do_union
 * @param set1[in] - 1 множество
 * @param set2[in] - 2 множество
 * @return результат операции
 */
set_t *do_union(set_t *set1, set_t *set2)
{
    if (!set1 || !set2)
    {
        return NULL;
    }
    int count = 0;
    for (size_t i = 0 ; i < set2->n; i++)
    {
        if (!is_in_set(set1,set2->mas[i]))
            count++;
    }

    set_t *new = alloc_set_t(count + set1->n);
    if (!new)
        return NULL;

    count = 0;
    for (size_t i = 0 ; i < set1->n; i++)
    {
        new->mas[count] = set1->mas[i];
        count++;
    }
    for (size_t i = 0 ; i < set2->n; i++)
    {
        if (!is_in_set(set1,set2->mas[i]))
        {
            new->mas[count] = set2->mas[i];
            count++;
        }
    }

    return new;
}

/**
  Пересечение
 * @brief do_intersection
 * @param set1[in] - 1 множество
 * @param set2[in] - 2 множество
 * @return результат операции
 */
set_t *do_intersection(set_t *set1, set_t *set2)
{
    if (!set1 || !set2)
    {
        return NULL;
    }
    int count = 0;
    for (size_t i = 0 ; i < set2->n; i++)
    {
        if (is_in_set(set1,set2->mas[i]))
            count++;
    }

    set_t *new = alloc_set_t(count);
    if (!new)
        return NULL;

    count = 0;
    for (size_t i = 0 ; i < set2->n; i++)
    {
        if (is_in_set(set1,set2->mas[i]))
        {
            new->mas[count] = set2->mas[i];
            count++;
        }
    }
    return new;
}

/**
  Вычитание
 * @brief do_subtraction
 * @param set1[in] - 1 множество
 * @param set2[in] - 2 множество
 * @return результат операции
 */
set_t *do_subtraction(set_t *set1, set_t *set2)
{
    if (!set1 || !set2)
    {
        return NULL;
    }
    int count = 0;
    for (size_t i = 0 ; i < set1->n; i++)
    {
        if (!is_in_set(set2,set1->mas[i]))
            count++;
    }
    set_t *new = alloc_set_t(count);
    if (!new)
        return NULL;
    count = 0;

    for (size_t i = 0 ; i < set1->n; i++)
    {
        if (!is_in_set(set2,set1->mas[i]))
        {
            new->mas[count] = set1->mas[i];
            count++;
        }
    }

    return new;
}

/////element_T
/**
  Освобождение памяти из-под списка
 * @brief free_spisok
 * @param spisok[in] - голова списка
 */
void free_spisok(element_t *spisok)
{
    if (!spisok)
        return;

    while (spisok)
    {
        free_set_t(spisok->set);
        element_t *tmp = spisok;
        spisok = spisok->next;
        free(tmp);
    }
}

/**
  Выделение памяти под один элемент списка
 * @brief create_element
 * @param element[in] - элемент
 * @param set[in] - указатель на множество
 * @return указатель на созданный элемент
 */
element_t *create_element(char element, set_t *set)
{
    element_t *new = malloc(sizeof(element_t));
    if (new)
    {
        new->element = element;
        new->set = set;
        new->next = NULL;
        new->prev = NULL;
    }
    return new;
}

/**
  Освобождение одного элемента списка
 * @brief free_element_t
 * @param element[in] - элемент списка
 */
void free_element_t(element_t *element)
{
    if (!element)
        return;
    free(element);
}

/**
  Удаление элемента из списка
 * @brief delete_element_t
 * @param element [in] - удаляемый элемент
 * @return следующий элемент
 */
element_t *delete_element_t(element_t *element)
{
    if (element->next)
    {
        element->next->prev = element->prev;
    }
    element->prev->next = element->next;
    element_t *tmp = element;
    element = element->next;
    free_set_t(tmp->set);
    free_element_t(tmp);
    return element;
}


/**
  Создание списка из строки
 * @brief make_spisok_from_line
 * @param line [in] строка
 * @return указатель на началло списка
 */
element_t *make_spisok_from_line(const char *line)
{
    element_t *head = create_element('$',NULL);
    element_t *element = head;
    size_t i = 0;
    char actions[] = "^U\\()";
    size_t actions_len = strlen(actions);
    size_t line_len = strlen(line);
    if (line[0] == '[')
    {
        set_t *set = NULL;
        set = create_set(line);
        element->set = set;
        i++;
    }
    for (; i < line_len ; i++)
    {
        for (size_t j = 0; j < actions_len; j++)
        {
            if (line[i] == actions[j])
            {
                set_t *set = NULL;
                if (i+1 > line_len)
                    break;
                if (line[i+1] == '[')
                {
                     set = create_set(line+i+1);
                }
                element_t *new = create_element(line[i], set);
                if (!new)
                {
                    free_spisok(head);
                    return NULL;
                }
                element->next = new;
                new->prev = element;
                element = new;
                break;
            }
        }
    }
    return head;
}

///calculate

/**
  Поиск круглых скобочек
 * @brief find_brackets
 * @param head[in] - начало списка
 * @param open[out] - указатель на открывающую скобку
 * @param close[out] - указатель на закрывающую скобку
 * @return 1 - если найдены, 0 - не найдены
 */
int find_brackets(element_t *head, element_t **open, element_t **close)
{
    if (!head)
        return 0;

    while(head != NULL)
    {
        if (head->element == '(')
        {
            *open = head;
        }
        else if (head->element == ')')
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
int make_action_in_brackets(element_t *open, element_t *close, char action)
{
    element_t *element = open;
    while (element != close)
    {
        if (element->element == action)
        {
            set_t *buf;
            if (action == 'U')
            {
                buf = do_union(element->prev->set, element->set);
            }
            else if (action == '^')
            {
                buf = do_intersection(element->prev->set, element->set);
            }
            else
            {
                buf = do_subtraction(element->prev->set, element->set);
            }
            if (!buf)
                return ERR_MEMORY;
            free_set_t(element->prev->set);
            element->prev->set = buf;
            element = delete_element_t(element);
        }
        else
        {
            element = element->next;
        }
    }
    return OK;
}

/**
  Выполняет вычисление ответа
 * @brief calculate
 * @param head[in]
 * @param otvet[out]
 * @return
 */
int calculate(element_t *head, set_t **otvet)
{
    element_t *open = NULL;
    element_t *close = NULL;
    char actions[] = "^U\\";
    size_t actions_len = strlen(actions);
    int rc = OK;

    while(find_brackets(head, &open, &close))
    {
        for (size_t i = 0; i < actions_len; i++)
        {
            rc = make_action_in_brackets(open, close, actions[i]);
            if (rc != OK)
                 return rc;
        }
        open->prev->set = open->set;
        open->prev->next = close->next;
        if (close->next)
        {
             close->next->prev = open->prev;
        }
        free_element_t(open);
        free_element_t(close);
        open = NULL;
        close = NULL;
    }

    for (size_t i = 0; i < actions_len; i++)
    {
        make_action_in_brackets(head, NULL, actions[i]);
    }
    *otvet = head->set;
    //free_element_t(head);
    return OK;
}
///calculate
////////////////////////////////
