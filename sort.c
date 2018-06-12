#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "hitCount.h"

static void split(struct websiteList *src, struct websiteList **frontPtr, struct websiteList **backPtr)
{
	/* Use SLOW FAST pointer mechanism to traverse */
	struct websiteList *slow, *fast;
	slow = src;
	fast = src->next;

	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}
	*frontPtr = src;
	*backPtr = slow->next;
	slow->next = NULL;
}

static struct websiteList *mergedList(struct websiteList *list1, struct websiteList *list2)
{
	struct websiteList *result = NULL;

	if (list1 == NULL)
		return list2;
	else if (list2 == NULL)
		return list1;

	if (list1->count >= list2->count) {
		result = list1;
		result->next = mergedList(list1->next, list2);
	} else {
		result = list2;
		result->next = mergedList(list1, list2->next);
	}
	return result;
}

void mergeSort(struct websiteList **head)
{
	struct websiteList *temp = *head;
	struct websiteList *x;
	struct websiteList *y;

	if (temp == NULL || temp->next == NULL)
		return;

	split(temp, &x, &y);
	mergeSort(&x);
	mergeSort(&y);

	*head = mergedList(x, y);
}


