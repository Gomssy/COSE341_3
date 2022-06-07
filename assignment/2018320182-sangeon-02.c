#include <stdio.h>
#include <stdlib.h>

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz);

void print_data(int page, int* frames, int frame_sz, int faulted)
{
	printf("%d | ", page);

	for(int i = 0; i < frame_sz; i++)
	{
		if(frames[i] != -1)
			printf("%d ", frames[i]);
		else
			printf(". ");
	}
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
    int ref[] = {7, 0, 1, 2, 0 ,3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
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
    int victim = 0;
    int page_faults = 0;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    int* ref = (int*) malloc(sizeof(int) * frame_sz);
    for (i=0; i<frame_sz; i++)
    {
	    frames[i] = -1;
	    ref[i] = 0;
    }
    // TODO

    for(int i = 0; i < ref_arr_sz; i++)
    {
	    int found = 0;
	    for(j = 0; j < frame_sz; j++)
	    {
	    	if(frames[j] == ref_arr[i])
		{
			ref[j] = 1;
			found = 1;
			break;
		}
	    }
	    if(found)
	    {
	    	print_data(ref_arr[i], frames, frame_sz, 0);
		continue;
	    }

	    while(ref[victim])
	    {
	    	ref[victim] = 0;
		victim = (victim+1)%frame_sz;
	    }

	    frames[victim] = ref_arr[i];
	    ref[victim] = 1;
	    print_data(ref_arr[i], frames, frame_sz, 1);
	    page_faults++;
    }

    return page_faults;
}

