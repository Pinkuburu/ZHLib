// File(/data/room/foot/dulewu.c) of foot's room
// Create by LUBAN written by Doing Lu
#include <ansi.h>

     inherit PRIVATE_ROOM;

void create()
{
        set("short", "大脚世界小屋");
	set ("long", @LONG
屋子不大，靠窗的地方摆了一张床，屋中一张桌，几只椅，非常
简单。看来主人是一个甘于平淡寂寞的人。 
LONG );

        set("objects", ([
                "/adm/npc/obj/jubaoxiang" : 1,
        ]));
	setup();

        set("KEY_DOOR", ([
                "exit" : "south",
                "room" : __DIR__"xiaoyuan",
        ]));
    

        set("sleep_room", 1);
        set("loving_room", 1);
        set("no_fight", 1);
    
        set("room_owner", "大脚板");
        set("room_name", "大脚世界");
        set("room_id", "foot");
        set("room_owner_id", "foot");
        set("room_position", "青砖路");
}

void init()
{
        ::init();
        add_action("do_findbaby", "xunzhao");
}

int do_findbaby(string arg)
{
        object me = this_player(), baby;
        string file;

        if (! arg || (arg != "baby" && arg != "child") ||
            ! is_room_owner(me)) 
                return 0;

        if (! me->query("couple/child"))
                return notify_fail("你又还没有孩子，来这里凑什么热闹啊？\n");

        if (objectp(baby = find_living(me->query("couple/child")))
        &&  environment(baby) && baby->is_baby())
                return notify_fail("你们的孩子已经跑出去玩了，好好四处找找吧！\n");

        if (me->query("gender") == "女性")
                file = read_file("/data/baby/" + me->query("id") + ".o");
        else file = read_file("/data/baby/" + me->query("couple/id") + ".o");

        if (stringp(file))
        {
                baby = new("/clone/user/baby");

                baby->load_baby(me);
                baby->move(environment(me));
                message_vision("你忽然看到床底下" +
                        ({"探出一颗小脑瓜", "伸出一双小脚丫", "伸出一支小手"})
                        [random(3)] + "．．．\n", me);

        } else
        {
                tell_object(me, MAG "你们的孩子不幸夭折了，请节哀吧。\n" NOR);
                me->delete("couple/child");
                me->delete("couple/child_name");
        }
        return 1;
}      
