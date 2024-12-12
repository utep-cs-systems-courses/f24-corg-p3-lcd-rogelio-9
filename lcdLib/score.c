#include "score.h"
#include "buzzer.h"

void callScoreSound() {
    buzzer_set_period(1500);  // different sound for special score
}