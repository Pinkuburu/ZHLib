// Code by JHSH
// 天罗地网势

#include <ansi.h>
inherit F_SSERVER;

int perform(object me, object target)
{
        object ob;
        string msg;
        int intBT;

        if( !target ) target = offensive_target(me);

	if( !target || !target->is_character() || !me->is_fighting(target) )
		return notify_fail("天罗地网势只能对战斗中的对手使用。\n");

	if( !living(target) )
		return notify_fail("你现在没有必要使用天罗地网势。\n");

	if( me->query_temp("weapon"))
		return notify_fail("你不放下兵器，如何能够使用美女拳法？\n");

	if( (int)me->query_skill("meinv-quan", 1) < 100 )
		return notify_fail("你的美女拳法不够娴熟，使不出天罗地网势。\n");

	if( (int)me->query_skill("yunv-xinfa", 1) < 120 )
		return notify_fail("你的玉女心法修为不够，使不出天罗地网势。\n");

	if( me->query("neili") <= 300 )
		return notify_fail("你现在的内力不足以使出天罗地网势！\n");

	if( me->query("jingli") <= 100 )
		return notify_fail("你现在的精神不足以使出天罗地网势！\n");

	if( me->query_skill_mapped("parry") != "meinv-quan" )
		return notify_fail("你的招架并非美女拳法, 无法使出天罗地网势。\n");

	if( me->query_skill_mapped("force") != "yunv-xinfa" &&
	    me->query_skill_mapped("force") != "nuhai-kuangtao")
		return notify_fail("你所用的内功不对, 无法使出天罗地网势！\n");

	me->add("neili", -50);
	me->add("jingli", -50);

        msg = "\n"+HIW"\n只见$N双臂飞舞，发掌奇快，两只手掌宛似化成了千手千掌笼罩在$n的身前，一招一式清清楚楚自成段落。\n"NOR;

	if( random(me->query("combat_exp")) > random(target->query("combat_exp")) * 2)
	{
		msg += HIC"只见$N双手幻化出满天拳影，没等$n明白的时候已经被$N拳风罩住了全身，想动也动不了了！\n"NOR;
                intBT = (int)me->query_skill("meinv-quan") / 25;
		if (intBT > 20) intBT = 20;
		target->start_busy(intBT);
	}
        else if( random(me->query("combat_exp")) > random(target->query("combat_exp")))
	{
		msg += HIC"$n见$N突然拳法出奇的快，让人匪夷所思，脚下一缓整个身子都被掌影包围了！\n"NOR;
        intBT = (int)me->query_skill("meinv-quan") / 50;
		target->start_busy(intBT);
		if (intBT > 10) intBT = 10;
	}
        else
        {
                message_vision(HIB"\n$n突然发现$N身形突变，感觉不对立时向后纵身才没被拦在掌影之中！\n"NOR, me, target);
                intBT = random(4);
                me->start_busy(4 + intBT);
        }
        message_vision(msg+"\n", me, target);
        return 1;
}
