// zhenyu.c ����������

#include <ansi.h>

inherit ITEM;

void create()
{
        set_name(HIW "����������" NOR, ({ "zhenyu wan", "zhenyu", "wan" }) );
        set_weight(3000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", HIW "һ�����۴�С��ҩ�裬���˱ǣ�һ��֮�±���Ŀ�\n"
                                "����������ҩ�������Ž�����Ĺ������ֳ��أ���\n"
                                "���Ļ�����ͣ������������һ�㡣\n" NOR);
                set("value", 500000);
                set("unit", "��");
                set("only_do_effect", 1);
        }
}

int do_effect(object me)
{
        string mapsk;
        int effect;
        int neili;

        log_file("static/using", sprintf("%s(%s) eat ���������� at %s.\n",
                 me->name(1), me->query("id"), ctime(time())));

        effect = 0;

        if (me->query("skybook/item/zhenyu"))
        {
        	message_vision(HIW "$N" HIW "һ������������һ�����������裬ֻ��$P"
                               HIW "����һ�������϶�ð����������\n" NOR, me);

        	me->add("combat_exp", 20000 + random(1000));
        	me->add("magic_points", 3000);
        	me->add("potential", 3000);

        	mapsk = me->query_skill_mapped("force");
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);
        	if (me->can_improve_skill("force"))
                	me->improve_skill("force", 1500000);

               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);
               	me->improve_skill("martial-cognize", 1500000);

        	me->improve_neili(80);
        	me->improve_jingli(50);
        	me->set("skybook/item/zhenyu", 1);
	}
        destruct(this_object());
        return 1;
}

int query_autoload()
{
        return 1;
}