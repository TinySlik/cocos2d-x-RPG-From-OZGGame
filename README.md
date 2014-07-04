ozg game — RPG Demo
================


本游戏主要用于技术的研究和积累



游戏中所用图片来自 http://www.66rpg.com/type.php?t=17 和 http://usui.moo.jp/rpg_tukuru.html

声音文件来自 http://www.66rpg.com/type.php?t=17



目前部署了ios、mac、win32平台。
win32平台下需要将Classes文件夹里面的所有文件的编码转为GBK。
sqlite3需要自行下载，然后把sqlite3.h和sqlite3.c复制到libs里面（ios、mac不需要进行这个步骤）。



游戏第一次启动的时候会把数据库复制到可写文件夹，若数据库结构因为一些改动发生了变化，需要把旧的数据库删除。



编译时需要将本游戏的目录复制到cocos2d-x-2.2.4/projects/。



![](https://raw.github.com/ouzhigang/OzgGameRPG/master/screenshot1.png)

![](https://raw.github.com/ouzhigang/OzgGameRPG/master/screenshot2.png)

![](https://raw.github.com/ouzhigang/OzgGameRPG/master/screenshot3.png)
