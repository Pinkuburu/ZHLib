#include <ansi.h>

inherit ITEM;

void init()
{
	add_action("do_use", "use");
}

void create()
{
	set_name("百花腹蛇膏", ({"baihua gao", "gao","baihua"}));
	if (clonep())
		set_default_object(__FILE__);
	else {
		set("unit", "枚");
		set("long", "这是一枚珍贵的百花腹蛇膏。\n");
		set("value", 20000);
		set("only_do_effect", 1);
	}
	setup();
}

int do_effect(object me)
{
	int limit;
	int max_limit;

	limit = me->query_current_neili_limit();
	max_limit = me->query_neili_limit();
	if (me->query("max_neili") < limit)
	{
                me->add("max_neili", 10 + random(20));
        	me->set("neili", me->query("max_neili"));
		if (me->query("max_neili") > max_limit)
			me->set("max_neili", max_limit);
		
		message_vision(HIY "$N吃下一枚百花腹蛇膏，顿然间只觉一股清凉之气直沁心肺...\n" NOR, this_player());
	}
	else
		message_vision(HIY "$N吃下一枚百花腹蛇膏，但是好象没什么用.\n" NOR, this_player());

	destruct(this_object());
	return 1;
}

int do_use(string arg)
{

	object obj, weapon;
	object me = this_player();
	
	if (!id(arg))
		return notify_fail("你要用什么？\n");

	if (!objectp(weapon = this_player()->query_temp("weapon")))
		return notify_fail("不用武器恐怕不行吧！\n");

	message_vision(HIY "$N取出一枚百花腹蛇膏，将它抹在兵刃及手掌之上.\n" NOR, this_player());
	message_vision(HIY "$N使用的百花腹蛇膏药效开始发作.\n" NOR, this_player());
	obj=new("/d/shenlong/obj/usedgao");
	obj->move(this_player());
	destruct(this_object());
	return 1;
}
