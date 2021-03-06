
#include <room.h>
inherit ROOM;

void create()
{
	set("short", "东厂兵器库");
	set("long", @LONG
这里是兵器库，到处银光闪闪，让人眼花缭乱。宝刀、宝剑、金箍棒，
大刀、长剑、哨棒，短刀、短剑、短棍，各色各样的兵器应有尽有，你一
时不知道挑什么好。
LONG );

	set("exits", ([
		"north" : __DIR__"bingyin1",
		"southwest" :  __DIR__"aobai12",
	]));
	set("objects", ([
		"/clone/weapon/changjian" : 1,
		"/clone/weapon/tiegun" : 1,
		"/clone/weapon/changbian" : 1,
		"/clone/weapon/duanjian" : 1,
		"/clone/weapon/gangdao" : 2,
	]));

//	set("no_clean_up", 0);
	create_door("north", "铁门", "south", DOOR_CLOSED);
	setup();
	replace_program(ROOM);
}
