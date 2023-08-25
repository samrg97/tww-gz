#include <cstdio>
#include "fs.h"
#include "libtww/dolphin/os/OSCache.h"
#include "menus/settings_menu.h"
#include "save_manager.h"
#include "menu.h"
#include "utils/card.h"
#include "practice.h"
#include "menus/item_equip_priority_menu.h"
#include "libtww/d/com/d_com_inf_game.h"
#include "libtww/f_op/f_op_scene_req.h"
#include "libtww/m_Do/m_Do_printf.h"

static char l_filename[128];
SaveManager gSaveManager;

bool SaveManager::s_injectSave = false;

void SaveManager::injectSave(void* src) {
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mPlayerStatusA, src, 0x18);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mPlayerStatusB, (char*) src + 0x18, 0x18);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mPlayerReturnPlace, (char*) src + 0x30, 0xc);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mPlayerItem, (char*) src + 0x3c, 0x15);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mGetItem, (char*) src + 0x51, 0x15);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mItemRecord, (char*) src + 0x66, 8);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mItemMax, (char*) src + 0x6e, 8);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mBagItem, (char*) src + 0x76, 0x18);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mGetBagItem, (char*) src + 0x8e, 0xc);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mBagItemRecord, (char*) src + 0x9a, 0x18);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mCollect, (char*) src + 0xb2, 0xd);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mMap, (char*) src + 0xbf, 0x84);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mInfo, (char*) src + 0x143, 0x5c);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mConfig, (char*) src + 0x19f, 5);
    memcpy((void*) &g_dComIfG_gameInfo.info.mSavedata.mPlayer.mPriest, (char*) src + 0x1a4, 0x10);
    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mPlayer.mStatusC, (char*) src + 0x1b4, 0x1c0);

    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mSave, (char*) src + 0x374, 0x240);

    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mOcean, (char*) src + 0x5b4, 0x64);

    memcpy(&g_dComIfG_gameInfo.info.mSavedata.mEvent, (char*) src + 0x618, 0x100);

    dComIfGs_getSave(g_dComIfG_gameInfo.info.mDan.mStageNo);
}

void SaveManager::injectDefault_before() {
    g_dComIfG_gameInfo.play.mNextStage.wipe = 0;
}

void SaveManager::injectDefault_during() {
    g_dComIfG_gameInfo.play.mNextStage.setLayer(-1);
}

void SaveManager::injectDefault_after() {}

void SaveManager::defaultLoad() {
    gSaveManager.mPracticeFileOpts.inject_options_during_load = SaveManager::injectDefault_during;
    gSaveManager.mPracticeFileOpts.inject_options_after_load = SaveManager::injectDefault_after;
    GZ_clearMenu();
}

void SaveManager::loadSave(uint32_t id, const char* category, special i_specials[], int size) {
    SaveManager::injectDefault_before();

    // Load the corresponding file path and properties
    snprintf(l_filename, sizeof(l_filename), "twwgz/save_files/%s.bin", category);
    loadFile(l_filename, &gSaveManager.mPracticeSaveInfo, sizeof(gSaveManager.mPracticeSaveInfo),
             id * sizeof(gSaveManager.mPracticeSaveInfo));
    
    snprintf(l_filename, sizeof(l_filename), "twwgz/save_files/%s/%s.bin", category,
             gSaveManager.mPracticeSaveInfo.filename);

    SaveManager::defaultLoad();

    if (gSaveManager.mPracticeSaveInfo.requirements) {
        gSaveManager.mPracticeFileOpts.inject_options_after_counter = gSaveManager.mPracticeSaveInfo.counter;
    }

    // If the selected file was a special, run the special callbacks
    if (i_specials) {
        for (int i = 0; i < size; ++i) {
            if (id == i_specials[i].idx) {
                if (i_specials[i].CallbackDuring) {
                    gSaveManager.mPracticeFileOpts.inject_options_during_load =
                        i_specials[i].CallbackDuring;
                }
                if (i_specials[i].CallbackAfter) {
                    gSaveManager.mPracticeFileOpts.inject_options_after_load =
                        i_specials[i].CallbackAfter;
                }
                break;
            }
        }
    }
}

void SaveManager::loadSavefile(const char* l_filename) {
    loadFile(l_filename, MEMFILE_BUF, sizeof(dSv_save_c), 0);
}

void SaveManager::triggerLoad(uint32_t id, const char* category, special i_specials[], int size) {
    loadSave(id, category, i_specials, size);

    SaveManager::loadSavefile(l_filename);
    dSv_save_c* save = (dSv_save_c*) MEMFILE_BUF;

    int state = tww_getLayerNo(save->getPlayer().mPlayerReturnPlace.mName,
                              save->getPlayer().mPlayerReturnPlace.mRoomNo, 0xFF);

    g_dComIfG_gameInfo.play.mNextStage.mRoomNo = save->getPlayer().mPlayerReturnPlace.mRoomNo;
    g_dComIfG_gameInfo.play.mNextStage.mPoint = save->getPlayer().mPlayerReturnPlace.mPlayerStatus;
    strcpy(g_dComIfG_gameInfo.play.mNextStage.mStage, save->getPlayer().mPlayerReturnPlace.mName);
    g_dComIfG_gameInfo.play.mNextStage.mLayer = state;

    // inject options after initial stage set since some options change stage location
    if (gSaveManager.mPracticeFileOpts.inject_options_during_load) {
        gSaveManager.mPracticeFileOpts.inject_options_during_load();
    }

    g_dComIfG_gameInfo.play.mNextStage.enabled = true;
    s_injectSave = true;
}

// runs at the beginning of phase_1 of dScnPly__phase_1 load sequence
void SaveManager::loadData() {
    if (s_injectSave) {
        OSReport("DOES THIS EVEN WORK!!!");
        SaveManager::injectSave(MEMFILE_BUF);

        // This code block happens after the save data is loaded, but before the load sequence completes
        //   so inject_options_after_load() is good for editing save data
        //   but fails in other ways, like editing actor spawn location
        if (gSaveManager.mPracticeFileOpts.inject_options_after_load) {
            gSaveManager.mPracticeFileOpts.inject_options_after_load();
        }
        s_injectSave = false;

        // Put logic for overriding item equip location here
        u8 cur_item_x = dComIfGs_getItemX();
        u8 cur_item_y = dComIfGs_getItemY();
        u8 cur_item_z = dComIfGs_getItemZ();

        OSReport("Item Equips in Save:  %d, %d, %d", cur_item_x, cur_item_y, cur_item_z);

        u8 new_items[3] = {NO_ITEM, NO_ITEM, NO_ITEM};

        for (int i = 0; i < NUM_EQUIPPABLE_ITEMS; i++) {
            u8 item_id = item_enum_to_item_slot(g_item_equip_priorities[i].item_name);
            u8 highest_priority = g_item_equip_priorities[i].high_priority;
            u8 medium_priority = g_item_equip_priorities[i].medium_priority;

            OSReport("Item ID is %d", item_id);

            //dComIfGs_setLife(item_id);
            if (item_id == NO_ITEM) {
                continue;
            }

            if (item_id == PICTO_BOX) {
                if (cur_item_x == DELUXE_PICTO_BOX || cur_item_y == DELUXE_PICTO_BOX || cur_item_z == DELUXE_PICTO_BOX) {
                    item_id = DELUXE_PICTO_BOX;
                }
            } else if (item_id == BOW) {
                if (cur_item_x == BOW_WITH_FIRE_AND_ICE_ARROWS || cur_item_y == BOW_WITH_FIRE_AND_ICE_ARROWS || cur_item_z == BOW_WITH_FIRE_AND_ICE_ARROWS) {
                    item_id = BOW_WITH_FIRE_AND_ICE_ARROWS;
                } else if (cur_item_x == BOW_WITH_LIGHT_ARROWS || cur_item_y == BOW_WITH_LIGHT_ARROWS || cur_item_z == BOW_WITH_LIGHT_ARROWS) {
                    item_id = BOW_WITH_LIGHT_ARROWS;
                }
            }

            if (item_id == cur_item_x || item_id == cur_item_y || item_id == cur_item_z) {
                if (new_items[highest_priority] == NO_ITEM) {
                    new_items[highest_priority] = item_id;
                    OSReport("Assigning item %d to slot %d", item_id, highest_priority);
                } else if (new_items[medium_priority] == NO_ITEM) {
                    new_items[medium_priority] = item_id;
                    OSReport("Assigning item %d to slot %d", item_id, medium_priority);
                } else {
                    new_items[3 - highest_priority - medium_priority] = item_id;
                    OSReport("Assigning item %d to slot %d", item_id, 3 - highest_priority - medium_priority);
                }
            }
        }

        dComIfGs_setItemX(new_items[name_X]);
        dComIfGs_setItemY(new_items[name_Y]);
        dComIfGs_setItemZ(new_items[name_Z]);
    }
}