#include "menu.h"
#include "libtww/d/kankyo/d_kankyo.h"
#include "libtww/d/com/d_com_inf_game.h"
#include "libtww/f_op/f_op_scene_req.h"
#include "libtww/m_Do/m_Do_main.h"
#include "controller.h"

enum SceneMenuIndex {
    MODIFY_WIND_INDEX,
    TIME_DISP_INDEX,
};

struct GZScene {
    int index;
    bool active;
};

extern GZScene g_scene[2];

class SceneMenu : public Menu {
public:
    SceneMenu() : Menu() {}
    static void draw();
    static void displayTimeInfo();

    static Cursor cursor;
};
