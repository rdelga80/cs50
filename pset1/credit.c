#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Number: ");
    long long cc_num = get_long_long();
    int first_num = 0;
    int sec_num = 0;

    int step = 1;
    long long tens = 1;

    do
    {
        if(step % 2 == 0)
        {
            if(((((cc_num % (tens * 10)) - (cc_num % tens)) / tens) * 2) > 9)
            {
                long long n = (((cc_num % (tens * 10)) - (cc_num % tens)) / tens) * 2;
                first_num += ((n % 100) - (n % 10)) / 10;
                first_num += n % 10;
            }
            else
            {
                first_num += (((cc_num % (tens * 10)) - (cc_num % tens)) / tens) * 2;
            }
        }
        else
        {
            if(step == 1)
            {
                sec_num += cc_num % (tens * 10);
            }
            else
            {
                sec_num += ((cc_num % (tens * 10)) - (cc_num % (tens))) / (tens);
            }
        }

        tens *= 10;
        step++;

    }
    while((cc_num % (tens)) != cc_num);

    if((first_num + sec_num) % 10 == 0)
    {
        switch(((cc_num % tens) - cc_num % (tens / 10)) / (tens / 10))
        {
            case 3:
                printf("AMEX\n");
                break;

            case 4:
                printf("VISA\n");
                break;

            case 5:
                printf("MASTERCARD\n");
                break;

            default:
                printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}