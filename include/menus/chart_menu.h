#include "menu.h"

enum ChartMenuIndex {
    TREASURE_CHART_INDEX,
    SPECIAL_CHART_INDEX,
    TRIFORCE_CHART_INDEX,
    ALL_CHARTS_INDEX,
};

enum TreasureChartIndices {
    TREASURE_CHART_1 = 0x800000,
    TREASURE_CHART_2 = 0x010000,
    TREASURE_CHART_3 = 0x40000000,
    TREASURE_CHART_4 = 0x20000000,
    TREASURE_CHART_5 = 0x1000,
    TREASURE_CHART_6 = 0x100000,
    TREASURE_CHART_7 = 0x040000,
    TREASURE_CHART_8 = 0x0400,
    TREASURE_CHART_9 = 0x10,
    TREASURE_CHART_10 = 0x1,
    TREASURE_CHART_11 = 0x0100,
    TREASURE_CHART_12 = 0x200000,
    TREASURE_CHART_13 = 0x8000,
    TREASURE_CHART_14 = 0x2,
    TREASURE_CHART_15 = 0x0200,
    TREASURE_CHART_16 = 0x08000000,
    TREASURE_CHART_17 = 0x80,
    TREASURE_CHART_18 = 0x04000000,
    TREASURE_CHART_19 = 0x2000,
    TREASURE_CHART_20 = 0x0800,
    TREASURE_CHART_21 = 0x010000,
    TREASURE_CHART_22 = 0x20,
    TREASURE_CHART_23 = 0x2000,
    TREASURE_CHART_24 = 0x080000,
    TREASURE_CHART_25 = 0x0100,
    TREASURE_CHART_26 = 0x0800,
    TREASURE_CHART_27 = 0x020000,
    TREASURE_CHART_28 = 0x10000000,
    TREASURE_CHART_29 = 0x01000000,
    TREASURE_CHART_30 = 0x0400,
    TREASURE_CHART_31 = 0x4000,
    TREASURE_CHART_32 = 0x4000,
    TREASURE_CHART_33 = 0x8000,
    TREASURE_CHART_34 = 0x02000000,
    TREASURE_CHART_35 = 0x400000,
    TREASURE_CHART_36 = 0x40,
    TREASURE_CHART_37 = 0x0200,
    TREASURE_CHART_38 = 0x020000,
    TREASURE_CHART_39 = 0x040000,
    TREASURE_CHART_40 = 0x080000000,
    TREASURE_CHART_41 = 0x1000,
};

enum SpecialChartIndices {
    TINGLE_CHART = 0x4,
    GHOST_SHIP_CHART = 0x8,
    INCREDIBLE_CHART = 0x080000,
    BIG_OCTO_CHART = 0x100000,
    FAIRY_CHART = 0x200000,
    HEART_PIECE_CHART = 0x400000,
    SEA_HEARTS_CHART = 0x800000,
    SECRET_CAVE_CHART = 0x01000000,
    LIGHT_RING_CHART = 0x02000000,
    PLATFORMS_CHART = 0x04000000,
    BEEDLE_CHART = 0x08000000,
    SUBMARINE_CHART = 0x10000000,
};

enum TriforceChartIndices {
    TRIFORCE_CHART_1 = 0x1,
    TRIFORCE_CHART_2 = 0x2,
    TRIFORCE_CHART_3 = 0x4,
    TRIFORCE_CHART_4 = 0x8,
    TRIFORCE_CHART_5 = 0x10,
    TRIFORCE_CHART_6 = 0x20,
    TRIFORCE_CHART_7 = 0x40,
    TRIFORCE_CHART_8 = 0x80,
};

struct GZChart {
    u32 index;
    u8 byte;
};



GZChart g_charts[61] = {
    {TREASURE_CHART_1, 0},{TREASURE_CHART_2, 0},{TREASURE_CHART_3, 0},
    {TREASURE_CHART_4, 0},{TREASURE_CHART_5, 0},{TREASURE_CHART_6, 0},
    {TREASURE_CHART_7, 1},{TREASURE_CHART_8, 1},{TREASURE_CHART_9, 1},
    {TREASURE_CHART_10, 1},{TREASURE_CHART_11, 0},{TREASURE_CHART_12, 0},
    {TREASURE_CHART_13, 1},{TREASURE_CHART_14, 1},{TREASURE_CHART_15, 0},
    {TREASURE_CHART_16, 0},{TREASURE_CHART_17, 1},{TREASURE_CHART_18, 0},
    {TREASURE_CHART_19, 1},{TREASURE_CHART_20, 0},{TREASURE_CHART_21, 1},
    {TREASURE_CHART_22, 1},{TREASURE_CHART_23, 0},{TREASURE_CHART_24, 0},
    {TREASURE_CHART_25, 1},{TREASURE_CHART_26, 1},{TREASURE_CHART_27, 1},
    {TREASURE_CHART_28, 0},{TREASURE_CHART_29, 0},{TREASURE_CHART_30, 0},
    {TREASURE_CHART_31, 0},{TREASURE_CHART_32, 1},{TREASURE_CHART_33, 0},
    {TREASURE_CHART_34, 0},{TREASURE_CHART_35, 0},{TREASURE_CHART_36, 1},
    {TREASURE_CHART_37, 1},{TREASURE_CHART_38, 0},{TREASURE_CHART_39, 0},
    {TREASURE_CHART_40, 0},{TREASURE_CHART_41, 1},{TINGLE_CHART, 1},
    {GHOST_SHIP_CHART, 1},{INCREDIBLE_CHART, 1},{BIG_OCTO_CHART, 1},
    {FAIRY_CHART, 1},{HEART_PIECE_CHART, 1},{SEA_HEARTS_CHART, 1},
    {SECRET_CAVE_CHART, 1},{LIGHT_RING_CHART, 1},{PLATFORMS_CHART, 1},
    {BEEDLE_CHART, 1},{SUBMARINE_CHART, 1},{TRIFORCE_CHART_1, 0},
    {TRIFORCE_CHART_2, 0},{TRIFORCE_CHART_3, 0},{TRIFORCE_CHART_4, 0},
    {TRIFORCE_CHART_5, 0},{TRIFORCE_CHART_6, 0},{TRIFORCE_CHART_7, 0},
    {TRIFORCE_CHART_8, 0},
};

class ChartMenu : public Menu {
public:
    ChartMenu() : Menu() {}
    static void draw();

    static Cursor cursor;
};

const char* chart_to_str(u8 chart_bit, u32 chart_value);