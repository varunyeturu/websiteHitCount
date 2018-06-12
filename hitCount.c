#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "hitCount.h"

#define YEARS_BITMAP_BYTES 2
struct yearData years[MAX_YEAR-MIN_YEAR+1];
int yearsBitmap[YEARS_BITMAP_BYTES];

void displayWebsiteList(struct websiteList **head)
{
	struct websiteList *temp;

	mergeSort(head);
	temp = *head;
	while(temp != NULL) {
		printf("%s\t%d\n", temp->websiteName, temp->count);
		temp = temp->next;	
	}
}

void storeWebsiteData(struct websiteList **head, char *websiteName)
{
	struct websiteList *temp = *head;
	struct websiteList *prev = temp;
	struct websiteList *node;

	if (temp == NULL) {
		goto alloc_new_node;
	}
	while (temp != NULL) {
		if (!strcmp(temp->websiteName, websiteName)) {
			temp->count++;
			return;
		}
		prev = temp;
		temp = temp->next;
	}
alloc_new_node:
	node = malloc(sizeof(struct websiteList));
	if (node == NULL) {
		printf("Cannot allocate memory for new NODE");
		return;
	}
	node->count = 1;
	strcpy(node->websiteName, websiteName);
	node->next = NULL;
	if (prev)
		prev->next = node;
	else
		*head = node;

}

void freeWebsiteList(struct websiteList **head)
{
	struct websiteList *current = *head;
	struct websiteList *next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}

void setYearsBitmap(int *bitmap, int idx)
{
	if (idx <= ((sizeof(int) * 8) - 1))
		SET_BIT(bitmap[0], idx);
	else {
		idx = idx%(sizeof(int) * 8);
		SET_BIT(bitmap[1], idx);
	}
}

void storeDate(char *dateStr, char *websiteName)
{
	int dateInt = atoi(dateStr);
	int year, month, day;
	int yearIdx, monthIdx, dayIdx;
	struct tm *GMTTime;

	GMTTime = gmtime((time_t *) &dateInt);

	year = GMTTime->tm_year+1900;
	yearIdx = year-MIN_YEAR;
	monthIdx = GMTTime->tm_mon;
	dayIdx = GMTTime->tm_mday-1;

	/* Update the bitmask for Year, Month and Day
	 * This will help later to traverse the stored
	 * data selectively and print it on the console
	 */
	setYearsBitmap(yearsBitmap, yearIdx);
	years[yearIdx].year = year;
	SET_BIT(years[yearIdx].monthsBitmap, monthIdx);
	years[yearIdx].months[monthIdx].month = monthIdx+1;
	SET_BIT(years[yearIdx].months[monthIdx].daysBitmap, dayIdx);
	years[yearIdx].months[monthIdx].days[dayIdx].day = dayIdx+1;
	storeWebsiteData(&years[yearIdx].months[monthIdx].days[dayIdx].head,
			 websiteName);
}

int readAndStoreData(char *inputFile)
{
	FILE *fp;
	char str[100], date[20], websiteName[80];
	char *token;
	
	fp = fopen(inputFile, "r");
	while (fscanf(fp, "%s", str) == 1) {
		token = strtok(str, "|");
		strcpy(date, token);
		token = strtok(NULL, "|");
		strcpy(websiteName, token);
		storeDate(date, websiteName);
	}
	return 0;
}

void displayHits()
{
	int year, yearIdx;
	int month, monthIdx;
	int day, dayIdx;
	int bitNum = 0,i;
	
	for (i = 0; i < YEARS_BITMAP_BYTES; i++) {
	/* Traverse the bitmaps for YEAR, MONTH and DAY to display all the hits */
	year = yearsBitmap[i];
	while (year) {
		yearIdx = __builtin_ffs(year) - 1;
		if (yearIdx) {
			month = years[yearIdx].monthsBitmap;
			while (month) {
				monthIdx = __builtin_ffs(month)-1;
				if (monthIdx) {
					day = years[yearIdx].months[monthIdx].daysBitmap;
					while (day) {
						dayIdx = __builtin_ffs(day)-1;
						if (dayIdx) {
							printf("%02d/%02d/%d GMT\n",
								years[yearIdx].months[monthIdx].month,
								years[yearIdx].months[monthIdx].days[dayIdx].day,
								years[yearIdx].year);
							displayWebsiteList(&years[yearIdx].months[monthIdx].days[dayIdx].head);
							freeWebsiteList(&years[yearIdx].months[monthIdx].days[dayIdx].head);
						}
						day = day & (day-1);
					}
				}
				month = month & (month-1);
			}
		}
		year = year & (year-1);
	}
	}
	printf("\n");
}

void main(int argc, char *argv[])
{

	FILE *fp;
	char *inputFile;
	char ch;
	
	if (argc < 2) {
		printf("Missing input parameter to the program\n");
		return;
	}
	readAndStoreData(argv[1]);
	displayHits();

}
