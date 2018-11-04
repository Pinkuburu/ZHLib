#include <ansi.h>
#include <armor.h>

inherit WAIST;

void create()
{
        set_name(NOR + MAG "��������" NOR, ({ "zijiao belt", "belt", "zijiao" }));
        set_weight(4000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", NOR + MAG "һ�����Ϻ�����֮�����ɵ�������������͸��а����\n" NOR
                            HIC "��Ч�м�������+ 15        ��Ч�Ṧ������ + 5\n" NOR
                            HIC "����Ч��������+ 5%        ����Ч���ȼ���  40\n" NOR
                            HIC "�����ر�������+ 5%\n" NOR);
                set("unit", "��");
                set("value", 500000);
                set("armor_prop/armor", 40);
                set("armor_prop/avoid_poison", 5);
                set("armor_prop/reduce_poison", 5);
                set("armor_prop/dodge", 5);
                set("armor_prop/parry", 15);
                set("wear_msg", NOR + MAG "$N" NOR + MAG "����ذ�$n"
                    NOR + MAG "ϵ�����䡣\n" NOR);
                set("remove_msg", HIC "$N" HIC "����ذ�$n"
                    HIC "������ȡ��������\n" NOR);
        }
        setup();
}

int query_autoload()
{
        return 1;
}