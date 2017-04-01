#include <stdlib.h>
#include "ca.h"

ca_state * ca_create(int width, int height, const int *data, int rule_b, int rule_s) {
    int buffer_len = (width + 2) * (height + 2);    
    ca_state * ca = malloc(sizeof (ca_state));
    int *src_ar = calloc(sizeof (int), buffer_len);
    int *dest_ar = calloc(sizeof (int), buffer_len);
    int *upper_sums = calloc(sizeof (int), buffer_len);
    if (!ca || !src_ar || !dest_ar || !upper_sums) {
        free(src_ar), free(dest_ar), free(upper_sums), free(ca);
        return 0;
    }    
    
    for (int b = 0; b < 9; b++) {
        ca->rule_lut[b] = (rule_b >> b) & 1;
        ca->rule_lut[9 + b] = (rule_s >> b) & 1;        
    }    
    
    int buffer_width = width + 2;
    
    for (int y = 0; y < height; y++) {
        int buf_offset = (y + 1) * buffer_width + 1;
        int data_offset = y * width;
            for (int i = 0; i < width; i++)
                src_ar[buf_offset + i] = data[data_offset + i];
    }
    
    ca->src_ar = src_ar;
    ca->dest_ar = dest_ar;
    ca->upper_sums = upper_sums;
    ca->buffer_width = buffer_width;
    ca->buffer_height = height + 2;
    ca->iteration = 0;
    return ca;
}

void ca_free(ca_state * ca) {
    if (!ca)
        return;
    free(ca->src_ar);
    free(ca->dest_ar);
    free(ca->upper_sums);
    free(ca);
}

void ca_iterate(ca_state * ca, int iterations) {
    int w = ca->buffer_width;
    int h = ca->buffer_height;
    int *src_ar = ca->src_ar;
    int *dest_ar = ca->dest_ar;
    int *upper_sums = ca->upper_sums;
    int *rule = ca->rule_lut;

    for (int t = 0; t < iterations; t++) {
            for (int i = w + 1; i < w + w - 1; i++) {
                int upper_sum = src_ar[i - w - 1] + src_ar[i - w] + src_ar[i - w + 1];
                int midde_sum = src_ar[i - 1] + src_ar[i] + src_ar[i + 1];                
                upper_sums[i - w] = upper_sum;
                upper_sums[i] = midde_sum;
            }
            for (int y = 1; y < h - 1; y++)
                for (int x = 1; x < w - 1; x++) {
                    int idx = y * w + x;                    
                    int lower_sum = src_ar[idx + w - 1] + src_ar[idx + w] + src_ar[idx + w + 1];
                    upper_sums[idx + w] = lower_sum;
                    int current = src_ar[idx];
                    int adj = upper_sums[idx - w] + upper_sums[idx] + lower_sum - current;
                    dest_ar[idx] = rule[current * 9 + adj];
                }            
            int *t = src_ar;
            src_ar = dest_ar;
            dest_ar = t;
        }
    ca->src_ar = src_ar;
    ca->dest_ar = dest_ar;
    ca->iteration += iterations;
}

void ca_copy_data(ca_state * ca, int *data) {
    int ca_width = ca->buffer_width;
    int data_width = ca_width - 2;
    int data_height = ca->buffer_height - 2; 
    for (int y = 0; y < data_width; y++) {
        int arOffset = (y + 1) * ca_width + 1;
        int bArOffset = y * data_width;
            for (int i = 0; i < data_height; i++)
                data[bArOffset + i] = ca->src_ar[arOffset + i];
    }
}
