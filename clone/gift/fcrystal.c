// fcrystal.c ϡ��ˮ��

#include <ansi.h>
#include <armor.h>

inherit HEAD;

void create()
{
	set_name(HIM "ϡ��ˮ��" NOR, ({ "flawless crystal" }) );
	set_weight(40);
	if( clonep() )
		set_default_object(__FILE__);
	else {
                set("long", HIM "һ�������˵Ĵ�ˮ��������"
                            "��ۼ���������С�\n" NOR);
                set("value", 100000);
		set("unit", "��");
                set("can_be_enchased", 1);
                set("magic/type", "magic");
                set("magic/power", 125);
                set("armor_prop/personality", 7);
                set("wear_msg", HIC "$N" HIC "����ذ�$n"
                    HIC "����ͷ�ϡ�\n" NOR);
                set("remove_msg", HIC "$N" HIC "����ذ�$n"
                    HIC "��ͷ��ժ��������\n" NOR);
	}
        setup();
}

int query_autoload() { return 1; }