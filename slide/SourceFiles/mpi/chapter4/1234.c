#include<stdio.h>
#include<math.h>

int check(int* num, int* current, int length){
  for (int i = 0; i < length; i++){
    if (num[i] > current[i]){
      return 0;
    }
  }
  return 1;
}

int balancedString(char * s){
    int length = 0;
    int j = 0;
    char allLetter[4] = "QWER";
    int count[4] = {0};

    for (length = 0; s[length]; length++){
      if (s[length] == allLetter[0]){
        count[0]++;
      }
      else if (s[length] == allLetter[1]){
        count[1]++;
      }
      else if (s[length] == allLetter[2]){
        count[2]++;
      }
      else if (s[length] == allLetter[3]){
        count[3]++;
      }
    }
    int overNum = 0;

    for (j = 0; j < 4; j++){
        printf("count %d : %d\n", j, count[j]);
      if (count[j] > length/4){
        overNum++;
        count[j] -= length/4;
      }else{
        count[j] = 0;
      }
    }
    if (overNum == 0){
        return 0;
    }

    char* letter = (char*)malloc(overNum * sizeof(char));
    int* num = (int*)malloc(overNum * sizeof(int));
    int* current = (int*)malloc(overNum*sizeof(int));
    int pos = 0;
    int total = 0;

    for (j = 0; j < 4; j++){

        if (count[j] > 0){
            letter[pos] = allLetter[j];
            num[pos] = count[j];
            current[pos] = 0;
            total += count[j];
            pos++;
        }
    }


    int start = 0;
    int end = total-1;
    // length = length
    for (j = start; j <= end; j++){
      for (int a = 0; a < overNum; a++){
        if (s[j] == letter[a]){
          current[a]++;
        }
      }
    }
    for (j = 0; j < overNum; j++){
      printf("num %d = %d\n", j, num[j]);
    }
    for (j = 0; j < overNum; j++){
      printf("current %d = %d\n", j, current[j]);
    }

    int mini = 100000;
    int temp;

    while(end < length && end >= start){
        // printf("now mini = %d\n", mini);
      if (check(num, current, overNum)){
          // printf("start = %d, end = %d is true\n", start, end);
        mini = fmin(end - start + 1, mini);
        for (j = 0; j < overNum; j++){
          if (s[start] == letter[j]){
            current[j]--;
          }
        }
        start++;
      }else{
        end++;
        for (j = 0; j < overNum; j++){
          if (s[end] == letter[j]){
            current[j]++;
          }
        }
      }
    }


    return mini;
}