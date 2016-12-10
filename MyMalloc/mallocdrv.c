#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"
//include your code


//replace malloc here with the appropriate version of mymalloc
#define MALLOC my_worstfit_malloc
//replace free here with the appropriate version of myfree
#define FREE my_free

//You can adjust how many things are allocated
#define TIMES 100

//If you want to make times bigger than 100, remove the call to qsort and do something else.
//Then remove this check.
#if TIMES >= 1000
	#error "TIMES is too big, qsort will switch to mergesort which requires a temporary malloc()ed array"
#endif

//Do not modify below here
struct tree {
	int data;
	struct tree *left;
	struct tree *right;
};

void freetree(struct tree *root)
{
	if(root->left != NULL)
		freetree(root->left);
	if(root->right != NULL)
		freetree(root->right);
	printf("freeing %d\n", root->data);
	FREE(root);
	printf("FREED\n");
}

void randominsert(struct tree *head, struct tree *new)
{
	int way;
	struct tree *curr, *prev;
	prev = NULL;
	curr = head;

	while(curr != NULL)
	{
		prev = curr;
		way = rand()%2;
		if(way == 0)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	if(way == 0)
		prev->left = new;
	else
		prev->right = new;
}

void printtree(struct tree *head)
{
//	printf("NODE\n");
	struct tree *curr = head;
//	printf("HEAD: %d\n", curr);
	if(head == NULL)
		return;

	printtree(curr->left);
	printf("%d\n", curr->data);
	printtree(curr->right);
	printf("right\n");
}			 

void test1()
{
	int i;
 	
	struct tree *head = (struct tree *)MALLOC(sizeof(struct tree));
//	printf("SIZE of tree: %d\n", sizeof(struct tree));
	printf("\nfirst add: %d\n", head);
	head->data = 0;
	head->left = NULL;
	head->right = NULL;

	for(i=1;i<TIMES;i++)
	{
		struct tree *new = (struct tree *)MALLOC(sizeof(struct tree));
		printf("returned add: %d\n", new);
		new->data = i;
		new->left = NULL;
		new->right = NULL;
		randominsert(head, new);
		//printf("Loop");
	}

	printtree(head);
	printf("done printing\n");
	freetree(head);
}

int comp(const void *a, const void *b)
{
	return *((int *)a) - *((int *)b);
}

void test2()
{
	int *a;
	int i;
	printf("before first test2 malloc\n");
	a = (int *)MALLOC(TIMES * sizeof(int));
	printf("after first test2 malloc\n");
	for(i=0;i<TIMES;i++)
	{
		a[i] = rand()%TIMES + 1;
	}
	printf("before sort\n");
	qsort(a, TIMES, sizeof(int), comp);
	printf("done sort\n");
	for(i=0;i<TIMES;i++)
	{
		printf("%d\n", a[i]);
	}
	printf("donemalloc\n");
	FREE(a);
	
}

void test3(){

	printf("%d\n", sbrk(0));

	int *a;
	a = (int *)MALLOC(3);
	printf("A: %d\n", a);
	int *b;
	b = (int*)MALLOC(30);
	printf("B: %d\n", b);
	int *c;
	c = (int*)MALLOC(4);
	printf("C: %d\n", c);

	printf("sbrk before free stuff: %d\n", sbrk(0));
	list();
	FREE(b);
	printf("after free: %d\n", sbrk(0));
	list();
	int *d = (int*)MALLOC(12);
	list();
	printf("sbrk after: %d\n", sbrk(0));

	FREE(a);
	FREE(c);
	FREE(d);
	printf("final sbrk: %d\n", sbrk(0));

	printf("STARING ROUND 2\n");
	int *e = (int*)MALLOC(5);
	int *f = (int*)MALLOC(10);
	int *g = (int*)MALLOC(5);
	int *h = (int*)MALLOC(7);
	list();
	printf("freeing\n");

	FREE(f);
	FREE(g);
	list();
	FREE(e);
	FREE(h);

}


int main() 
{
	srand((unsigned int)time(NULL));
	printf("original val brk: %p\n", sbrk(0));
	test1();
	printf("brk after  test1: %p\n", sbrk(0));
	test2();
	printf("brk after  test2: %p\n", sbrk(0));
	test3();
	printf("brk after  test3: %p\n", sbrk(0));

	return 0;
}
