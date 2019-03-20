#include <iostream>
#include <stdio.h>

using namespace std;
#define START 18
#define END 80

// структура дата хранит дату и флаг смерти
typedef struct data data_t;
struct data
{
    int day;
    int month;
    int year;
    int death;
    data(int d, int m, int y, bool death):day(d), month(m), year(y), death(death){}
    data(){data(0,0,0,0);}
};

/**
  Компоратор для дат формата data_t
 * @brief cmp_data_t
 * @param l [in] - левая дата
 * @param r [in] - правая дата
 * @return
 */
int cmp_data_t(data_t l, data_t r)
{
    if (l.year > r.year)
        return 1;
    else if (l.year < r.year)
        return -1;
    if (l.month > r.month)
        return 1;
    if (l.month < r.month)
        return -1;
    if (l.day > r.month)
        return 1;
    if (l.day < r.day)
        return -1;
    if (l.death > r.death)
        return -1;
    if (l.death < r.death)
        return 1;
    return 0;
}

/**
  Переводит даты рождения и смерти к отрезкам от 18 до 80
 * @brief to_new_normal_points
 * @param birth [in,out] - дата рождения/исполнилось 18
 * @param death [in,out] - дата сметри/исполненилось 80
 * @return 1 - если человек умер до совершеннолетия, 0 - все OK
 */
bool to_new_normal_points(data_t &birth, data_t &death)
{
    if (death.year > birth.year + START)
    {
        birth.year += START;
    }
    else {
        return 1;
    }
    if (death.year > birth.year + END - START)
    {
        death.year = birth.year + END - START;
        death.day = birth.day;
        death.month = birth.month;
    }
    return 0;
}

/**
  Считывает массив дат из стандарного потока ввода
 * @brief read_all
 * @param len [out] - количество полученных дат
 * @return указатель на массив дат
 */
data_t *read_all(int &len)
{
    if (scanf("%d", &len) != 1 || len <= 0)
    {
        return NULL;
    }
    data_t *mas = new data_t[len * 2];
    int i = 0;
    for (int k = 0; k < len; k++)
    {
        //считаем дату рождения
        if (scanf("%d %d %d", &mas[i].day, &mas[i].month, &mas[i].year) != 3)
        {
            delete [] mas;
            return NULL;
        }
        mas[i].death = 0;
        i++;
        //дата смерти
        if (scanf("%d %d %d", &mas[i].day, &mas[i].month, &mas[i].year) != 3)
        {
            delete [] mas;
            return NULL;
        }
        mas[i].death = 1;
        if (to_new_normal_points(mas[i-1],mas[i])) // приводим к виду 18-80
        {
            i-=2;//если умер до совершеннолетия, не считаем его
        }
        i++;
    }
    len = i;
    return mas;
}

/**
  Слияние
 * @brief Merge
 * @param a[in] - первый массив
 * @param len_a[in] - длина первого массива
 * @param b[in] - второй массив
 * @param len_b[in] - длина второго массива
 * @param c[out] - массив для заполнения
 */
void Merge(data_t *a, int len_a, data_t *b, int len_b, data_t *c, int (*cmp)(data_t, data_t))
{
    int i = 0;
    int ai = 0;
    int bi = 0;
    while ( ai != len_a && bi != len_b)
    {
        if (cmp(a[ai],b[bi]) < 0)
        {
            c[i] = a[ai];
            ai++;
        }
        else
        {
            c[i] = b[bi];
            bi++;
        }
        i++;
    }
    while (bi != len_b)
    {
        c[i] = b[bi];
        bi++;
        i++;
    }
    while (ai != len_a)
    {
        c[i] = a[ai];
        ai++;
        i++;
    }

}
/**
  Моя Сортировка слиянием
 * @brief mySort
 * @param a[in,out] - массив
 * @param alen[in] - длина массива
 */
void mySort(data_t  *a, int alen, int (*cmp)(data_t, data_t))
{
    if( alen <= 1 )
    {
    return;
    }

    int firstLen = (alen) / 2;
    int secondLen = (alen) - firstLen;

    mySort( a, firstLen, cmp);
    mySort( a + firstLen, secondLen, cmp );

    data_t *c = new data_t[alen];

    Merge( a, firstLen, a + firstLen, secondLen, c,cmp);

    for (int i = 0; i < alen; i++)
    {
        a[i] = c[i];
    }
    delete[] c;
}

/**
  Процесс решения задачи
 * @brief process
 * @param a[in] - массив дат
 * @param alen[in] - длина массива
 * @return количество современников
 */
int process(data_t *a, int alen)
{
    int count_sovr = 0;
    int max_sovr = 0;
    mySort(a,alen, cmp_data_t);//сортируем

    for(int i = 0; i < alen; i++)//считаем
    {
        if (a[i].death == 1)
        {
            count_sovr--;
        }
        if (a[i].death == 0)
        {
            count_sovr++;
        }
        if (count_sovr > max_sovr)
        {
            max_sovr = count_sovr;
        }
    }
    return max_sovr;
}

int main()
{
    int len = 0;
    data_t *mas = read_all(len);
    if (!mas)
    {
        printf("[error]");
        return 0;
    }
    int max = process(mas, len);
    printf("%d", max);
    delete [] mas;
    return 0;
}
