// duo.c ��ڶ���

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int perform(object me, object target)
{
        object weapon;
        int lv1,lv2,comb1,comb2;
        int force1,force2,neili1,neili2,str1,str2,staff1,parry2,dodge1,dodge2;
        if( !target ) target = offensive_target(me);

        if( !target
        ||      !target->is_character()
        ||      !me->is_fighting(target))
                return notify_fail("��ڶ���ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( !objectp(weapon = me->query_temp("weapon") ) )
                return notify_fail("�����õĲ��Ǵ򹷰���������ʩչ��ڶ��ȣ�\n");

        if( me->query_temp("secondary_weapon"))
                return notify_fail("��˫�ֶ����ű�����û�е���ֻ�ֿ���ʩչ��ڶ��ȣ�\n");

        if (weapon->query("skill_type") != "staff")
                return notify_fail("�����õĲ��Ǵ򹷰���������ʩչ��ڶ��ȣ�\n");

        if( me->query_skill_mapped("staff") != "dagou-bang" )
                return notify_fail("�����õĲ��Ǵ򹷰���������ʩչ��ڶ��ȣ�\n");

        if( me->query_skill_mapped("force") != "huntian-qigong" )       
                return notify_fail("�����õĲ��ǻ���������ʩչ������ڶ��ȣ�\n");

        if( me->query_skill("force") < 100 )
                return notify_fail("��Ļ�������������ǳ���޷�ʩչ��ڶ��ȣ�\n");

        if( me->query_skill("staff") < 100 )
                return notify_fail("��ڶ�����Ҫ��տ�Ĵ򹷰���������Чʩչ��\n");

        if( me->query("neili") <= 300 )
                return notify_fail("�����������ʹ����ڶ��ȣ�\n");

        if( !objectp(weapon = target->query_temp("weapon") ) )
                return notify_fail("���ֲ�δʹ�ñ��У�\n");

        message_vision(HIG "\n��Ȼ$N�۽�$n��ǰ�������ָ����$n˫�۲�ȥ��\n\n" NOR, me, target);

        staff1 = me->query_skill("staff",1);
        dodge1 = me->query_skill("dodge",1);
        force1 = me->query_skill("force",1);
        neili1 = me->query("neili");
        str1 = me->query_str();

        if (living(target))
        {
             parry2 = target->query_skill("parry",1);
             dodge2 = target->query_skill("dodge",1);
             force2 = target->query_skill("force",1);
             neili2 = target->query("neili");
             str2 = target->query_str();
        }  else
        {
             parry2 = 1;
             dodge2 = 1;
             force2 = 1;
             neili2 = 1;
             str2 = 1;
        }

        lv1 = staff1*dodge1;
        lv2 = parry2*dodge2;

        if (lv1 < lv2)
        {
                message_vision("˭֪$n���ܲ���������$N��ָ�ѵ���ǰһ�ߣ��͵�ʹһ���������š���$N�Ĺ�����ʱ��գ�$nͬʱʩ�Է�����\n",me,target);
                me->start_busy(1+random(3));
                me->add("neili",-50);
                COMBAT_D->do_attack(target,me,me->query_temp("weapon"));
                return 1;
        }

        if (lv1 > random(lv2*2))
        {
                message_vision(HIR"$n��ͷһ������æ��������$N��˫ָ��\n\n"NOR,me,target);
                comb1 = str1*force1;
                comb2 = str2*force2;
                
                if (comb1 < comb2)
                {
                        
                message_vision(HIG"$N����ջ�˫ָ�������³�����ץס��$n���е�"+weapon->query("name")+HIG"���˾��ضᣡ\n\n"NOR,me,target);
                        
                message_vision(HIR"$Nֻ��$n���е�"+weapon->query("name")+HIG"��ͬ��������һ�㣬ֻ�÷��֣�Ʈ���˿���\n"NOR,me,target);
                        me->start_busy(1+random(2));
                        me->add("neili",-150);
                        return 1;
                }
        
                if (comb1 > comb2 * 4/5)
                {
                        
                message_vision(HIG"����$N����ջ�˫ָ�������³�����ץס��$n���е�"+weapon->query("name")+HIG"���������ɵض��˹�ȥ��\n"NOR,me,target);
                        weapon->move(me);
                        me->add("neili",-50);
                        return 1;
                }

                message_vision(HIG"����$N����ջ�˫ָ�������³�����ץס��$n���е�"+weapon->query("name")+HIG"���˾��ضᣡ\n\n"NOR,me,target);
                message_vision(HIR"$n����ͷ���ԣ���æ������������$N�Գ֡�\n������\n����������\nһʱ˫�����ֲ��¡�\n\n",me,target);

                        switch(random(3))
                        {
                                case 0:
                                {
                                        
                message_vision(HIG"ͻȻ$N���һ�������ϼ�������"+weapon->query("name")+HIG"Ӳ�������\n",me,target);
                                        weapon->move(me);
                                        me->add("neili",-150);                                  
                                }       
                                break;
                                case 1:
                                {
                                        
                message_vision(HIR"ͻȻ$N��$nͬʱ��һ�����������˿�������"+weapon->query("name")+HIG"����$N���У�\n",me,target);
                                        weapon->move(me);
                                        me->add("neili",-200);                                  
                                        target->add("neili",-150);                                      
                                }
                                break;
                                case 2:
                                {
                                        
                message_vision(HIR"ͻȻ$N��$nͬʱ��һ�����������˿�������"+weapon->query("name")+HIG"����$n���У�\n",me,target);
                                        me->add("neili",-200);                                  
                                        target->add("neili",-150);
                                        me->start_busy(1+random(2));
                                }
                                break;
                        }
                        return 1;
        }
        message_vision("$n������թ������Ӳ�ӣ�Ʈ���㿪$N�Ĺ�����\n",me,target);
        me->add("neili",-50);
        me->start_busy(1+random(2));
        return 1;
}
