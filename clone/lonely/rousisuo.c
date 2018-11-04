#include <weapon.h>
#include <ansi.h>

inherit WHIP;

void create()
{
        set_name(HIW "��˿��" NOR,({ "rousi suo", "suo", "rousi" }) );
        set_weight(1300);
        if (clonep())
                destruct(this_object());
        else {
                set("long", HIW "һ���ǳ�������ϸ˿�������˿��֯������������֮һ��\n" NOR);
                set("unit", "��");
                set("value", 800000);
                set("no_sell", 1);
                set("material", "steel");
                set("wield_msg", HIW "$N" HIW "�ֱ�һ�ӣ���˿���������ߣ����Ѷ�����\n" NOR); 
                set("unwield_msg", HIW "$N�ֱ�һ�ڣ���˿�����Ѿ��ص����У����⽥��ɢȥ�����һƬ�谵��\n" NOR);
                set("stable", 100);
        }
        init_whip(100);
        setup();
}

mixed hit_ob(object me, object victim, int damage_bonus)
{
        int n;

        if (me->query_skill_mapped("whip") != "chanhun-suo" ||
            me->query_skill("chanhun-suo", 1) < 100)
                return damage_bonus / 2;

        switch (random(6))
        {
        case 0:
                if (! victim->is_busy())
                victim->start_busy(me->query_skill("whip") / 10 + 2);
                return HIY "$N" HIY "ŭ��һ����" HIW "��˿��" HIY "��������һ�㣬�����ζ�����"
                       "��$n" HIY "�����������Ӱ֮�¡�\n" NOR;

        case 1:
                n = me->query_skill("whip");
                victim->receive_damage("qi", n * 3 / 4, me);
                victim->receive_wound("qi", n * 3 / 4, me);
                return  HIW "$N" HIW "����������˿��������£�ʱ�����裬ʱ��������������$n" HIW 
                        "����������Ѩ��\n" NOR;

        }
        return damage_bonus;
}