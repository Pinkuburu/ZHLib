#include <ansi.h>
 
inherit ITEM;

void create()
{
        set_name(HIW "「大天龙指」" NOR, ({ "tianlong zhi", "tianlong", "zhi", "book" }));
        set_weight(500);
        if (clonep())
                destruct(this_object());
        else {
                set("unit", "本");
                set("long", YEL "这是一本薄薄的书册，书皮已然发黄，封"
                            "面上写着「大天龙指」四个字。\n" NOR);
                set("value", 1);
                set("no_sell", 1);
                set("material", "paper");
                set("skill", ([
                        "name":       "tianlong-zhi",
                        "exp_required" : 140000,
                        "jing_cost":  60,
                        "difficulty": 54,
                        "max_skill":  1,
                        "min_skill":  0
                ]));
        }
}
