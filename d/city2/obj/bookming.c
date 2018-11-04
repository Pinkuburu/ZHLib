// 明史辑略

inherit ITEM;

void init()
{
        add_action("do_shake", "doudong");
        add_action("do_shake", "shake");
        add_action("do_shake", "twitter");
}

void create()
{
        set_name("明史辑略", ({"book ming","book","ming"}));
        set_weight(1500);
        if (clonep())
                set_default_object(__FILE__);
        else
        {
                set("long", "一本厚厚的书。\n");
                set("unit", "本");
                set("material", "paper");
        }
        setup();
}


int do_shake(string arg)
{
	object me;
	me = this_player();

        if (! arg || ! id(arg)) return 0;

        if (! query("gotgold"))
	{
	        message("vision","突然，几片金叶从书中掉了出来。\n", environment(me));
                new("/d/city2/obj/goldleaf")->move(environment(me));
                new("/d/city2/obj/goldleaf")->move(environment(me));
                set("gotgold", 1);
        } else
                message_vision("$N拿起$n抖了半天，什么也没有发现。\n",
                               me, this_object());

        return 1;
}
