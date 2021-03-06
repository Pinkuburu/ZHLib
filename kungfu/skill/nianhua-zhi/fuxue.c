// fuxue.c 拈花指 「拈花拂穴」

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
	string msg;

	if (! target) target = offensive_target(me);

        if (!target) return notify_fail("你要对谁使用佛穴？\n");
/*
	if (! target || ! me->is_fighting(target))
		return notify_fail("拈花拂穴只能对战斗中的对手使用。\n");
*/

        if (me->query_skill_mapped("force") != "hunyuan-yiqi" &&
            me->query_skill_mapped("force") != "yijinjing" &&
            me->query_skill_mapped("force") != "shaolin-xinfa" &&
            me->query_skill_mapped("force") != "jiuyang-shengong")
                return notify_fail("你使用的非少林内功，无法施展拈花拂穴！\n");

	if (environment(me)->query("no_fight"))
		return notify_fail("你不能在这里使用拈花指。\n");

	if ((int)me->query_skill("force") < 150)
		return notify_fail("你内功火候不够，不能使用拈花拂穴！\n");

	if ((int)me->query_skill("finger") < 150)
		return notify_fail("你的指法修为不够，目前还不能使用拈花拂穴绝技！\n");

	if ((int)me->query("neili") < 200)
		return notify_fail("你现在真气不够，不能使用拈花拂穴！\n");

	if (target->is_busy())
		return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

	if (me->query_skill_mapped("finger") != "nianhua-zhi") 
		return notify_fail("你没有激发拈花指，无法使用拈花拂穴！\n");

	msg = HIY "$N" HIY "使出拈花指绝技「拈花拂穴」，罩住了$n"
              HIY "的全身，指气层层叠叠的攻向$p" HIY "的要穴！\n" NOR;

        if (random(me->query("combat_exp")) > (int)target->query("combat_exp") / 3 || !living(target))
	{
		msg +=  HIG "结果$p" HIG "被$P" HIG "攻了个措手不及！\n" NOR;
		target->start_busy((int)me->query_skill("nianhua-zhi", 1) / 19 + 2);
		me->add("neili", -100);
	}
	else
	{
		msg += CYN "可是$p" CYN "看破了$P"
                       CYN "的招式，连忙跃开，避开了$P"
                       CYN "的攻击。\n" NOR;
		me->add("neili", -30);
		me->start_busy(3);
	}
	message_combatd(msg, me, target);

	return 1;
}
