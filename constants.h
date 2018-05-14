#ifndef CONSTANTS_H
#define CONSTANTS_H
#define TILE_SIZE 10
#define TANK_RADIUS 3
#define NONE 0
#define FIRE 1
#define QUAD 2
#define HELP 3
#define SELF 4
#define RESTART 5
#define CREATE 6
#define SEARCH 7
#define SAVE 10
#define LOAD 11
#define USE 100

#define SEND 1000
#define RECEIVE 1001
#define NOCONNECT 1002

enum GameObjectsData {
    GD_Type
};

enum GameObjectTypes {
    GD_TANK,
    GD_WALL,
    GD_DOOR,
    GD_ENEMY,
    GD_IRON,
    GD_RIVER,
    GD_FOREST,
    GD_LOCK,
    GD_BOX,
    GD_KEY,
    GD_BLINKBOARD,
    GD_MONSTER,
    GD_BULLET,
    GD_SNAKE
};
enum Direction{
    NoMove,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown
};

#endif // CONSTANTS_H
