#include <ansi.h>
#include <armor.h>

inherit CLOTH;

void create()
{
        set_name(HIY "中华制服" NOR, ({ "zhi fu", "zhi", "fu" }) );
        set("long",HIY "一件做工考究的制服，只见上面金光闪闪，袖口上印着几\n"
                   "个字：「中华英雄」。\n");
        set_weight(100);
        if( clonep() )
                set_default_object(__FILE__);
        else {
                set("unit", "件");
                set("material", "cloth");
                set("armor_prop/armor", 600);
        }
        setup();
}


