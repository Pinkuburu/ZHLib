// 唱仙法

#include <ansi.h>

int perform(object me)
{
	int skill;
	string msg;

	if (! me->is_fighting())
		return notify_fail("唱仙法只能在战斗中使用。\n");

	if ((int)me->query_skill("shedao-qigong", 1) < 60)
		return notify_fail("你的蛇岛奇功不够娴熟，不会使用唱仙法。\n");

	if ((int)me->query("neili") < 300)
		return notify_fail("你已经唱得精疲力竭，内力不够了。\n");

	if ((int)me->query_temp("chang"))
		return notify_fail("你已经在使用唱仙法了。\n");

	skill = me->query_skill("force");

	me->add("neili", -500);

	message_combatd(HIR "只听$N" HIR "口中念念有词，顷刻"
                        "之间武功大进！\n" NOR, me);

	me->add_temp("apply/attack", skill/6);
	me->add_temp("apply/dodge", skill/6);
	me->add_temp("apply/defense", skill/6);
	me->set_temp("chang", 1);

        call_out("remove_effect", skill/10, me, skill/6);

	return 1;
}

void remove_effect(object me, int extra)
{
        if (!me) return;

        me->add_temp("apply/attack", -extra);
        me->add_temp("apply/defense", -extra);
	me->add_temp("apply/dodge", -extra);

        me->delete_temp("chang");
        message_vision(CYN"$N只觉真气枯竭，口干舌燥，再也无法唱下去了。\n"NOR,me);
}
