#include "probability_model.h"
#include <string.h>

struct ProbabilityModel_struct {
	Prob probs[HISTORY_UNITS][NUM_SYMBOLS][TREE_PROBS];
	uint8_t history[HISTORY_LEN];
};

ProbabilityModel* probability_model_new()
{
	ProbabilityModel* model = malloc(sizeof(ProbabilityModel));

	memset(model->history, 0, sizeof(uint8_t) * HISTORY_LEN);
	for (size_t i = 0; i < HISTORY_UNITS; i++) {
		for (size_t j = 0; j < NUM_SYMBOLS; j++) {
			for (size_t k = 0; k < TREE_PROBS; k++) {
				model->probs[i][j][k] = PROB_INIT_VAL;
			}
		}
	}

	return model;
}

void probability_model_free(ProbabilityModel* model)
{
	free(model);
}

// static void xorshift32(uint32_t *state, uint32_t entropy)
// {
// 	*state ^= entropy;
// 	*state ^= *state << 13;
// 	*state ^= *state >> 17;
// 	*state ^= *state << 5;
// }

Prob probability_model_get(ProbabilityModel* model, int tree_index)
{
	int prob = 0;
	int sum = 0;
	for (int i = 0; i < HISTORY_UNITS; i++) {
		int weight = (HISTORY_UNITS - i)*(HISTORY_UNITS - i);
		sum += weight;
		prob += model->probs[i][model->history[i]][tree_index]*weight;
	}
	return prob/sum;
}

#define NUM_MOVE_BITS 1
static void update_prob(bool bit, Prob* prob)
{
	if (bit) *prob -= *prob >> NUM_MOVE_BITS;
	else *prob += ((1 << MODEL_BITS) - *prob) >> NUM_MOVE_BITS;
}

void probability_model_update(ProbabilityModel* model, int tree_index, bool bit)
{
	for (int i = 0; i < HISTORY_UNITS; i++) {
		update_prob(bit, &model->probs[i][model->history[i]][tree_index]);
	}
}

void probability_model_insert_byte(ProbabilityModel* model, uint8_t byte)
{
	for (int i = 0; i < HISTORY_LEN-1; i++) {
		model->history[HISTORY_LEN - i - 1] = model->history[HISTORY_LEN - i - 2];
	}
	model->history[0] = byte;
}
