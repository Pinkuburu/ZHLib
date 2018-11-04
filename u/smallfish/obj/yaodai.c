// yaodai.c

#include <armor.h>

inherit WAIST;

void create()
{
	set_name("腰带", ({ "waist band","waist","band" }) );
	set_weight(500);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "条");
		set("value", 1000);
		set("material", "leather");
		set("armor_prop/armor", 3);
                set("remove_msg","$N将腰带一下子抽了出来。\n");
	}
	setup();
}

