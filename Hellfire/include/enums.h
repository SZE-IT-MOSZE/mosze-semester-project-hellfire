#ifndef ENUMS_H
#define ENUMS_H

enum class ChoiceType
{
    Strength = 1,
    Intelligence = 2,
    Persuasion = 3,
    Default = 0
};

inline const char* getChoiceTypeText(ChoiceType tp) {

    switch(tp) {
        case ChoiceType::Strength: return "strength";
        case ChoiceType::Intelligence: return "intelligence";
        case ChoiceType::Persuasion: return "persuasion";
        default: return "";
    }
}

enum class ConsumableType
{
    Strength,
    Intelligence,
    Persuasion
};

enum class WeaponType
{
    Staff, Sword
};

enum class MenuOptions : int {
    NEW_GAME = 0, LOAD_GAME = 1, EXIT_GAME = 2

};

inline bool operator==(int number, MenuOptions option) {
return (number == static_cast<int>(option));
}
#endif
