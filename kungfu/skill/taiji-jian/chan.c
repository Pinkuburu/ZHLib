// chan.c 太极剑法「缠」字诀

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        object weapon;
        int level, b;
        string msg;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("太极剑法「缠」字诀只能对战斗中的对手使用。\n");

        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");

        if (target->is_busy())
                return notify_fail(target->name() + "目前正自顾不暇，放胆攻击吧！\n");

        if ( !living(target))
                return notify_fail("对方已经不能战斗了！\n");

        if ((level = me->query_skill("taiji-jian", 1)) < 80)
                return notify_fail("你的太极剑法不够娴熟，不会使用「缠」字诀。\n");

        if (me->query("neili") < 60)
                return notify_fail("你现在真气不够，无法使用「缠」字诀。\n");

        msg = HIG "$N" HIG "使出太极剑法「缠」字诀，剑意圈转，连"
              "绵不绝，连递数个虚招企图扰乱$n" HIG "的攻势。\n" NOR;

        me->add("neili", -50);
        if (random(level) > (int)target->query_skill("parry", 1) / 2)
        {
                msg += HIR "结果$p" HIR "被$P" HIR "闹个手忙脚乱，"
                       "惊慌失措，呆在当场，不知如何应对！\n" NOR;
//                target->start_busy(level / 16 + 2);
                b = level / 20;
                if (b > 8)   b = 8;
                target->start_busy(b);
                me->start_busy(1);
        } else
        {
                msg += CYN "可是$p" CYN "看破了$P" CYN "的企图，镇"
                       "定逾恒，全神应对自如。\n" NOR;
                me->start_busy(2);
        }
        message_combatd(msg, me, target);
        return 1;
}

