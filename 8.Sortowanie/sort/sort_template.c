#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
    Food *fa = (Food*)d1;
    Food *fb = (Food*)d2;
    if(fa->valid_date.year < fb->valid_date.year){
        return -1;
    } else if(fa->valid_date.year > fb->valid_date.year) {
        return 1;
    } else if(fa->valid_date.year == fb->valid_date.year){
        if(fa->valid_date.month < fb->valid_date.month){
            return -1;
        } else if(fa->valid_date.month > fb->valid_date.month){
            return 1;
        } else if(fa->valid_date.month == fb->valid_date.month){
            if(fa->valid_date.day < fb->valid_date.day){
                return -1;
            } else if(fa->valid_date.day > fb->valid_date.day){
                return 1;
            } else {
                return 0;
            }
        }
    }
}

// compare foods
//int cmp(const void *a, const void *b) {
//    Food *fa = (Food*)a;
//    Food *fb = (Food*)b;
//    int compare_dates = cmp_date(a,b);
////    printf("%s %s\n",fa->name,fb->name);
//    if(strcmp(fa->name,fb->name) == 0 && fa->price == fb->price && compare_dates == 0){
////        printf("%s %s\n",fa->name,fb->name);
//        return 10;
//    } else if(strcmp(fa->name,fb->name) == 0 && fa->price < fb->price && compare_dates == 0) {
////        printf("%s %s\n", fa->name, fb->name);
//        return 1;
//    } else if(strcmp(fa->name,fb->name) == 0 && compare_dates == -1){
////        printf("%s %s\n", fa->name, fb->name);
//        return 2;
//    } else if(strcmp(fa->name,fb->name) < 0){
////        printf("%s %s\n",fa->name,fb->name);
//        return 3;
//    } else {
//        return -2;
//    }
//}
int cmp(const void *a, const void *b) {
    Food *fa = (Food*)a;
    Food *fb = (Food*)b;
    int compare_dates = cmp_date(a,b);
//    printf("%s %s\n",fa->name,fb->name);
    if(strcmp(fa->name,fb->name) == 0 && fa->price == fb->price && compare_dates == 0){
//        printf("%s %s\n",fa->name,fb->name);
        return 10;
    } else if(strcmp(fa->name,fb->name) == 0 && fa->price == fb->price && compare_dates == -1) {
//        printf("%s %s\n", fa->name, fb->name);
        return 3;
    } else if(strcmp(fa->name,fb->name) == 0 && fa->price < fb->price) {
        return 3;
    } else if(strcmp(fa->name,fb->name) < 0){
//        printf("%s %s\n",fa->name,fb->name);
        return 3;
    } else {
        return -2;
    }
}


int cmp_2(const void *a, const void *b){
    char *str = (char*)a;
    Food *fb = (Food*)b;
    if(strcmp(str,fb->name) == 0){
            return 10;
    } else {
        return 1;
    }
}

// bsearch returning address where to insert new element
//void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
int bsearch2 (const void *key, const void *base, size_t nmemb, int size, ComparFp compar, int *result) {
    Food *key_food = (Food*)key;
    Food *tab = (Food*)base;
    int i;
    for(i=size;i<nmemb;i++){
        int comparison = compar(key_food,&tab[i]);
        if(comparison == 10){
            *result = 1;
            return i;
        } else if(comparison == -2 || comparison == 1){
            continue;
        } else {
            *result = 0;
            return i;
        }
    }
    *result = 0;
    return i;
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
    int result = 1;
    int index = 0;
    while(result == 1){
        index = bsearch2(art,p,n,index,cmp_2,&result);
        if(result==1){
//            printf("%0.2f %0.2f %02d.%02d.%d\n",p[index].price,(double)p[index].amount,p[index].valid_date.day,p[index].valid_date.month,p[index].valid_date.year);
            printf("%0.2f %d %02d.%02d.%d\n",p[index].price,p[index].amount,p[index].valid_date.day,p[index].valid_date.month,p[index].valid_date.year);

        }
        index += 1;
    }
}

// add record to table if absent
//Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
int add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    int result;
    int index = bsearch2(new,tab,*np,0,compar,&result);
//    printf("%d %s %f %d %d\n",*np,new->name,new->price,index,result);
    if(result == 0){
        //Add element
        for(int i=*np-1;i>=index;i--){
            tab[i+1] = tab[i];
        }
        tab[index] = *new;
        return 1;
    } else {
        //Add amount to element
        tab[index].amount += new->amount;
        return 0;
    }
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int num = 0;
    for(int i=0;i<no;i++) {
        Food new_product;
        scanf("%s",&new_product.name);
        scanf("%f", &new_product.price);
        scanf("%d", &new_product.amount);
        scanf("%d.", &new_product.valid_date.day);
        scanf("%d.", &new_product.valid_date.month);
        scanf("%d", &new_product.valid_date.year);
//        printf("%s %0.2f %d %d %d %d\n",new_product.name,new_product.price, new_product.amount, new_product.valid_date.day,new_product.valid_date.month, new_product.valid_date.year);

        if(sorted == 1){
            num += add_record(tab,&num,cmp,&new_product);
        } else {
            num += add_record(tab,&num,cmp_date,&new_product);
        }
//        printf("===========\n");
//        for(int i=0;i<num;i++){
//            printf("%s %0.2f %d %d %d %d\n",tab[i].name,tab[i].price,tab[i].amount,tab[i].valid_date.day,tab[i].valid_date.month,tab[i].valid_date.year);
//        }
//        printf("===========\n");
    }
//    printf("===========\n");
//    for(int i=0;i<num;i++){
//        printf("%s %0.2f %d %d %d %d\n",tab[i].name,tab[i].price,tab[i].amount,tab[i].valid_date.day,tab[i].valid_date.month,tab[i].valid_date.year);
//    }
//    printf("%d\n",num);
    return num;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
    float sum = 0;

    struct tm t = {.tm_year = curr_date.year - 1900, .tm_mon = curr_date.month - 1, .tm_mday=curr_date.day};
    t.tm_mday += days;
    mktime(&t);

//    printf("%s\n",asctime(&t));

    Date curr;
    curr.day = t.tm_mday;
    curr.month = t.tm_mon + 1;
    curr.year = t.tm_year + 1900;

//    printf("%d.%d.%d\n",curr.day,curr.month,curr.year);

    Food x;
    x.valid_date = curr;
    for(int i=0;i<n;i++){
        if(cmp_date(&food_tab[i],&x) == 0){
            sum += food_tab[i].price * (float)food_tab[i].amount;
        }
    }
    return sum;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
} Parent;

typedef struct {
    Person person;
    int prev_index;
} Children;

typedef int (*ComparPerson)(Person, Person);

void insertionSort(Person arr[], int n, ComparPerson cmp_p)
{
    int i, j;
    Person key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && cmp_p(arr[j],key) == 1) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void insertionSort_2(Person arr[], int n, int a,ComparPerson cmp_p)
{
    int i, j;
    Person key;
    for (i = a+1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= a && cmp_p(arr[j],key) == 1) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
}

int cmp_person_date(Person a, Person b){
    Date da = a.born;
    Date db = b.born;
    if(da.year < db.year){
        return -1;
    } else if(da.year > db.year) {
        return 1;
    } else if(da.year == db.year){
        if(da.month < db.month){
            return -1;
        } else if(da.month > db.month){
            return 1;
        } else if(da.month == db.month){
            if(da.day < db.day){
                return -1;
            } else if(da.day > db.day){
                return 1;
            } else {
                return 0;
            }
        }
    }
}

int cmp_person_sex(Person a, Person b){
    if(a.bits.sex == b.bits.sex){
        return 0;
    }
    if(a.bits.sex == M && b.bits.sex == F){
        return -1;
    }
    if(a.bits.sex == F && b.bits.sex == M){
        return 1;
    }
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(Person *person_tab, int n) {
    printf("SIEMA\n");
//    char *name = p[2].name;
//	printf("%s\n", name);
}

int create_list(Person *person_tab, int n, int num) {
//    qsort(person_tab,n,sizeof(Person),cmp_person_date);
//    qsort(person_tab,n,sizeof(Person),cmp_person_sex);

    insertionSort_2(person_tab,n,0,cmp_person_date);
    insertionSort_2(person_tab,n,0,cmp_person_sex);

    Children curr[50];
    int curr_index = 0;

    for(int i=0;i<n;i++){
        char *curr_parent = person_tab[i].name;
//        printf("curr_parent %s\n",curr_parent);
        for(int j=i+1;j<n;j++){
            if(person_tab[j].parent == curr_parent){
                curr[curr_index].person = person_tab[j];
                curr[curr_index].prev_index = j;
                curr_index += 1;
            }
        }
        int swap_index = i+1;
        for(int k=0;k<curr_index;k++){
            int prev = curr[k].prev_index;
//            printf("%s <-> %s\n",curr[k].person.name,person_tab[swap_index].name);
            for(int l=n-1;l>=swap_index;l--){
                person_tab[l+1] = person_tab[l];
            }
            for(int m=prev+1;m<n;m++){
                person_tab[m] = person_tab[m+1];
            }
            person_tab[swap_index] = curr[k].person;
            swap_index += 1;
        }
        curr_index = 0;
    }

//    for(int i=0;i<n;i++){
//        printf("%d %s %d.%d.%d %s\n",i,person_tab[i].name,person_tab[i].born.day,person_tab[i].born.month,person_tab[i].born.year,person_tab[i].parent);
//    }

    for(int i=0;i<n;i++){
        if(person_tab[i].bits.in_line == no){
//            printf("%s\n",person_tab[i].name);
            for(int j=i;j<n-1;j++){
                person_tab[j] = person_tab[j+1];
            }
            n -= 1;
            i -= 1;
        }
    }

//    for(int i=0;i<n;i++){
//        printf("%d %s %d.%d.%d %s\n",i,person_tab[i].name,person_tab[i].born.day,person_tab[i].born.month,person_tab[i].born.year,person_tab[i].parent);
//    }

    printf("%s",person_tab[num-1].name);

    return 0;
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];

    freopen("2.txt","r",stdin); //plik na wejście

    fgets(buff, ART_MAX, stdin);

	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
            create_list(person_tab,no_persons,no);
//			print_person(person_tab,no);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
