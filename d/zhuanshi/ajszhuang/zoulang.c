// by yuchang
inherit ROOM;


string look_writing();


void create()
{
	set("short", "走廊");
	set("long", @LONG
这是一条走廊。房顶一侧支在西面的高墙上，另一侧则与东
侧的清和殿屋顶相连。彩梁画栋，连顶棚也用彩漆绘满了各种飞
天的图形，每幅精美的雕画似乎都在诉说一个娓娓动人的故事。

LONG
	);

	set("exits", ([
                "east"  : __FILE__,
                "south" : __FILE__,
                "west"  : __FILE__,
                "north" : __FILE__,
        ]));


	setup();
}

void init()
{
     tell_object(this_player(), "你觉得好像迷了路，分辨不清东南西北，"
                                "只有一个劲儿乱钻了!\n");
        
}


int valid_leave(object me, string dir)
{
	int kar;

	kar = 80 - me->query("kar");

	if (random(kar) == 0) 
	{
		tell_object(me, "你走得精疲力尽，终于到了洞外。\n");
		me->move(__DIR__"cjl");
		return 1;
	}

	return ::valid_leave(me,dir);
}



