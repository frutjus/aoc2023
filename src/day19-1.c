#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"
#include "list.h"

typedef enum {
    x,
    m,
    a,
    s,
} Attribute;

Attribute to_attribute(char c) {
    switch (c) {
        case 'x': return x;
        case 'm': return m;
        case 'a': return a;
        case 's': return s;
        default:
            printf("Unrecognised attribute: [%c/%d]\n", c, c);
            exit(1);
    }
}

typedef enum {
    reject,
    accept,
} Outcome;

typedef struct {
    struct {
        Attribute attribute;
        enum {
            test_default,
            test_less_than,
            test_greater_than,
        } type;
        int value;
    } test;
    struct {
        enum {
            target_workflow,
            target_out,
        } type;
        union {
            char workflow[4];
            Outcome out;
        };
    } target;
} Rule;

int hash(const char *str) {
    int h = 0;
    while (*str != '\0') {
        h = h * 26 + (*str - 'a');
        str++;
    }
    return h;
}

Outcome sort(List *workflows, List current_workflow, int *attributes) {
    if (List_isempty(current_workflow)) {
        printf("Ran out of workflows\n");
        exit(1);
    }

    Rule *rule = (Rule*)current_workflow.head;
    bool pass;

    if (rule->test.type == test_default) {
        pass = true;
    } else {
        if (rule->test.type == test_less_than) {
            pass = attributes[rule->test.attribute] < rule->test.value;
        } else {
            pass = attributes[rule->test.attribute] > rule->test.value;
        }
    }

    if (pass) {
        if (rule->target.type == target_out) {
            printf("%c", rule->target.out == accept ? 'A' : 'R');
            return rule->target.out;
        } else {
            printf("%s -> ", rule->target.workflow);
            return sort(workflows, workflows[hash(rule->target.workflow)], attributes);
        }
    } else {
        return sort(workflows, *current_workflow.tail, attributes);
    }
}

int main() {
    const char *str = read_file("inp/day19.txt");

    int workflows_max_size = 26 * 26 * 26 + 26 * 26 + 26;
    List workflows[workflows_max_size];
    iter(i, workflows_max_size) {
        workflows[i] = List_new();
    }

    ull ans = 0;

    while (!parse_newline(&str)) {
        char workflow_name[4] = { 0 };

        for (int i = 0; isalpha(*str); i++, str++) {
            workflow_name[i] = *str;
            if (i > 2) {
                printf("Workflow name too long\n");
                exit(1);
            }
        }

        int workflow_index = hash(workflow_name);

        if (!List_isempty(workflows[workflow_index])) {
            printf("Hash collision: [%s/%d]\n", workflow_name, workflow_index);
            exit(1);
        }

        parse_string(&str, "{");

        do {
            Rule *rule = (Rule*)myalloc(sizeof(Rule));

            if (*(str + 1) == '<' || *(str + 1) == '>') {
                rule->test.attribute = to_attribute(*str);
                str++;
                if (*str == '<') {
                    rule->test.type = test_less_than;
                } else {
                    rule->test.type = test_greater_than;
                }
                str++;
                rule->test.value = (int)parse_num(&str);
                parse_string(&str, ":");
                
            } else {
                rule->test.type = test_default;
            }

            if (*str == 'A') {
                rule->target.type = target_out;
                rule->target.out = accept;
                str++;
            } else if (*str == 'R') {
                rule->target.type = target_out;
                rule->target.out = reject;
                str++;
            } else {
                rule->target.type = target_workflow;

                int i;
                for (i = 0; isalpha(*str); i++, str++) {
                    rule->target.workflow[i] = *str;
                    if (i > 2) {
                        printf("Target workflow name too long\n");
                        exit(1);
                    }
                }
                rule->target.workflow[i] = '\0';
            }

            List_append(&workflows[workflow_index], rule);
        } while (parse_comma(&str));

        parse_string(&str, "}");
        parse_newline(&str);

        //printf("Created workflow [%s] with %llu cases\n", workflow_name, List_size(workflows[workflow_index]));
    }

    for (int i = 1; *str != '\0'; i++) {
        int attributes[4] = { 0 };

        parse_string(&str, "{");

        do {
            int index = to_attribute(*str);
            str++;
            parse_string(&str, "=");
            attributes[index] = parse_num(&str);
        } while (parse_comma(&str));

        parse_string(&str, "}");
        parse_newline(&str);

        printf("%4d | {x=%d,m=%d,a=%d,s=%d} | in -> ", i, attributes[x], attributes[m], attributes[a], attributes[s]);

        Outcome out = sort(workflows, workflows[hash("in")], attributes);

        printf("\n");

        if (out == accept) {
            ans += attributes[x] + attributes[m] + attributes[a] + attributes[s];
        }
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
