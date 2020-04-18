#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MODEL_BITS 8
#define HISTORY_UNITS 4
#define HISTORY_LEN 5
#define NUM_SYMBOLS 0x100
#define TREE_PROBS 0x80
#define PROB_INIT_VAL ((1 << MODEL_BITS) / 2)
typedef uint16_t Prob;

typedef struct ProbabilityModel_struct ProbabilityModel;

ProbabilityModel* probability_model_new();
void probability_model_free(ProbabilityModel* model);

Prob probability_model_get(ProbabilityModel* model, int tree_index);
void probability_model_update(ProbabilityModel* model, int tree_index, bool bit);
void probability_model_insert_byte(ProbabilityModel* model, uint8_t byte);
