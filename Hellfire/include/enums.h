#ifndef ENUMS_H
#define ENUMS_H

enum class ChoiceType
{
    Strength,
    Intelligence,
    Persuasion,
    Default
};

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
