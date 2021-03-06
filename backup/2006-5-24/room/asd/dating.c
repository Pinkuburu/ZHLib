// File(/data/room/asd/dating.c) of asd's room
// Create by LUBAN written by Doing Lu

#include <room.h>

     inherit PRIVATE_ROOM;

void create()
{
        set("short", "捡破烂的地方大厅");
	set ("long", @LONG
这就是捡破烂的地方主人好小子迎送客人的地方。正中有些桌椅，
侧面是两扇屏风。东首是主人好小子的房间，有一扇木门虚掩着。南
边是大门，出去就是院子。 
LONG );

	setup();

	set("exits", ([
                "east"   : __DIR__"woshi",
		"north"  : __DIR__"houyuan",
	]));

        create_door("east", "木门", "west", DOOR_CLOSED);

        set("no_sleep_room", 1);

        set("KEY_DOOR", ([
                "exit" : "south",
                "room" : __DIR__"xiaoyuan",
        ]));
    
        set("room_owner", "好小子");
        set("room_name", "捡破烂的地方");
        set("room_id", "asd");
        set("room_owner_id", "asd");
        set("room_position", "青砖路");
}
