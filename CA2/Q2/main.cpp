#include <time.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <memory.h>
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

long int find_average_and_std_serial(float array[], size_t size)
{
	struct timeval start, end;
	
	gettimeofday(&start, NULL);

	// Average
	float sums[4] = {0}, average;
	for (size_t i = 0; i < size; i += 4)
		sums[0] += array[i];
	for (size_t i = 1; i < size; i += 4)
		sums[1] += array[i];
	for (size_t i = 2; i < size; i += 4)
		sums[2] += array[i];
	for (size_t i = 3; i < size; i += 4)
		sums[3] += array[i];
	average = ((sums[0] + sums[1]) + (sums[2] + sums[3])) / size;
	
	// Standard Deviation
	sums[0] = sums[1] = sums[2] = sums[3] = 0;
	float standard_deviation, difference;
	for (size_t i = 0; i < size; i += 4)
	{
		difference = array[i] - average;
		sums[0] += difference * difference;
	}
	for (size_t i = 1; i < size; i += 4)
	{
		difference = array[i] - average;
		sums[1] += difference * difference;
	}
	for (size_t i = 2; i < size; i += 4)
	{
		difference = array[i] - average;
		sums[2] += difference * difference;
	}
	for (size_t i = 3; i < size; i += 4)
	{
		difference = array[i] - average;
		sums[3] += difference * difference;
	}
	standard_deviation = sqrt(((sums[3] + sums[2]) + (sums[1] + sums[0])) / size);

	gettimeofday(&end, NULL);

	long int execution_time = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);	
	printf("Serial Method:\n");
	printf("\tAverage: %f\n", average);
	printf("\tStandard Deviation: %f\n", standard_deviation);
	printf("\tExecution time in microseconds: %ld\n\n", execution_time);

	return execution_time;
}

long int find_average_and_std_parallel(float array[], size_t size)
{
	struct timeval start, end;
	
	gettimeofday(&start, NULL);

	// Average
	float average;
	__m128 value;
	__m128 sum = _mm_set1_ps(0);
	for (size_t i = 0; i < size; i += 4)
	{
		value = _mm_loadu_ps(&array[i]);
		sum = _mm_add_ps(sum, value);
	}
	sum = _mm_hadd_ps(sum, sum);
	sum = _mm_hadd_ps(sum, sum);
	average = (_mm_cvtss_f32(sum)) / size;

	// Standard Deviation
	float standard_deviation;
	sum = _mm_set1_ps(0);
	__m128 average_register = _mm_set1_ps(average);
	for (size_t i = 0; i < size; i += 4)
	{
		value = _mm_loadu_ps(&array[i]);
		value = _mm_sub_ps(value, average_register);
		value = _mm_mul_ps(value, value);
		sum = _mm_add_ps(sum, value);
	}
	sum = _mm_hadd_ps(sum, sum);
	sum = _mm_hadd_ps(sum, sum);
	standard_deviation = (_mm_cvtss_f32(sum)) / size;
	standard_deviation = sqrt(standard_deviation);

	gettimeofday(&end, NULL);

	long int execution_time = (((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec) - (start.tv_usec);
	printf("Parallel Method:\n");
	printf("\tAverage: %f\n", average);
	printf("\tStandard Deviation: %f\n", standard_deviation);
	printf("\tExecution time in microseconds: %ld\n\n", execution_time);
	return execution_time;
}

int main (void)
{
	print_student_info();

	float* array = new float[ARRAY_SIZE];
	generate_random_array(array, ARRAY_SIZE);

	long int serial_execution_time = find_average_and_std_serial(array, ARRAY_SIZE);
	long int parallel_execution_time = find_average_and_std_parallel(array, ARRAY_SIZE);

	printf("Speed up: %.2f\n", ((float)serial_execution_time/(float)parallel_execution_time));

	delete array;
	return 0;
}
