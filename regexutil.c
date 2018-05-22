#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Input 2 args\n");
        exit(1);
    }

    char target[1024];
    char pattern[1024];
    strcpy(pattern, argv[1]);
    strcpy(target, argv[2]);
    printf("target[%s]\n", target);
    printf("pattern[%s]\n", pattern);

    /* Compile regular expression */
    regex_t regex;
    int reti = regcomp(&regex, pattern, 0);
    //int reti = regcomp(&regex, "^a[[:alnum:]]", 0);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, target, 0, NULL, 0);
    //reti = regexec(&regex, "abc", 0, NULL, 0);
    if (!reti) {
        puts("Match");
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
}

