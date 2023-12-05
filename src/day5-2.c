#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"

typedef struct {
    unsigned long long int destination_range_start;
    unsigned long long int source_range_start;
    unsigned long long int range_length;
} CategoryMapRange;

typedef struct {
    unsigned long long int range_start;
    unsigned long long int range_length;
} CategoryRange;

CategoryRange *parse_range(const char **str) {
    CategoryRange *rng = (CategoryRange*)myalloc(sizeof(CategoryRange));
    rng->range_start = parse_num(str);
    parse_spaces(str);
    rng->range_length = parse_num(str);
    parse_spaces(str);
    return rng;
}

bool parse_word_map(const char **str) {
    return parse_string(str, "map:");
}

List *parse_map(const char **str) {
    
    parse_newline(str);

    List *ranges = List_newm();

    while (isdigit(**str)) {
        CategoryMapRange *range = (CategoryMapRange*)myalloc(sizeof(CategoryMapRange));

        range->destination_range_start = parse_num(str);
        parse_spaces(str);
        range->source_range_start = parse_num(str);
        parse_spaces(str);
        range->range_length = parse_num(str);
        parse_newline(str);

        List_append(ranges, (void*)range);
    }

    return ranges;
}

List *apply_map(CategoryRange sourcerng, List map) {
    printf("Checking %llu to %llu...\n", sourcerng.range_start, sourcerng.range_start + sourcerng.range_length - 1);

    List *dests = List_newm();

    for (List range = map; !List_isempty(range); range = *range.next) {
        CategoryMapRange rng = *(CategoryMapRange*)range.contents;

        printf("\tAgainst %llu to %llu\n", rng.source_range_start, rng.source_range_start + rng.range_length - 1);

        unsigned long long int start = sourcerng.range_start > rng.source_range_start ? sourcerng.range_start : rng.source_range_start;
        unsigned long long int end = sourcerng.range_start + sourcerng.range_length < rng.source_range_start + rng.range_length ?
            sourcerng.range_start + sourcerng.range_length : rng.source_range_start + rng.range_length;
        
        if (start < end) {
            printf("\tMatches %llu to %llu\n", start, end - 1);

            CategoryRange *dest = (CategoryRange*)myalloc(sizeof(CategoryRange));
            dest->range_start = start + rng.destination_range_start - rng.source_range_start;
            dest->range_length = end - start;
            List_append(dests, dest);
        }
    }

    if (List_isempty(*dests)) {
        CategoryRange *dest = (CategoryRange*)myalloc(sizeof(CategoryRange));
        *dest = sourcerng;
        List_append(dests, dest);
    }

    return dests;
}

int main() {
    String inp = read_file("inp/day5-sample2.txt");

    const char *str = inp.contents;
    
    parse_string(&str, "seeds:");
    parse_spaces(&str);

    List seeds = List_new();

    while (isdigit(*str)) {
        List_append(&seeds, parse_range(&str));
    }

    List maps = List_new();

    while (skip_until(&str, parse_word_map)) {
        List_append(&maps, parse_map(&str));
    }

    for (List map = maps; !List_isempty(map); map = *map.next) {
        List nextSeeds = List_new();

        for (List seed = seeds; !List_isempty(seed); seed = *seed.next) {
            List_appendList(&nextSeeds, apply_map(*(CategoryRange*)seed.contents, *(List*)map.contents));
        }

        seeds = nextSeeds;
    }

    unsigned long long int ans = -1;

    for (List seed = seeds; !List_isempty(seed); seed = *seed.next) {
        ans = *(unsigned long long int*)seed.contents < ans ? *(unsigned long long int*)seed.contents : ans;
    }
    
    printf("Answer = %llu\n", ans);

    return 0;
}

int main2() {
    List l1 = List_new(), l2 = List_new();
    int nums[5] = { 1, 2, 3, 4, 5 };

    for (int i = 0; i < 5; i++) {
        List_append(&l1, nums + i);
        List_append(&l2, nums + i);
    }

    List_appendList(&l1, &l2);

    printf("%llu\n", List_size(l1));
    for (List i = l1; !List_isempty(i); i = *i.next) {
        printf("%d ", *(int*)i.contents);
    }
    printf("\n");

    return 0;
}