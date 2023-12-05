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

unsigned long long int apply_map(unsigned long long int source, List map) {
    for (List range = map; !List_isempty(range); range = *range.next) {
        CategoryMapRange rng = *(CategoryMapRange*)range.contents;
        
        if (rng.source_range_start <= source && source < rng.source_range_start + rng.range_length) {
            return source + rng.destination_range_start - rng.source_range_start;
        }
    }

    return source;
}

int main() {
    String inp = read_file("inp/day5-sample.txt");

    const char *str = inp.contents;
    
    parse_string(&str, "seeds:");
    parse_spaces(&str);

    ArrayInt seeds = parse_nums(&str, parse_spaces);

    List maps = List_new();

    while (skip_until(&str, parse_word_map)) {
        List_append(&maps, parse_map(&str));
    }

    for (List map = maps; !List_isempty(map); map = *map.next) {
        for (int i = 0; i < seeds.size; i++) {
            seeds.contents[i] = apply_map(seeds.contents[i], *(List*)map.contents);
        }
    }

    unsigned long long int ans = -1;

    for (int i = 0; i < seeds.size; i++) {
        ans = seeds.contents[i] < ans ? seeds.contents[i] : ans;
    }
    
    printf("Answer = %llu\n", ans);

    return 0;
}
