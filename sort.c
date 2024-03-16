#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Ryan Schlosbon - Lab Assignment 8 - 03/15/24 - Merge Sort Program

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
  extraMemoryAllocated += sz;
  size_t* ret = malloc(sizeof(size_t) + sz);
  *ret = sz;
  printf("Extra memory allocated, size: %ld\n", sz);
  return &ret[1];
}

void DeAlloc(void* ptr)
{
  size_t* pSz = (size_t*)ptr - 1;
  extraMemoryAllocated -= *pSz;
  printf("Extra memory deallocated, size: %ld\n", *pSz);
  free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
  return ((size_t*)ptr)[-1];
}

//mergeSort function takes a data array, as well as left and right index indicators

void mergeSort(int pData[], int l, int r) {  
  if (r <= l) {
    return;
  }

  int m = (l + r) / 2; //Finds the midpoint between the two ranges
  int leftsize = m - l + 1; // Finds the size of the left side of the array
  int rightsize = r - m; // Finds the size of the right side of the array

  mergeSort(pData, l, m); //mergeSort calls itself recursively to sort the left side of the array
  mergeSort(pData, (m + 1), r); // Sorts the right side of the array - m is offset by one as the previous function call went up to the midpoint. We want to exclude it.)

if(l<r){

  int* left = (int*)Alloc(sizeof(int) * leftsize);
  int* right = (int*)Alloc(sizeof(int) * rightsize);


  // Copy the data from pData into temp arrays left[] and right[]
  for (int i = 0; i < leftsize; i++) {
    left[i] = pData[l + i];
  }

  for (int j = 0; j < rightsize; j++) {
    right[j] = pData[m + 1 + j];
  }

  //Variables that hold indexes
  int i = 0; //Index of first subarray
  int j = 0; //Index of second subarray
  int k = l; //Index of merged subarray

  while (i < leftsize && j < rightsize) {
    if (left[i] <= right[j]) { //if left array item at index i is less than or equal to right side array item at index j, add it to main array.
      pData[k] = left[i]; // Copies the item from the left array to the main array
      i++;
    } else { //add the right side item as it is less than the left side item
      pData[k] = right[j]; // Copies the item from the right array to the main array
      j++;
    }
    k++;
  }

  // Copy any remaining elements of left[]
  while (i < leftsize) {
    pData[k] = left[i];
    i++;
    k++;
  }

  // Copy any remaining elements of right[]
  while (j < rightsize) {
    pData[k] = right[j];
    j++;
    k++;
  }

  DeAlloc(left); // Frees the left array
  DeAlloc(right); // Freez the right array
	}
}


// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
  {
  FILE* inFile = fopen(inputFileName,"r");
  int dataSz = 0;
  int i, n, *data;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile,"%d\n",&dataSz);
    *ppData = (int *)malloc(sizeof(int) * dataSz);
    // Implement parse data block
    if (*ppData == NULL)
    {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i=0;i<dataSz;++i)
    {
      fscanf(inFile, "%d ",&n);
      data = *ppData + i;
      *data = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
  int i, sz = dataSz - 100;
  printf("\tData:\n\t");
  for (i=0;i<100;++i)
  {
    printf("%d ",pData[i]);
  }
  printf("\n\t");

  for (i=sz;i<dataSz;++i)
  {
    printf("%d ",pData[i]);
  }
  printf("\n\n");
}

int main(void)
{
  clock_t start, end;
  int i;
    double cpu_time_used;
  char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

  for (i=0;i<4;++i)
  {
    int *pDataSrc, *pDataCopy;
    int dataSz = parseData(fileNames[i], &pDataSrc);

    if (dataSz <= 0)
      continue;

    pDataCopy = (int *)malloc(sizeof(int)*dataSz);

    printf("---------------------------\n");
    printf("Dataset Size : %d\n",dataSz);
    printf("---------------------------\n");

    printf("Merge Sort:\n");
    memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
    extraMemoryAllocated = 0;
    start = clock();
    mergeSort(pDataCopy, 0, dataSz - 1);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
    printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
    printArray(pDataCopy, dataSz);

    free(pDataCopy);
    free(pDataSrc);
  }
}