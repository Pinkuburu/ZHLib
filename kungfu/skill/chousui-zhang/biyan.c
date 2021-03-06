// 弹射碧焰

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        object du;
        int damage;
        int ap, dpa, dpb;
        string msg,str;
        int lvl;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("你只能对战斗中的对手弹射碧焰。\n");

        if ((int)me->query_skill("chousui-zhang", 1) < 150)
                return notify_fail("你的抽髓掌火候不够。\n");

        if (me->query_temp("weapon"))
                return notify_fail("你只有空手才能使用抽髓掌。\n");

        if (!(me->query_skill_mapped("strike") == "chousui-zhang"))
                return notify_fail("你并没有激发抽髓掌，如何用碧焰？\n");

        if (!(me->query_skill_prepared("strike") == "chousui-zhang"))
                return notify_fail("你并没有准备抽髓掌，如何用碧焰？\n");

        if ((int)me->query_skill("poison", 1) < 180)
                return notify_fail("你的基本毒技火候不够。\n");

        if ((int)me->query_skill("huagong-dafa", 1) < 150)
                return notify_fail("你的化功大法火候不够。\n");

        if ((int)me->query("max_neili") < 1500)
                return notify_fail("你的内力修为不足，无法用内力弹射碧焰。\n");

        if ((int)me->query("neili") < 800)
                return notify_fail("你现在内息不足，无法用内力弹射碧焰。\n");

        if (! objectp(du = me->query_temp("handing")))
                return notify_fail("你首先要拿着(hand)一些毒药作为引子。\n");

        if (! mapp(du->query("poison")))
                return notify_fail(du->name() + "又不是毒药，无法弹射？\n");

        msg = HIR "$N" HIR "红着双眼，猛提内力，对准$n"
              HIR "“嗖”的一声，弹射出一点绿莹莹明亮无比的" NOR +
              HIG "碧焰" NOR + HIR "悄然袭去。\n\n" NOR;
        me->start_busy(2 + random(2));
        me->add("neili", -450);

        lvl = me->query_skill("chousui-zhang", 1);
        damage = lvl + random(lvl*3);

        if (living(target))
              dpa = target->query("max_neili") * 5 / 4;
        else  dpa = 0;

        if (me->query("max_neili") * 2 / 3 + random(me->query("max_neili")) < dpa)
        {
                msg += WHT "可是$n" WHT "浑然不放在眼里，衣袖轻轻一拂，便将" NOR +
                       HIG "碧焰" NOR + WHT "熄灭。\n" NOR;
        } else
        {
                ap = me->query_skill("poison", 1) / 2 +
                     me->query_skill("force");

                if (living(target))
                     dpb = target->query_skill("dodge") * 3 / 2;
                else dpb = 0;

                if (ap * 2 / 3 + random(ap) < dpb)
                {
                        msg += CYN "$n" CYN "见势不妙，急忙腾挪身形，避开了$N" CYN "的攻击。\n" NOR;
                        target->start_busy(1);
                } else
                {
                        msg += HIR "结果$n" HIR "一个不慎，被火星溅到了衣衫，哪知" NOR +
                               HIG "碧焰" NOR + HIR "一接触身体便开始大势燃烧，只听得$p"
                               HIR "惨叫连连。\n" NOR;
                        target->affect_by("fire_poison",
                                       ([ "level" : me->query("jiali") * 3 / 2 + random(me->query("jiali") * 2),
                                          "id"    : me->query("id"),
                                          "duration" : lvl / 25 + random(lvl / 15) ]));
                        target->receive_damage("qi", damage, me);
                        target->receive_wound("qi", damage/2, me);
                        target->receive_damage("jing", damage * 2 / 3, me);
                        target->receive_wound("jing", damage/3, me);
                        str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
                        msg += "($n"+str+")\n";
                        target->start_busy(1);
                }
        }

        destruct(du);
        message_combatd(msg, me, target);

        return 1;
}

