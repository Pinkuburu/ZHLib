// sort by doing

inherit ROOM;

int do_prevent(object me);

void create()
{
	set("short", "长安戏院");
	set("long", @LONG
这里就是著名的长安戏院，几个戏子在台上(stage)唱戏。
台下黑压压的一片都是人。唱到妙处，观众们禁不住大声喝采。
东边是后台。
LONG );
	set("exits", ([
		"south" : __DIR__"xichang1",
                "up" : __DIR__"stage",
                "east" : __DIR__"houtai",
	]));
        set("item_desc", ([
		"stage" : "\n看到戏台，你突然有种想登台表演的冲动。\n" ,
	]));
        set("objects", ([
		"/d/city2/npc/guanzho":  5,
	]));
	setup();
}

int valid_leave(object me, string dir)
{
        if (! userp(me))
                return ::valid_leave(me, dir);

        if (dir == "up")
        {
 	        if (! me->query_temp("hastryup"))
                {
                        me->set_temp("hastryup", 1);
       	                if ((int)me->query("meili") >= 20)
		        {
	                        message_vision("只听哗啦啦一片掌声，观"
                                               "众们一起鼓掌。有人大声"
	                                       "说道：\n静一静，请看这位" +
                                               RANK_D->query_respect(me) +
                                               "表演。\n", me);
                                return ::valid_leave(me, dir);
	                } else
		        {
	                        message_vision("\n突然观众一拥而上，对"
                                               "$N大叫道：凭你也敢上台"
                                               "闹事？老子宰了你这" +
                                               RANK_D->query_rude(me) +
                                               "！\n", me);
                                return do_prevent(me);
                        }
                 }
                 message_vision("$N拼死冲上台去。\n", me);
                 return ::valid_leave(me, dir);
        } 
        return ::valid_leave(me, dir);
}

int do_prevent(object me)
{
        int i;
        object env;
        object man, *ob;

        env = this_object();
        ob = ({ });
        if (!present("guanzhong jia", env))
	{
	        man=new(__DIR__"npc/guanzho1");
	        man->move(env);
                ob += ({ man });
	}

	if (!present("guanzhong yi", env))
	{
	        man=new(__DIR__"npc/guanzho2");
	        man->move(env);
                ob += ({ man });
	}

	if (!present("guanzhong bing", env))
	{
	        man=new(__DIR__"npc/guanzho3");
	        man->move(env);
                ob += ({ man });
	}

	if (!present("guanzhong ding", env))
	{
	        man=new(__DIR__"npc/guanzho4");
	        man->move(env);
                ob += ({ man });
	}

	if (!present("guanzhong wu", env))
	{
	        man=new(__DIR__"npc/guanzho5");
	        man->move(env);
                ob += ({ man });
	}

	for (i=0; i < sizeof(ob); i++)
        {
		ob[i]->set_leader(me);
		ob[i]->kill_ob(me);
		me->fight_ob(ob[i]);
	}

        return notify_fail("观众们拼死也不让你上台。\n");
}
