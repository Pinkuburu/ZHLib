// inspire.c

#include <ansi.h>

int exert(object me, object target)
{
        string force;

        if (! me->query("breakup"))
                return notify_fail("你还没有领悟利用真气激励精神的诀窍。\n");

        if (me->is_busy())
                return notify_fail("等你忙完了手头的事情再说！\n");

	if (me->is_fighting())
		return notify_fail("现在你正在战斗中？还是等打完了再说吧！\n");

        force = me->query_skill_mapped("force");
        if (! stringp(force))
                return notify_fail("先激发你的特殊内功。\n");

	if ((int)me->query_skill(force, 1) < 200)
		return notify_fail("你的" + to_chinese(force) + "修为还不够。\n");

        if ((int)me->query("eff_jing") >= (int)me->query("max_jing"))
                return notify_fail("你现在精神饱满，有什么好激励的？\n");

	if ((int)me->query("neili") < 200)
		return notify_fail("你的真气不够。\n");

	if ((int)me->query("eff_jing") < (int)me->query("max_jing") / 3)
		return notify_fail("你的精损伤太重，现在难以振奋自己。\n");

	message_vision(HIW "$N" HIW "闭目冥神，缓缓的呼吸吐纳，额头上渗出点点汗珠，随即化作白雾蒸腾而起。\n"
                       HIY "$N" HIY "睁开双目，眼中神光四射，精神看上去好多了。\n" NOR,
		       me);

	me->receive_curing("jing", 5 + (int)me->query_skill("force") / 6);
	me->add("neili", -180);
	me->start_busy(1);

	return 1;
}
