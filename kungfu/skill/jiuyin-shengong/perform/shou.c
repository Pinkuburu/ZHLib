// shou.c ��������

#include <ansi.h>

inherit F_SSERVER;
 
int perform(object me, object target)
{
	string msg;
        int ap, dp;
 
	if (! target) target = offensive_target(me);

	if (! target || ! me->is_fighting(target))
	        return notify_fail("��������ֻ����ս���жԶ���ʹ�á�\n");

	if (me->query_skill("jiuyin-shengong", 1) < 150)
		return notify_fail("��ľ����񹦻�������죬����ʹ�þ������֣�\n");

	if (me->query("neili") < 300)
		return notify_fail("�������������\n");

        msg = HIY "$N" HIY "��������һ���³���һ��������ʽ���أ�������ı���$n"
              HIY "��ҪѨ��\n";

        ap = me->query_skill("jiuyin-shengong");
        dp = target->query("combat_exp") / 10000;
	me->add("neili", -60);
	me->start_busy(1 + random(2));

	me->want_kill(target);
        if (dp >= 100)
        {
                msg += HIC "$n" HIC "֪�����в��ƣ�С��Ӧ�ԣ�û��һ������\n" NOR;
        } else
        if (random(ap) > dp)
        {
                msg += HIR "��һ����ȫ������$n" HIR "�����󣬱�$N"
                       HIR "���ʵʵ�Ĵ�����̴�д�Ѩ������������ʱ��ɢ��\n" NOR;
                message_combatd(msg, me, target);
                target->die(me);
                return 1;
        } else
        {
                msg += HIM "$n" HIM "���һ������æ���ҵֵ�����"
                      "Ȼû��һ���˺������ҵ��ѣ�\n" NOR;
        }

        message_combatd(msg, me, target);

	return 1;
}