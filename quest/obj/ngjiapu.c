#include <ansi.h>
inherit ITEM;
void create()
{
    set_name(CYN"���ϼ���"NOR, ({ "ngjiapu" }) );
    set_weight(1);
    if( clonep() )
            set_default_object(__FILE__);
    else {
            set("unit", "��");
            set("material", "paper");
            set("long", "һ�����ϼ���\n");
          }

    setup();
}
