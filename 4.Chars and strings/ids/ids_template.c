#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
    int counter = 0;
    int curr;
    char curr_str[MAX_ID_LEN] = "";
//    printf("%s\n",curr_str);
    int i = 0;
    int k = 0;
    for(;;){
        curr = getchar();
        if(curr < 0){
            break;
        }
//        printf("%c\n",curr);
        if(curr == 47){ // comment
            curr = getchar();
            if(curr == 47){ //line comment
                for(;;){
                    curr = getchar();
                    if(curr == 10){
                        break;
                    }
                }
            }
            else if(curr == 42){ //block comment
                int prev = curr;
                curr = getchar();
                prev = curr;
                for(;;){
                    curr = getchar();
                    if(prev == 42 && curr==47){
                        prev = curr;
                        curr = getchar();
                        break;
                    }
                    prev = curr;
                }
            }
        } else if(curr == 34){ // "
            int prev = curr;
            for(;;){
                curr = getchar();
                if(curr == 34){
                    if(prev == 92){
                        continue;
                    }else{
                        break;
                    }
                }
                prev = curr;
            }
        } else if(curr == 39){ // '
            for(;;){
                curr = getchar();
                if(curr == 39){
                    break;
                }
            }
        } else if(isalpha(curr) || curr == '_'){
            curr_str[i] = (char)curr;
            i++;
//            printf("%c\n",curr);
            for(;;){
                curr = getchar();
//                printf("%c\n",curr);
                if(isspace(curr) || curr == 10){
//                    printf("%s\n",curr_str);
                    int flag = 1;
                    for(int j=0;j<32;j++) {
                        if (!strcmp(curr_str, keywords[j])) {
                            memset(curr_str, 0, MAX_ID_LEN);
                            i = 0;
                            flag = 0;
//                            printf("keyword\n");
                            break;
                        }
                    }
                    if(flag){
                        int flag2 = 1;
                        for(int j=0;j<MAX_IDS;j++){
                            if(!strcmp(curr_str,identifiers[j])){
                                flag2 = 0;
//                                printf("identifiers\n");
                                break;
                            }
                        }
                        if(flag2){
                            counter++;
//                            printf("NEW ID: %s\n",curr_str);
                            for(int j=0;j<sizeof(curr_str);j++){
                                identifiers[k][j] = curr_str[j];
                            }
                            k++;
                        }
                    }
                    memset(curr_str, 0, MAX_ID_LEN);
                    i = 0;
                    break;
                }else if(!isalnum(curr) && curr != '_'){
//                    printf("%s\n",curr_str);
                    int flag = 1;
                    for(int j=0;j<32;j++) {
                        if (!strcmp(curr_str, keywords[j])) {
                            memset(curr_str, 0, MAX_ID_LEN);
                            i = 0;
                            flag = 0;
//                            printf("keyword\n");
                            break;
                        }
                    }
                    if(flag){
                        int flag2 = 1;
                        for(int j=0;j<MAX_IDS;j++){
                            if(!strcmp(curr_str,identifiers[j])){
                                flag2 = 0;
//                                printf("identifiers\n");
                                break;
                            }
                        }
                        if(flag2){
                            counter++;
//                            printf("NEW ID: %s\n",curr_str);
                            for(int j=0;j<sizeof(curr_str);j++){
                                identifiers[k][j] = curr_str[j];
                            }
                            k++;
                        }
                    }
                    memset(curr_str, 0, MAX_ID_LEN);
                    i = 0;
                    break;
                }
                curr_str[i] = curr;
                i++;
            }
        }
    }
    return counter;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
    freopen("ids3.txt","r",stdin); //plik z ktorego czyta program
	printf("%d\n", find_idents());
	return 0;
}

