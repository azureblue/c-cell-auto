#ifndef CA_H
#define CA_H

struct cellular_automaton_state {
    int buffer_width;
    int buffer_height;
    int iteration;
    int rule_lut[20];
    int * src_ar;
    int * dest_ar;
    int * upper_sums;
};

typedef struct cellular_automaton_state ca_state;

ca_state * ca_create(int width, int height, const int * data, int ruleB, int ruleS);

void ca_free(ca_state * ca);

void ca_iterate(ca_state * ca, int iterations);

void ca_copy_data(ca_state * ca, int * data);

#endif /* CA_H */

