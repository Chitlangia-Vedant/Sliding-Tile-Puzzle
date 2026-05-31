#ifndef DIRECTION_H
#define DIRECTION_H
#include<iostream>
#include<string>
#include<string_view>
#include<array>

class Direction
{
public:
    enum Type
    {
        up=-2,
        right=-1,
        x=0,   // representing no movement
        left=1,
        down=2,
    };
private:
    Type m_dir;
public:
    // FIXED: Make constructors constexpr so they can be evaluated at compile-time
    constexpr Direction() : m_dir(x) {}
    constexpr Direction(Type val) : m_dir(val) {}
    constexpr Direction(int val) : m_dir(static_cast<Type>(val)) {}
    
    [[nodiscard]] Type getDir() const; 
    [[nodiscard]] static Direction getRandom(); 

    [[nodiscard]] Direction operator-() const; 
    [[nodiscard]] bool operator==(Direction other) const;
    [[nodiscard]] bool operator!=(Direction other) const;

    [[nodiscard]] char toChar() const; 
    [[nodiscard]] int asIndex() const; 
    
    static const std::array<Direction, 4> ALL_DIRS;
    
};

inline constexpr std::array<Direction, 4> Direction::ALL_DIRS = {
    Direction(Direction::up), 
    Direction(Direction::right), 
    Direction(Direction::down), 
    Direction(Direction::left)
    };

#endif