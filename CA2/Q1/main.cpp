#include <time.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <sys/time.h>
#include <x86intrin.h>

#define	ARRAY_SIZE 4194304

void print_student_info() {
    printf("******************************************************\n");
    printf("Group members:\n");
    printf("                Sajjad Alizadeh:	810197547\n");
    printf("                Sarina Hamedani:	810197606\n");
    printf("******************************************************\n\n");
}

float random_float(float min, float max)
{
	srand(time(NULL));
	float random_number = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random_number * range) + min;
}

void generate_random_array(float array[], size_t size)
{
	for (size_t i = 0; i < size; i++)
		array[i] = random_float(0, 1000000);
}

long int find_min_serial(float array[], size_t size)
{
	struct timeval start, end;
	float min_elements[4];
	int min_indexes[4];
	float min_element;
	int min_index;

	gettimeofday(&start, NULL);

	min_elements[0] = array[0];
	min_indexes[0] = 0;

	for (size_t i = 0; i < size; i += 4)
	{
		if (array[i] < min_elements[0])
		{
			min_elements[0] = array[i];
			min_indexes[0] = i;
		}
	}

	min_elements[1] = array[1];
	min_indexes[1] = 1;

	for (size_t i = 1; i < size; i += 4)
	{
		if (array[i] < min_elements[1])
		{
			min_elements[1] = array[i];
			min_indexes[1] = i;
		}
	}

	min_elements[2] = array[2];
	min_indexes[2] = 2;

	for (size_t i = 2; i < size; i += 4)
	{
		if (array[i] < min_elements[2])
		{
			min_elements[2] = array[i];
			min_indexes[2] = i;
		}
	}

	min_elements[3] = array[3];
	min_indexes[3] = 3;

	for (size_t i = 3; i < size; i += 4)
	{
		if (array[i] < min_elements[3])
		{
			min_elements[3] = array[i];
			min_indexes[3] = i;
		}
	}

	min_element = min_elements[0];
	min_index = min_indexes[0];
	for (size_t i = 1; i < 4; i++)
	{
		if (min_elements[i] < min_element)
		{
			min_element = min_elements[i];
			min_index = min_indexes[i];
		}
	}

	gettimeofday(&end, NULL);

	long int execution_time = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);	
	printf("Serial Method:\n");
	printf("\tMin value: %f\n", min_element);
	printf("\tMin index: %d\n", min_index);
	printf("\tExecution time in microseconds: %ld\n\n", execution_time);

	return execution_time;
}

long int find_min_parallel(float array[], size_t size)
{
	struct timeval start, end;
	float array_values[4], array_indexes[4];
	
	gettimeofday(&start, NULL);

	__m128 min_elements = _mm_set1_ps(FLT_MAX);
	__m128 increment = _mm_set1_ps(4);
	__m128 indexes = _mm_setr_ps(0, 1, 2, 3);
	__m128 min_indexes = _mm_setr_ps(0, 1, 2, 3);
	__m128 value, lt;

	for (size_t i = 0; i < size; i += 4)
	{
		value = _mm_loadu_ps(&array[i]);
		lt = _mm_cmpgt_ps(min_elements, value);
		min_indexes = _mm_blendv_ps(min_indexes, indexes, lt);
		min_elements = _mm_min_ps(min_elements, value);
		indexes = _mm_add_ps(indexes, increment);
	}

    _mm_storeu_ps(array_values, min_elements);
    _mm_storeu_ps(array_indexes, min_indexes);

    float min_element = array_values[0];
    int min_index = (int) array_indexes[0];

	for (size_t i = 1; i < 4; i++)
	{
		if (array_values[i] < min_element)
		{
			min_element = array_values[i];
			min_index = (int) array_indexes[i];
		}
	}

	gettimeofday(&end, NULL);

	long int execution_time = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);
	printf("Parallel Method:\n");
	printf("\tMin value: %f\n", min_element);
	printf("\tMin index: %d\n", min_index);
	printf("\tExecution time in microseconds: %ld\n\n", execution_time);
	return execution_time;
}

int main (void)
{
	print_student_info();

	float* array = new float[ARRAY_SIZE];
	generate_random_array(array, ARRAY_SIZE);

	long int serial_execution_time = find_min_serial(array, ARRAY_SIZE);
	long int parallel_execution_time = find_min_parallel(array, ARRAY_SIZE);

	printf("Speed up: %.2f\n", ((float)serial_execution_time/(float)parallel_execution_time));

	delete array;
	return 0;
}
