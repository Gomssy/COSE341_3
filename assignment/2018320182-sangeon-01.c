#include <stdio.h>
#include <stdlib.h>

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

typedef struct Node {
	int data;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct Stack {
	int size;
	int max_size;
	Node* top;
}Stack;

Node* create_node()
{
	Node* ret = (Node*)malloc(sizeof(Node));
	ret->data = -1;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

Stack* create_stack(int max_size)
{
	Stack* ret = (Stack*)malloc(sizeof(Stack));
	ret->size = 0;
	ret->max_size = max_size;
	ret->top = NULL;
	return ret;
}

void push(Stack* stack, int data)
{
	Node* node = create_node();
	node->data = data;
	if(stack->top != NULL)
	{
		node->next = stack->top;
		stack->top->prev = node;
	}
	stack->top = node;
	stack->size++;
}

void delete_data(Stack* stack, Node* nodeDel)
{
	if(nodeDel->next != NULL)
		nodeDel->next->prev = nodeDel->prev;
	if(nodeDel->prev != NULL)
		nodeDel->prev->next = nodeDel->next;
	if(nodeDel == stack->top)
		stack->top = nodeDel->next;
	
	stack->size--;
	free(nodeDel);
}

int check_page_fault(Stack* stack, int page, int *rep)
{
	*rep = -1;
	Node* node = stack->top;

	//no page -> page fault, but no replacement
	if(node == NULL)
	{
		push(stack, page);
		return 1;
	}

	//find node and check page found
	while(node->next != NULL && node->data != page)
		node = node->next;

	if(node->data == page)
	{
		delete_data(stack, node);
		push(stack, page);
		return 0;
	}
	else
	{
		//page fault
		if(stack->size == stack->max_size)
		{
			*rep = node->data;
			delete_data(stack, node);
		}
		push(stack, page);
		return 1;
	}
}

void print_data(Stack* stack, int page, int* frames, int frame_sz, int faulted)
{
	printf("%d | ", page);

	for(int i = 0; i < frame_sz; i++)
		if(frames[i] != -1)
			printf("%d ", frames[i]);
		else
			printf(". ");

	if(faulted)
		printf("(fault)");

	printf("\n");
}

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
   if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

   // int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0 ,3, 2, 1, 2, 0, 1, 7, 0, 1};
    int page_faults = lru(ref, ref_arr_sz, frame_sz);
    printf("%d\n", page_faults);
    free(ref);

    return 0;
}

int* generate_ref_arr(size_t sz, size_t page_max) {
    int* ref_arr = (int*) malloc(sizeof(int) * sz);
    
    // TODO :: Generate Random Refernece String
    for(int i = 0; i < sz; i++)
	ref_arr[i] = rand()%page_max;

    return ref_arr;
}

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz) {
    int i, j;
    int page_faults = 0;
    Stack* lruStack = create_stack(frame_sz);
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++) frames[i] = -1;

    // TODO
    for(i = 0; i < ref_arr_sz; i++)
    {
	    int rep;
	    int faulted = check_page_fault(lruStack, ref_arr[i], &rep);
	    
	    if(faulted)
	    {
		    for(int j = 0; j < frame_sz; j++)
		    {
			    if(frames[j] == rep)
			    {
			    	frames[j] = ref_arr[i];
				break;
			    }
		    }
	    }
	    print_data(lruStack, ref_arr[i], frames, frame_sz, faulted);
	    page_faults += faulted;


    }
    return page_faults;
}

