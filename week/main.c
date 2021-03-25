#include <stdio.h>
#include <stdlib.h>

int const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

int const *table_str[] = {"日", "一", "二", "三", "四", "五", "六"};

int RTC_Get_Week(int year, int month, int day)
{
	int temp2;
	int yearH, yearL;

	yearH = year / 100;
	yearL = year % 100;
	if(yearH > 19)
        yearL += 100;

	temp2 = yearL + yearL / 4;
	temp2 = temp2 % 7;
	temp2 = temp2 + day + table_week[month - 1];

	if(((yearL % 4) == 0) && (month < 3))
        temp2--;

	return (temp2 % 7);
}

int main()
{
    char buf[20] = {0};
    char *str = buf;

    int year = 0;
    int month = 0;
    int day = 0;

    printf("请输入日期\n");

    scanf("%s", buf);

    year = atoi(str);
    if(year < 100)
        year += 2000;

    while((*str <= '9') && (*str >= '0'))
    {
        str++;
    }
    str++;

    month = atoi(str);

    while((*str <= '9') && (*str >= '0'))
    {
        str++;
    }
    str++;

    day = atoi(str);

    printf("星期%s\n", table_str[RTC_Get_Week(year, month, day)]);

    return 0;
}
