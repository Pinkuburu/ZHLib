// File(/data/room/shuren/xiaoyuan.c) of shuren's room
// Create by LUBAN written by Doing Lu

     inherit PRIVATE_ROOM;

void create()
{
        set("short", "��ңСԺ");
	set ("long", @LONG
������һ�������Ժ�ӣ����������˼���������������ɢɢ��һ
Щ����ҶƬ��Ժ�����ڣ�����Ժ�Ӿ���һ��С�ݣ��ǳ����¡� 
LONG );

        set("room_key", "1 of shuren");
	setup();

	set("exits", ([
                "out"    : "/d/room/playtown/road13",
	]));

        set("outdoors", "playertown");
        set("no_sleep_room", 1);
        set("valid_startroom", 1);

        set("KEY_DOOR", ([
                "exit" : "north",
                "room" : __DIR__"dulewu",
        ]));
    
        set("room_owner", "������");
        set("room_name", "��ң");
        set("room_id", "winds");
        set("room_owner_id", "shuren");
        set("room_position", "��ש·");
}