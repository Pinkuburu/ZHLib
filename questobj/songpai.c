#include <ansi.h>
inherit ITEM;
void create()
{
    set_name(MAG"松牌"NOR, ({ "songpai" }) );
    set_weight(1);
    if( clonep() )
            set_default_object(__FILE__);
    else {
            set("unit", "块");
            set("material", "stone");
            set("long", "西方神教孤松先生的信物。\n");
          } 
    ::init_item();
} 
