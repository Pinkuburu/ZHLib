// po.c 独孤九剑

#include <ansi.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg,str;
        string *learned;
        object weapon, weapon2;
        string type,perform_type;
        mapping prepare;
        int n, tp;
        int skill, ap, dp, damage, ss;

        me->clean_up_enemy();
        if (! target) target = me->select_opponent();

        skill = me->query_skill("lonely-sword", 1);

        if (! me->is_fighting(target))
                return notify_fail("独孤九剑只能对战斗中的对手使用。\n");

        if(me->query_skill_mapped("sword") != "lonely-sword") 
                return notify_fail("你没有用独孤九剑，无法使用九剑绝招！\n");

        if (skill < 50)
                return notify_fail("你的独孤九剑等级不够，练好了再来！\n");

        if (! objectp(weapon = me->query_temp("weapon")) ||
            weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");

        if (me->query("neili") < 200)
                return notify_fail("你的内力不够。\n");
        weapon2 = target->query_temp("weapon");
        prepare = target->query_skill_prepare();
        if (weapon2) type = weapon2->query("skill_type");
        else if (! prepare || sizeof(prepare) == 0) type = "unarmed";
        else if (sizeof(prepare) == 1) type = (keys(prepare))[0];
        else if (sizeof(prepare) == 2) 
                type = (keys(prepare))[target->query_temp("action_flag")];

        ap = me->query_skill("sword");
        tp = me->query_temp("apply/attack");

        learned = me->query("can_perform/lonely-sword");
        if (arrayp(learned) && member_array(type, learned) != -1)
                ap *= 2;

        ss = 0;

        dp = target->query_skill(type, 1) * 2 + ss * 2;

        switch(type)
        {
                case "sword":
                        perform_type = "歪歪斜斜刺出，招式全然不成章法，";  //破剑式
                        break;
                case "blade":
                        perform_type = "摇摇晃晃刺去，剑上浑没半分劲力，";  //破刀式
                        break;
                case "staff":
                case "strike":
                case "club":
                        perform_type = "全不顾对手来招，剑尖一挑，斜斜划出，";  //破枪式
                        break;
                case "hammer":
                case "dagger":
                        perform_type = "剑柄疾收反撞，剑尖向空处随手刺出，";  //破鞭式
                        break;
                case "unarmed":
                case "finger":
                case "claw":
                case "hand":
                case "cuff":
                        perform_type = "连连颤动，幻出点点光芒，迎着对手掌风疾刺而至！";  
//破掌式
                        break;
                case "throwing":
                        perform_type = "刷刷刷几剑，全从希奇古怪的方位刺削而至，";  //破箭式
                        break;
                case "whip":
                        perform_type = "左削右劈，随意挥洒，根本不依剑术招理，";  //破索式
                        break;
                case "force":
                        perform_type = "向天空抛了上去，跟着骈指作剑，挟风疾刺而出，";  //破气式
                        break;
                default:
                        perform_type = "顾自乱挥乱舞，口中胡念着『唵嘛呢叭咪哄』之类，";  
//破阵式
                        break;
        }

        if (ap / 2 + random(ap) + random(tp * 3 / 2) > dp || !living(target))
        {
                msg = HIC "$N" HIC "手中的" + weapon->name() + HIC
                      "随意挥洒，招招从出其不意的方位直指$n" HIC +
                      to_chinese(type)[4..<1] + "中的破绽。\n" NOR;
                n = 3 + random(5);
                if (ap / 3 + random(ap) + random(tp * 3 / 2) > dp || !living(target))
                {
                        msg += HIY "$n" HIY "大吃一惊，慌乱之下破绽迭出，$N" HIY "唰唰连攻" +
                               chinese_number(n) + "剑！\n" NOR;
                        message_combatd(msg, me, target);
                        while (n-- && me->is_fighting(target))
                        {
                                if (! target->is_busy() && random(2))
                                        target->start_busy(1);
                                COMBAT_D->do_attack(me, target, weapon, 0);
                        }

                        if (weapon2 && type != "pin")
                        {
                                msg = HIW 
"\n忽地"+me->name()+"手腕一振，手中"+weapon->name()+HIW""+perform_type+"\n"NOR;
                                msg += HIY 
"\n$n见来招怪异，正自莫名其妙间，突觉手腕剧痛，手中"+weapon2->name()+
                                      HIY "再也拿捏不住，脱手飞向半空！\n" NOR;
                                weapon2->move(environment(me));
                                target->reset_action();
                                target->receive_damage("qi", ap + random(ap/2), me);
                                target->receive_wound("qi", ap + random(ap/2), me);
				str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
				msg += "($n"+str+")\n";
                        } else
                        {
                                msg = HIW "\n只见"+me->name()+"手腕轻抖，手中"+
					weapon->name()+HIW""+perform_type+"\n" NOR;
                                msg += HIM "\n$n猛觉剧痛连心，赫然发现自己手掌竟已被"+
					weapon->name()+HIM"洞穿，不由得哀声痛叫！\n"NOR;
                                target->receive_damage("qi", ap + random(ap), me);
                                target->receive_wound("qi", ap + random(ap), me);
				str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
				msg += "($n"+str+")\n";
                        }
			target->start_busy(1);
                } else
                {
                        msg += HIY "$n" HIY 
"见来招怪异，情知不妙，连忙手忙脚乱的出招抵挡，一时间无暇反击。\n" NOR;
                        target->start_busy(4 + random(skill / 100));
                }
                me->start_busy(2 + random(2));
        } else
        {
                msg = HIC "$N" HIC "拿着手中的" + weapon->name() + HIC "，东戳西指，"
                      "可是$n" HIC "防守的异常严密，$N" HIC "一时竟然无法找到破绽。\n" NOR;
                me->start_busy(3 + random(2));
                target->start_busy(1);
        }

        message_vision(msg, me, target);
        if(!userp(target) && !target->is_killing(me->query("id")) ) target->kill_ob(me);
	me->add("neili",-100);
        return 1;
}


