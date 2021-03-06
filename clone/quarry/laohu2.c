#include <ansi.h>
inherit QUARRY;

void create()
{
	set_name(NOR + YEL "东北虎" NOR, ({ "dongbei hu", "dongbei", "hu" }));
	set("long", YEL "这是一只体型巨大的东北虎，威猛极了。\n" NOR);

	set("age", 3);
	set("str", 35);
	set("dex", 50);
	set("max_qi", 2000);
	set("max_jing", 2000);
        set("neili", 3200);
        set("max_neili", 3200);
	set("combat_exp", 350000);

        set("power", 50);
        set("item1", __DIR__"item/hurou");
        set("item2", __DIR__"item/hupi");

        set_temp("apply/parry", 60);
        set_temp("apply/dodge", 60);
        set_temp("apply/attack", 30);
        set_temp("apply/defense", 60);
        set_temp("apply/unarmed_damage", 30);
        set_temp("apply/armor", 30);

	setup();
}