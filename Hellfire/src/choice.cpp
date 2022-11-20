#include "choice.h"

Choice::Choice(int dif, int cor, std::string txt, int exp, int tp, int stp) {
    difficulty = dif;
    corruption = cor;
    text = txt;
    experience = exp;
    step = stp;
    type = static_cast<ChoiceType>(tp);
    failed = false;
    chosen = false;
}

std::string Choice::getText(){
    return text;
}

int Choice::getStep() {
    return step;
}

int Choice::getType() {
    return static_cast<int>(type);
}

bool Choice::isFailed(){
    return failed;
}

bool Choice::isChosen(){
    return chosen;
}

int Choice::getDifficulty(){
    return difficulty;
}

int Choice::getExperience(){
    return experience;
}

int Choice::getCorruption(){
    return corruption;
}
