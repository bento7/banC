#include <stdio.h>
#include <time.h>
// create struct Date
struct Date
{
    int day;
    int month;
    int year;
};

// create struct Transaction
struct Transaction
{
    struct Date date;
    int amount;
    char name_giver [32];
    char name_receiver [32];
};

// create struct Entete
struct Entete
{
    struct Date date;
    int solde;
};

// create struct Account
struct Account
{
    int id;
    char name [32];
};

void date(struct Date *d) {
    // `time_t` is an arithmetic time type
    time_t now;

    // Obtain current time
    time(&now);

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    d->day = local->tm_mday;            // get day of month (1 to 31)
    d->month = local->tm_mon + 1;      // get month of year (0 to 11)
    d->year = local->tm_year + 1900;   // get year since 1900
}

int main() {
    struct Date d;
    struct Date * d1 = &d;
    date(d1);
    printf("%i, %i, %i", d.year, d.day, d.month);
    return 0;
}