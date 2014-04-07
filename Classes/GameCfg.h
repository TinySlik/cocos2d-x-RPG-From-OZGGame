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

#define GAME_INIT_SQL "insert into save_data (map_id, player_to_x, player_to_y, player_direction, items) values(1, 14.5, 9.5, 'down', '[]');"

#endif
