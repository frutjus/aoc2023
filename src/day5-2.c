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

void apply_map(List *sources, List map) {
    for (List *source = sources; !List_isempty(*source); source = source->next) {
        for (List range = map; !List_isempty(range); range = *range.next) {
            CategoryMapRange rng = *(CategoryMapRange*)range.contents;
            CategoryRange src = **(CategoryRange**)source;

            unsigned long long int start = src.range_start > rng.source_range_start ? src.range_start : rng.source_range_start;
            unsigned long long int end = src.range_start + src.range_length < rng.source_range_start + rng.range_length ?
                src.range_start + src.range_length : rng.source_range_start + rng.range_length;
            
            if (start < end) {
                if (start > src.range_start) {
                    CategoryRange *split = (CategoryRange*)myalloc(sizeof(CategoryRange));
                    split->range_start = src.range_start;
                    split->range_length = start - src.range_start;
                    List_insert(source, split);
                }
                if (end < src.range_start + src.range_length) {
                    CategoryRange *split = (CategoryRange*)myalloc(sizeof(CategoryRange));
                    split->range_start = end;
                    split->range_length = src.range_start + src.range_length - end;
                    List_insert(source, split);
                }
                (**(CategoryRange**)source).range_start = start + rng.destination_range_start - rng.source_range_start;
                (**(CategoryRange**)source).range_length = end - start;
                break;
            }
        }
    }
}

int main() {
    String inp = read_file("inp/day5.txt");

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
        
        apply_map(&seeds, *(List*)map.contents);
    }

    unsigned long long int ans = -1;

    for (List seed = seeds; !List_isempty(seed); seed = *seed.next) {
        ans = *(unsigned long long int*)seed.contents < ans ? *(unsigned long long int*)seed.contents : ans;
    }
    
    printf("Answer = %llu\n", ans);

    return 0;
}
