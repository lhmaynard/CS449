#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdll.h"

/* just a convenience */
void fatal( char * msg )
{
	printf("%s\n",msg);
	exit(EXIT_FAILURE);
}


/* ----------------------------------------------------------------------------
	initList:

*/
void initList(CDLL *list, int (*compare)(void*, void*), void (*print)(void*, int),
			  void (*freeData)(void *))
{
	/* Y O U R   C O D E    H E R E */
	//1) set the head pointer in the CDLL struct to NULL
	//2) assign each of the incoming function pointers into their respective fields in the CDLL struct
	
	list->head=NULL;
	list->compare= compare;
	list->print=print;
	list->freeData=freeData;

}


/* ----------------------------------------------------------------------------
*/
void insertAtTail(CDLL *list, void *data)
{
	/* Y O U R   C O D E    H E R E */
	CDLL_NODE *newNode = calloc(1, sizeof(CDLL_NODE));
	if(list->head==NULL){
		list->head=newNode;
		list->head->next=list->head;
		list->head->prev=list->head;
		list->head->data=data;
	}
	else{
		
		newNode->next= list->head;
		newNode->prev= list->head->prev;
		newNode->data=data;
		list->head->prev->next= newNode;
		list->head->prev= newNode;
	}

}



/* ----------------------------------------------------------------------------
	deleteNode:

	You have  passed in the pointer to the node to be deleted.
	No need to iterate or search. Of course a check for a NULL pointer passed in
	would not hurt.
	Delete the deadNode then return the pointer to it's successor (if CW) or
	if you are going CCW then return pointer to the deadNode's predecessor.
	If deadnode was the last node then return NULL since there is no succ or pred.
*/
CDLL_NODE * deleteNode(CDLL *list, CDLL_NODE *deadNode, int direction )
{
	/* Y O U R   C O D E    H E R E */
	int quit = 0;
	deadNode->prev->next=deadNode->next;
	deadNode->next->prev=deadNode->prev;
	
	CDLL_NODE *save;
	if(direction==CLOCKWISE){
		save = deadNode->next;

	}
	else{
		save = deadNode->prev;

	}
	if(deadNode==list->head) list->head=list->head->next;

	if(save==deadNode)quit = 1;
	
	list->freeData(deadNode->data);
//	memset(deadNode, 0, sizeof(CDLL_NODE));
	free(deadNode);
	
	if(quit){
		return NULL;
	}
	else{
		return save;
	}

}



/* ----------------------------------------------------------------------------
	printList:

	Observe my solution executable to see how it should look
	You are really just writing the loop and calling the printData on each node
*/

void printList( CDLL list, int direction, int mode )
{
	/* Y O U R   C O D E    H E R E */

	CDLL_NODE *iterator = list.head;
	do{
		list.print(iterator->data , mode);
		if(direction==CLOCKWISE) iterator=iterator->next;
		else iterator=iterator->prev;
	}while(iterator!=list.head);

}



/* ----------------------------------------------------------------------------
	searchList:

	Scan list until you find a node that contains the data value passed in.
	If found return that pointer - otherwise return NULL
*/
CDLL_NODE * searchList( CDLL list, void * target )
{
	/* Y O U R   C O D E    H E R E */

	CDLL_NODE *iterator = list.head;
	
	do{
		int temp = list.compare(target, iterator->data);
		if(temp==0) return iterator;
		iterator = iterator->next;
		
	}while(iterator!=list.head);

	return NULL;

}
