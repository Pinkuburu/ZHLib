//qingshe.c

#include <ansi.h>

inherit SNAKE;

void create()
{
	set_name("竹叶青蛇", ({ "qing she", "she" }) );
	set("race", "蛇类");
	set("age", 2);
	set("long", "一只让人看了起鸡皮疙瘩的竹叶青蛇。\n");
	set("attitude", "peaceful");
	set("str", 20);
	set("con", 20);
	set("limbs", ({ "头部", "身体", "七寸", "尾巴" }) );
	set("verbs", ({ "bite" }) );

        set("snake_poison", ([
                "level"  : 40,
                "perhit" : 20,
                "remain" : 40,
                "maximum": 40,
                "supply" : 2,
        ]));

	set("combat_exp", 5000 + random(4000));

	set_temp("apply/attack", 18);
	set_temp("apply/damage", 15);
	set_temp("apply/armor", 12);
	set_temp("apply/defence",16);

	setup();
}

void die()
{
	object ob;
	message_vision("$N倒在地上，死了！\n", this_object());
	ob = new("/d/baituo/obj/qingdan");
	ob->move(environment(this_object()));
	::die();
}

int convert(string arg)
{
	object me = this_player();
	object ob;

	if (arg!="snake" && arg!="she" && arg!="qing she") return 0;
	if (me->query("family/family_name") != "欧阳世家") 
		return notify_fail("你不能化蛇为杖。\n");
	return notify_fail("竹叶青过于细小，不能化为杖。\n");
}