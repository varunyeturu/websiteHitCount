/* MIN YEAR is taken from 1970 because the epoch time stamp beings from the year 1970*/
#define MIN_YEAR		1990
/* MAX YEAR is fixed until year 2018 which is the current year. The website hits cannot be beyond this year */
#define MAX_YEAR		2018
#define MAX_MONTHS_PER_YEAR	12
#define MAX_DAYS_PER_MONTH	31
#define MAX_WEBSITE_NAME_LEN	100
#define SET_BIT(bitmap, bitNumber)	(bitmap |= (1 << bitNumber))
#define GET_BIT(bitmap, bitNumber)	((bitmap >> bitNumber) & 1)
struct websiteList {
	int count;
	char websiteName[MAX_WEBSITE_NAME_LEN];
	struct websiteList *next;
};

struct dayData {
	int day;
	struct websiteList *head;
};

struct monthData {
	int month;
	struct dayData days[MAX_DAYS_PER_MONTH];
	int daysBitmap;
};

struct yearData {
	int year;
	struct monthData months[MAX_MONTHS_PER_YEAR];
	int monthsBitmap;
};

void mergeSort(struct websiteList **head);

