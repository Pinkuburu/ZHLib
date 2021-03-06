// cheng-buyou.c  成不忧
inherit NPC;
#include <ansi.h>

mixed ask_me();
void create()
{
        set_name("成不忧", ({ "cheng buyou", "cheng" }) );
        set("gender", "男性");
        set("class", "swordsman");
        set("title", "华山剑宗第十三代弟子");
        set("age", 41);
        set("long",
                "一个白净面皮的矮子，可也不能小看他。\n");
        set("attitude", "peaceful");
        set("str", 26);
        set("int", 24);
        set("con", 23);
        set("dex", 27);

        set("max_qi", 2800);
        set("max_jing",1400);
        set("neili", 3600);
        set("max_neili", 3600);
        set("jiali", 45);

        set("combat_exp", 850000);
        set("shen_type", -1);
        set("apprentice_available", 3);

        set("chat_chance_combat", 80);
        set("chat_msg_combat", ({
                (: perform_action, "sword.feilong" :),
                (: exert_function, "recover" :),
        }) );

        set_skill("unarmed", 80);
        set_skill("sword", 150);
        set_skill("force", 100);
        set_skill("parry", 130);
        set_skill("dodge", 130);
        set_skill("literate", 50);
        set_skill("huashan-sword", 150);
        set_skill("huashan-xinfa", 100);
        set_skill("huashan-quanfa", 80);
        set_skill("huashan-shenfa", 130);

        map_skill("sword", "huashan-sword");
        map_skill("parry", "huashan-sword");
        map_skill("force", "huashan-neigong");
        map_skill("unarmed", "huashan-quanfa");
        map_skill("dodge", "huashan-shenfa");

        set("inquiry", ([
                "绝招"   : (: ask_me :),
                "绝技"   : (: ask_me :),
                "夺命连环三仙剑" : (: ask_me :),
        ]));

        create_family("华山剑宗", 13, "弟子");
        setup();

        carry_object("/clone/weapon/changjian")->wield();
        carry_object("/clone/cloth/cloth")->wear();
}

void attempt_apprentice(object ob)
{
        command("say 老夫从不收徒，你另请高明吧。");
}

mixed ask_me()
{
        object me;

        me = this_player();
        if (me->query("can_perform/huashan-sword/duoming"))
                return "你还不快去练习武功，以我剑宗的绝技重振剑宗风采？";

        if (me->query("family/family_name") != "华山派")
                return RANK_D->query_respect(me) + "不是我们华山派的人，请走吧！";

        if (me->query_skill("huashan-sword", 1) < 150)
                return "你的华山剑法还不到家，不要再给我丢脸了！";

        message_vision(HIY "$n" HIY "顺手捡起一根小树枝，挥手"
                       "飞速击出三剑，破空之声不绝于耳！\n"
                       HIY "$n" HIY "回头凝视着$N" HIY "道：这一"
                       "招没什么稀奇，就是要舍生忘死，运足内力！"
                       "虽然华山应该终于剑法，但是你也要好好修炼内力！\n" NOR,
                       me, this_object());
        tell_object(me, HIG "你学会了夺命连环三仙剑。\n" NOR);
        if (me->can_improve_skill("sword"))
                me->improve_skill("sword", 90000);
        me->set("can_perform/huashan-sword/duoming", 1);
        return 1;
}