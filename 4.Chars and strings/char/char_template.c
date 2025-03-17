#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    *nl = 0;
    *nw = 0;
    *nc = 0;
    int curr;
    int bool = 0;
    for(;;){
        curr = getchar();
        if(curr<0){
            break;
        }
        *nc += 1;
//        printf("%d | %c\n",curr,curr);
        if(isspace(curr)){
            bool = 0;
        }
        else if(bool == 0){
            *nw += 1;
            bool = 1;
        }
        if (curr == '\n') {
            *nl += 1;
        }
//        printf("nl:%d nw:%d nc:%d\n",*nl,*nw,*nc);
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    int curr;
    int ascii_table[LAST_CHAR][2];
    for(int i=0;i<LAST_CHAR;i++){
        ascii_table[i][0] = i;
        ascii_table[i][1] = 0;
    }
    for(;;){
        curr = getchar();
//        printf("%d | %c\n",curr,curr);
        if(curr<0){
            break;
        }
        if(curr >= FIRST_CHAR && curr <= LAST_CHAR-1){
            ascii_table[curr][1] += 1;
        }
    }
    for(int i=0;i<LAST_CHAR-1;i++){
        for(int j=0;j<LAST_CHAR-i-1;j++){
            if(ascii_table[j][1] < ascii_table[j+1][1]){
                int temp0 = ascii_table[j][0];
                int temp1 = ascii_table[j][1];
                ascii_table[j][0] = ascii_table[j+1][0];
                ascii_table[j][1] = ascii_table[j+1][1];
                ascii_table[j+1][0] = temp0;
                ascii_table[j+1][1] = temp1;
            }
        }
    }
    *n_char = ascii_table[char_no-1][0];
    *cnt = ascii_table[char_no-1][1];
}

void bigram_count(int bigram_no, int bigram[]) {
    int bigrams[MAX_BIGRAMS][2];
    for(int i=0;i<MAX_BIGRAMS;i++){
        bigrams[i][0] = i;
        bigrams[i][1] = 0;
    }
    int prev = getchar();
    int curr;
    for(;;){
        curr = getchar();
        if(curr < 0){
            break;
        }
        if((prev >= FIRST_CHAR && prev < LAST_CHAR) && (curr >= FIRST_CHAR && curr < LAST_CHAR)){
            int index = (prev-FIRST_CHAR)*(LAST_CHAR-FIRST_CHAR) + (curr-FIRST_CHAR);
            bigrams[index][1] += 1;
        }
        prev = curr;
    }
    for(int i=0;i<MAX_BIGRAMS-1;i++){
        for(int j=0;j<MAX_BIGRAMS-i-1;j++){
            if(bigrams[j][1] < bigrams[j+1][1]){
                int temp0 = bigrams[j][0];
                int temp1 = bigrams[j][1];
                bigrams[j][0] = bigrams[j+1][0];
                bigrams[j][1] = bigrams[j+1][1];
                bigrams[j+1][0] = temp0;
                bigrams[j+1][1] = temp1;
            }
        }
    }
    int k_index = bigrams[bigram_no-1][0];
    bigram[0] = k_index/(LAST_CHAR-FIRST_CHAR)+FIRST_CHAR;
    bigram[1] = k_index%(LAST_CHAR-FIRST_CHAR)+FIRST_CHAR;
    bigram[2] = bigrams[bigram_no-1][1];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    int prev = getchar();
    int curr;
    for(;;){
        curr = getchar();
//        printf("%c | %c\n",prev,curr);
        if(curr < 0){
            break;
        }
        if(prev == 47 && curr == 47){
//            printf("%c | %c\n",prev,curr);
            *line_comment_counter += 1;
            prev = curr;
            for(;;){
                curr = getchar();
                if(curr == 10){
                    break;
                }
                prev = curr;
            }
        }
        if(prev == 47 && curr == 42){
//            printf("%c | %c\n",prev,curr);
            prev = curr;
            curr = getchar();
            prev = curr;
            for(;;){
                curr = getchar();
                if(prev == 42 && curr == 47){
                    *block_comment_counter += 1;
                    prev = curr;
                    curr = getchar();
                    break;
                }
                prev = curr;
            }
        }
        prev = curr;
    }
}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

//    freopen("str5.txt","r",stdin); //plik z ktorego czyta program

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc(&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
