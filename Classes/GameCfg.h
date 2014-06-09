//
//  GameCfg.h
//  OzgGameRPG
//
//  Created by ozg on 14-3-18.
//
//

#ifndef OzgGameRPG_GameCfg_h
#define OzgGameRPG_GameCfg_h

#define GAME_SYS_DB "db.sys" //系统数据库
#define GAME_SCENE 0.5 //场景切换的时间

#define GAME_TMX_ROLE_WIDTH 32.0
#define GAME_TMX_ROLE_HEIGHT 32.0
#define GAME_PLAYER_MOVESPEED 0.5

#define GAME_MAP_ENCOUNTER 0.025 //遇敌率

#define GAME_INIT_SQL "insert into save_data (map_id, player_to_x, player_to_y, player_direction, gold, window_style) values(1, 14.5, 9.5, 'down', 200, 'style2');"

#define GAME_BATTLE_SELECTED_ID "GAME_BATTLE_SELECTED_ID" //战斗画面选择了一个技能或道具后，临时保存的技能id或道具id
#define GAME_BATTLE_FIELD "GAME_BATTLE_FIELD" //战斗画面的执行完毕技能或道具动作后，影响了哪个字段

#define GAME_STYLE "GAME_STYLE" //当前使用的样式

#define MAX_GOLD 999999 //金钱最大值
#define MAX_STATUS1 9999 //HP最大值
#define MAX_STATUS2 999 //MP、攻击力、防御力、速度、技能攻击、技能防御的最大值
#define MAX_ITEMS_TOTAL 99 //最大物品数

#define GAME_BG_AUDIO_VOLUME "GAME_BG_AUDIO_VOLUME" //背景音乐大小
#define GAME_EFFECT_AUDIO_VOLUME "GAME_EFFECT_AUDIO_VOLUME" //效果音大小

#define GAME_MAPSHOP_NPC_ID 7 //商店NPC的id
#define GAME_MAPINN_NPC_ID 8 //旅馆NPC的id
#define GAME_INN 100 //住宿一次花费多少钱

#endif
