// yunv-xinjing

inherit SKILL;

string type() { return "knowledge"; }

int valid_enable(string usage) { return usage == "medical"; }

int valid_learn(object me)
{
        if ((int)me->query_skill("medical", 1) < 100)
                return notify_fail("��Ļ���ҽ������̫ǳ����"
                                   "�������µ���Ů�ľ���\n");

         if ((int)me->query_skill("medical",1) > 180) return 1;
        if ((int)me->query_skill("medical", 1) < (int)me->query_skill("yunv-xinjing", 1))
                return notify_fail("��Ļ���ҽ��ˮƽ���ޣ���"
                                   "�����ո���µ���Ů�ľ���\n");

        return 1;
}