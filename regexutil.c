#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// https://stackoverflow.com/questions/4214314/get-a-substring-of-a-char
int getSubString(const char *str, int s, int e, char *substr, int outbufsize) {
    if((e - s) > outbufsize) {
        return -1;
    }

    memcpy(substr, &str[s], e - s);
    substr[e - s] = '\0';
    return 0;
}

/**
 * TODO: マッチ文字列リストをコール元に返す
 * @param pattern 正規表現パターン。パターン内で使用可能なグループ数の上限は10。
 * @param target pattern適用対象の文字列
 * @param matchStrs マッチした文字列のリスト
 * @retval 0 No match
 * @retval 1 Match
 *
 * https://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex
 */
int getRegexMatch(const char *pattern, const char *target) {
    printf("pattern[%s]\ntarget[%s]\n", pattern, target);

    /* Compile regular expression */
    size_t maxGroups = 10;
    regex_t regex;
    regmatch_t groupArray[maxGroups];
    int reti = regcomp(&regex, pattern, REG_EXTENDED);
    //int reti = regcomp(&regex, "^a[[:alnum:]]", 0);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, target, maxGroups, groupArray, 0);
    //reti = regexec(&regex, "abc", 0, NULL, 0);
    if (reti == 0) {
        int i = 0;
        for (i = 0; i < maxGroups; i++) {
            if(groupArray[i].rm_so == (size_t)-1) {
                break; // No more groups
            }

            char matchStr[strlen(target) + 1];
            getSubString(target,
                         groupArray[i].rm_so,
                         groupArray[i].rm_eo,
                         matchStr, sizeof(matchStr));

            // for debug
            printf("Group %u: [%2u-%2u]: %s\n",
                    i, groupArray[i].rm_so, groupArray[i].rm_eo,
                    matchStr);
        }
    }
    else if (reti == REG_NOMATCH) {
        puts("No match");
    }
    else {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    /* Free memory allocated to the pattern buffer by regcomp() */
    regfree(&regex);

    return (!reti);
}

int main(int argc, char *argv[]) {
    //if(argc != 3) {
    //    fprintf(stderr, "Input 2 args\n");
    //    exit(1);
    //}

    //char target[1024];
    //char pattern[1024];
    //strcpy(pattern, argv[1]);
    //strcpy(target, argv[2]);
    //printf("### %d\n", getRegexMatch(pattern, target));

    char t1[1024] = "ajbzzz,ccc";
    char p1[1024] = "(a.*b)(z+),(c+)";
    getRegexMatch(p1, t1);

    char t2[1024] = "key1=123,key2=abc,key3=999";
    char p2[1024] = "([a-z0-9]+)=([0-9]{3}),([a-z0-9]+)=([a-zA-Z]{3}),([a-z0-9]+)=(9{3})";
    getRegexMatch(p2, t2);

    char t3[1024] = "";
    char p3[1024] = "";

    char t4[1024] = "";
    char p4[1024] = "";
}

