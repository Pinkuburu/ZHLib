// 弹射火焰

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        object du;
        int damage;
        int ap;
        string msg;
        int lvl;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("你只能对战斗中的对手弹射火焰。\n");

        if ((int)me->query_skill("chousui-zhang", 1) < 80)
                return notify_fail("你的抽髓掌火候不够。\n");

	if (me->query_temp("weapon"))
		return notify_fail("你只有空手才能使用抽髓掌。\n");

	if (!(me->query_skill_mapped("strike") == "chousui-zhang"))
		return notify_fail("你并没有激发抽髓掌，如何用火焰？\n");

	if (!(me->query_skill_prepared("strike") == "chousui-zhang"))
		return notify_fail("你并没有准备抽髓掌，如何用火焰？\n");


        if ((int)me->query_skill("poison", 1) < 80)
                return notify_fail("你的基本毒技火候不够。\n");

        if ((int)me->query_skill("huagong-dafa", 1) < 80)
                return notify_fail("你的化功大法火候不够。\n");

        if ((int)me->query("max_neili") < 500)
                return notify_fail("你的内力修为不足，无法用内力弹射出火焰。\n");

        if ((int)me->query("neili") < 300)
                return notify_fail("你现在内息不足，无法用内力弹射出火焰。\n");

        if (! objectp(du = me->query_temp("handing")))
                return notify_fail("你首先要拿着(hand)一些毒药作为引子。\n");

        if (! mapp(du->query("poison")))
                return notify_fail(du->name() + "又不是毒药，无法弹射？\n");

        msg = HIR "$N" HIR "诡异的一笑，暗自提运内力，手指对准$n"
              HIR "轻轻一弹，只见一点殷红色的火星向$n" HIR "飘去。\n\n" NOR;
        me->start_busy(2 + random(2));
        me->add("neili", -200);

        lvl = me->query_skill("chousui-zhang", 1);
        if (me->query("max_neili") / 2 + random(me->query("max_neili")) <
            target->query("max_neili") * 5 / 4)
        {
                msg += WHT "可是$n" WHT "浑然不放在眼里，衣袖轻轻一拂，便将火星熄灭。\n" NOR;
        } else
        {
                ap = me->query_skill("poison", 1) / 2 +
                     me->query_skill("force");
                if (ap / 2 + random(ap) < target->query_skill("dodge") * 3 / 2)
                {
                        msg += CYN "$n" CYN "见势不妙，急忙腾挪身形，避开了$N的攻击。\n" NOR;
                        target->start_busy(1);
                } else
                {
                        msg += HIG "$n" HIG "只觉得伤口一阵灼伤，随即便跟着麻木起来。\n" NOR;
                        target->affect_by("fire_poison",
                                       ([ "level" : me->query("jiali") + random(me->query("jiali")),
                                          "id"    : me->query("id"),
                                          "duration" : lvl / 50 + random(lvl / 20) ]));
                        target->start_busy(1);
                }
        }

        destruct(du);
        message_combatd(msg, me, target);

        return 1;
}
