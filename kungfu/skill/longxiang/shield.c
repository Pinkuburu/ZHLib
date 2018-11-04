// Code of ShenZhou
// shield.c ������

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if( target != me ) 
                return notify_fail("��ֻ����������������˻����񹦡�\n");

        if( (int)me->query("neili") < 100  ) 
                return notify_fail("�������������\n");
        if( (int)me->query_temp("shield") ) 
                return notify_fail("���Ѿ����˹����ˡ�\n");
/*

        if( (int)me->query_temp("longxiang") )
                return notify_fail("���Ѿ���������֮���ˡ�\n");
*/

        if( (int)me->query_skill("longxiang", 1) < 20)
                return notify_fail("������������������\n");

        skill = me->query_skill("force");
        me->add("neili", -100);

        message_vision(
        MAG "$N����һ�������������񹦣�����������������һ����������ȫ����\n" NOR, me);

        me->add_temp("apply/armor",  skill/2);
        me->add_temp("apply/defense",  skill/2);
        //me->add_temp("apply/armor_vs_force", skill/2)

        me->set_temp("shield", 1);

        me->start_call_out( (: call_other, __FILE__, "remove_effect", me, skill/2 :), skill/2);

        if( me->is_fighting() ) me->start_busy(3);

        return 1;
}

void remove_effect(object me, int amount)
{
         int skill;
        skill = me->query_skill("force");

        //me->add_temp("apply/armor_vs_force", - amount)
        me->add_temp("apply/armor", - skill/2);
        me->add_temp("apply/defense", - skill/2);
        me->delete_temp("shield");
        tell_object(me, "��Ļ�����������ϣ��������ջص��\n");
}

