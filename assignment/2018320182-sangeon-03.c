#include <stdio.h>
#include <stdlib.h>

int* generate_ref_arr(size_t sz, size_t page_max);
int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, int page_max);

void print_data(int page, int* frames, int frame_sz, int faulted, int ref_bit)
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
	
	int div = 1<<8;
	for(int i = 0; i < 8; i++)
	{
		printf("%d", ref_bit/div);
		ref_bit %= div;
		div /= 2;
	}
	printf("\n");
}

// Driver code
//  Usage: ./program {ref_arr_size} {page_max} {frame_size}
int main(int argc, char** argv) {
   if (argc < 4) return -1;
    
    int ref_arr_sz = atoi(argv[1]);
    int page_max = atoi(argv[2]);
    int frame_sz = atoi(argv[3]);

//    int* ref = generate_ref_arr(ref_arr_sz, page_max);
    int ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int page_faults = lru(ref, ref_arr_sz, frame_sz, page_max);
    printf("%d\n", page_faults);
    free(ref);

    return 0;
}

int* generate_ref_arr(size_t sz, size_t page_max) {
    int* ref_arr = (int*) malloc(sizeof(int) * sz);
    
    // TODO :: Generate Random Refernece String
    for(int i = 0; i < sz; i++)
	    ref_arr[i] = rand() % page_max;

    return ref_arr;
}

int lru(int* ref_arr, size_t ref_arr_sz, size_t frame_sz, int page_max) {
    int i, j;
    int page_faults = 0;
    
    // Initializing frames
    int* frames = (int*) malloc(sizeof(int) * frame_sz);
    int* ref = (int*) malloc(sizeof(int) * page_max);
    int* table = (int*) malloc(sizeof(int) * page_max);
    for (i=0; i<frame_sz; i++) frames[i] = -1;
    for (i = 0; i < page_max; i++)
    {
    	ref[i] = 0;
	table[i] = -1;
    }

    for(i = 0; i < ref_arr_sz; i++)
    {
    	int faulted = 0;
	if(table[ref_arr[i]] == -1)
	{
		faulted = 1;

		int min_ref = 1 << 9, victim = -1;
		for(j = 0; j < frame_sz; j++)
		{
			if(frames[j] == -1)
			{
				victim = j;
				break;
			}
			if(ref[frames[j]] < min_ref)
			{
				min_ref = ref[frames[j]];
				victim = j;
			}
		}
		
		table[frames[victim]] = -1;
		table[ref_arr[i]] = victim;
		frames[victim] = ref_arr[i];
	}
	for(j = 0; j < page_max; j++)
	{
		ref[j] = ref[j] >> 1;	
		if(j == ref_arr[i])
			ref[j] = ref[j] | 1<<8;

	}

	print_data(ref_arr[i], frames, frame_sz, faulted, ref[1]);
	page_faults += faulted;

    }

    return page_faults;
}

