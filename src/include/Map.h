enum TileType
{
    outside_wall,
    inside_wall,
    room,
    entrance,
    way
};
enum DirectionType
{
    north,
    south,
    west,
    east
};
typedef struct
{
    int x, y, w, h;
} ROOM;
const int MAP_W = 50;
const int MAP_H = 50;
const int ROOM_MAX = 15;

class Map
{
public:
    Map()
    {
        for (int i = 0; i < MAP_H; i++)
        {
            for (int j = 0; j < MAP_W; j++)
            {
                map[i][j] = inside_wall;
            }
        }
    }


    int map[MAP_H][MAP_W];
    ROOM rooms[ROOM_MAX];
};