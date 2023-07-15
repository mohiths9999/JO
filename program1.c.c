#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_ITEMS 30
#define MAX_NAME_LENGTH 64

struct market_value{
    char name[MAX_NAME_LENGTH];
    int market_price;
};

struct sale_price{
    char name[MAX_NAME_LENGTH];
    int price;
};

struct subset{
  char name[MAX_NAME_LENGTH];
};

int market_size;
int ncards;
int money;
int profit;
struct subset s1[10];
int elements;

void read_market_prices(const char *filename,struct market_value m[]) {
int i;
FILE *fp = fopen(filename, "r");
if (fp == NULL) {
perror(filename);
exit(1);
}
fscanf(fp, "%d", &market_size);
for (i = 0; i < market_size; i++) {
fscanf(fp, "%s %d", &m[i].name, &m[i].market_price);
}
fclose(fp);
}

void read_prices(struct sale_price p1[], long int *pos,FILE *fp2) {
    int i;
    if (fp2 == NULL) {
        exit(1);
    }
    fseek(fp2, *pos, SEEK_SET);
    fscanf(fp2, "%d", &ncards);
    fscanf(fp2, "%d", &money);
    for (i = 0; i < ncards; i++) {
        fscanf(fp2, "%s %d", p1[i].name, &p1[i].price);
    }
    *pos = ftell(fp2);
}

struct profit{
    char name[MAX_NAME_LENGTH];
    int price;
    int profit;
};

void profit_sheet(struct market_value m[],struct sale_price p[],int ncards,int market, struct profit pro[]){
    int i,j;
    for(i=0;i<market;i++){
        for(j=0;j<ncards;j++){
            if(strcmp(m[i].name,p[j].name)==0){
                strcpy(pro[j].name,p[j].name);
                pro[j].price=p[j].price;
                pro[j].profit=m[i].market_price-p[j].price;
            }
        }
    }
}


void computeMaxProfit(struct profit p[], int n, int maxWeight) {
  int maxProfit = 0;
  int weight = 0;
  char a[20][20];
  int i, j, k;
  elements= 0;

  for(i=0;i<n;i++){
    maxProfit = maxProfit+p[i].profit;
    weight = weight + p[i].price;
  }
  if(weight<=maxWeight){
    for(i=0; i<n; i++){
      strcpy(s1[i].name,p[i].name);
      elements = elements+1;
    }
    profit = maxProfit;
    return;
  }

  int numSubsets = pow(2, n); 
  int *subsetIndices = (int*) malloc(n * sizeof(int)); 
  int subsetProfit, subsetWeight;
  int maxSubsetProfit = 0;
  int maxSubsetIndex = -1;
  for(i=1; i<numSubsets; i++){
    subsetProfit = 0;
    subsetWeight = 0;
    int index = 0;
    for(j=0; j<n; j++){
      if(i & (1<<j)){
        subsetProfit += p[j].profit;
        subsetWeight += p[j].price;
        subsetIndices[index] = j;
        index++;
      }
    }
    if(subsetWeight <= maxWeight && subsetProfit > maxSubsetProfit){
      maxSubsetProfit = subsetProfit;
      maxSubsetIndex = i;
    }
  }
  profit = maxSubsetProfit;
  if(maxSubsetIndex != -1){
    for(i=0; i<n; i++){
      if(maxSubsetIndex & (1<<i)){
        strcpy(s1[i].name,p[i].name);
        elements = elements+1;
      }
    }
  }
  free(subsetIndices);
}



int main(int argc, char *argv[]) {
  int i;
  clock_t start, end;
  double time_taken;
  if (argc != 5 || strcmp(argv[1], "-m") || strcmp(argv[3], "-p")) {
    fprintf(stderr, "Usage: %s -m <market-price-file> -p <price-list-file>\n",
    argv[0]);
    return 1;
  }
  FILE *fp1 = fopen("output.txt","w");
  long int pos = 0;
  char line[100];
  struct market_value m[10];
  struct sale_price p1[10];
  struct profit pro1[10];
  FILE *fp2 = fopen(argv[4], "r");
  read_market_prices(argv[2],m);
  while (fgets(line, sizeof(line), fp2) != NULL){
    read_prices(p1,&pos,fp2);
    start = clock();
    profit_sheet(m,p1,ncards,market_size,pro1);
    computeMaxProfit(pro1,ncards,money);
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(fp1,"%d\t%d\t%d\t%lf\n",ncards,profit,elements,time_taken);
    for(int j=0;j<elements;j++){
      fputs(s1[j].name,fp1);
      fputs("\n",fp1);
    }
  }
  fclose(fp2);
  fclose(fp1);
}
