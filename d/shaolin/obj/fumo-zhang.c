// fumo-zhang.c ��ħ��

#include <weapon.h>
#include <ansi.h>

inherit STAFF;
inherit F_NOCLONE;

void create()
{
	set_name("��ħ��", ({ "fumo zhang", "zhang" }));
	set_weight(10000);
	set("unit", "��");
	set("long", @LONG
����һ������ѩ���ķ�ħ�ȣ�����������ҫ�˶�Ŀ������������ǰ��
��Ħ����Я����ԭ��һ·��֮������ħ�����Ƿ�����Ͻ�ħ������
LONG );
	set("value", 1000000);
        set("no_sell", 1);
	set("material", "steel");
	set("wield_msg", HIY "ֻ�������һ������ħ���ڿն���������Ծ��$N���С�
˲ʱ��ʼ���Ʊ�ɫ������������������һƬ�ޱ�ɱ�⡣\n" NOR);
	set("unwield_msg", HIY "��ħ����$N���з����ڰ����һת����ৡ��ؿ���$N����\n" NOR);
	init_staff(200);
	setup();
        check_clone();
}