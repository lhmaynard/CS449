#include <errno.h>
#include <stdlib.h>

struct node *first_node;
struct node *last_node;

struct node{
	int size;
	int used;
	struct node *next_node;
	struct node *prev_node;
};

int count = 0;

void *my_worstfit_malloc(int size){
//	printf("Size of node: %d\n", sizeof(struct node));

	struct node *curr_node;
	struct node *worst_node;
	struct node new_node;

	struct node *point;
	void *return_add;

//	printf("first_node before check: %d\n", first_node);	

	if(count==0){
//		printf("FIRST\n");
	

		point = (struct node *) sbrk(0);
//		printf("SBRK1: %d\n", sbrk(0));
		sbrk(size + sizeof(struct node));
//		printf("SBRK2: %d\n", sbrk(0)); 
		first_node = point;
		new_node.size = size;
		new_node.used = 1;
		new_node.next_node = NULL;
		new_node.prev_node = NULL;

		last_node = first_node;

		*first_node = new_node;

		count++;

		return_add = (void *)((void *)point + sizeof(struct node));

		return return_add;
	}

	else{
		int worst_size = 0;
		curr_node = first_node;
		int spot_found = 0;
		int no_split = 0;
		
//		printf("finding spot for %d\n", size);
		while(curr_node != NULL){
			
//			printf("size: %d used: %d next: %d\n", curr_node->size, curr_node->used, curr_node->next_node);		
			if(curr_node->used != 1 && curr_node->size >= (size+sizeof(struct node))){
//				printf("found spot for %d before size check\n", size);
				spot_found = 1;	

				if(curr_node->size > worst_size){
				
					worst_size = curr_node->size;
					worst_node = curr_node;
				}
				
			}

			curr_node = curr_node->next_node;
		}

		if(spot_found == 1){

			worst_node->used = 1;
			return_add = (void *)((void *)worst_node + sizeof(struct node));
			

			point = worst_node + size + sizeof(struct node);
//			printf("%d - %d", worst_node->size, sizeof(struct node));
			new_node.size = worst_node->size - sizeof(struct node) - size;
			new_node.used = 0;
			new_node.prev_node = worst_node;
			new_node.next_node = worst_node->next_node;

			*point = new_node;
//			printf("size; %d used: %d next: %d prev: %d", point->size, point->used, point->next_node, point->prev_node);
			count++;

			worst_node->next_node = point;
			point->next_node->prev_node = point; 
			
			worst_node->size = size;
			return return_add;
		}

		else{

			point = (struct node *) sbrk(0);
			sbrk(size + sizeof(struct node));
//			printf("FINAL BRK: %d\n", sbrk(0));
			struct node *temp;
	
			struct node *it;
			it = first_node;
			while(it->next_node != NULL) it = it->next_node;
			last_node = it;			

			temp = last_node;
			last_node = point;
			new_node.size = size;
			new_node.used = 1;
			new_node.next_node = NULL;
			new_node.prev_node = temp;
			temp->next_node = point;

			*point = new_node;
			count++;

			return_add = (void *)((void *)point + sizeof(struct node));
		
			return return_add;
		

		}

	}
}

void list(){

	struct node * walk = first_node;
	printf("PRINTING LIST: \n");
	while(walk != NULL){
		
		printf("size: %d used: %d next: %d\n", walk->size, walk->used, walk->next_node);		
		walk = walk->next_node;
}}


void my_free(void *ptr){

	struct node *curr_node;

	curr_node = ptr - sizeof(struct node);
	curr_node->used = 0;
	count--;

	struct node *forward;
	struct node *backward;
	forward = curr_node->next_node;
	backward = curr_node->prev_node;

	if(backward != NULL){
		if(backward->used != 1){

			backward->size = backward->size + curr_node->size + sizeof(struct node);
			backward->next_node = forward;
			if(forward != NULL) forward->prev_node = backward;
			curr_node = backward;
			curr_node->used = 0;			

		}
	

	}


	if(forward != NULL){
		if(forward->used == 0){

			curr_node->next_node = forward->next_node;
			forward->next_node->prev_node = curr_node;
			curr_node->size = curr_node->size + forward->size + sizeof(struct node);
			

		}
	
	}

	else{

       	if(curr_node->prev_node != NULL){
		curr_node->prev_node->next_node = NULL;
		last_node = curr_node->prev_node;
	}
	
	sbrk(-1 * (curr_node->size + sizeof(struct node)));

	}


}
