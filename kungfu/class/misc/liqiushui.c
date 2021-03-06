// liqiushui.c

#include <ansi.h>

inherit "/inherit/char/punisher";

void create()
{
        set_name("李秋水", ({ "li qiushui", "li" }));
        set("long",  "她体态轻盈，姿势曼妙，只是脸上蒙了一张面纱，看不真切。\n");
        set("gender", "女性");
        set("age", 88);
        set("attitude", "peaceful");
        set("shen_type", 1);
        set("str", 34);
        set("int", 39);
        set("con", 38);
        set("dex", 37);
        
        set("max_qi", 6000);
        set("max_jing", 3000);
        set("neili", 7600);
        set("max_neili", 7600);
        set("jiali", 160);
        set("combat_exp", 2600000);
        set("score", 10000000);

        set_skill("force", 670);
        set_skill("xiaowuxiang", 670);
        set_skill("dodge", 670);
        set_skill("lingbo-weibu", 670);
        set_skill("liuyang-zhang", 640);
        set_skill("parry", 670);
        set_skill("zhemei-shou", 640);
        set_skill("hand", 640);
        set_skill("strike", 640);
        set_skill("literate", 600);
        set_skill("medical", 690);
        set_skill("xiaoyao-qixue", 690);

        map_skill("force", "xiaowuxiang");
        map_skill("dodge", "lingbo-weibu");
        map_skill("hand", "zhemei-shou");
        map_skill("strike", "liuyang-zhang");
        map_skill("parry", "zhemei-shou");

        prepare_skill("hand","zhemei-shou");
        prepare_skill("strike","liuyang-zhang");

        create_family("逍遥派", 0, "前辈");

        set("chat_chance_combat", 120);
        set("chat_msg_combat", ({
                (: perform_action, "hand.jiubu" :),
                (: perform_action, "strike.jiutian" :),
                (: perform_action, "dodge.lingbo" :),
                (: exert_function, "recover" :),
        }));

        setup();
        carry_object("/clone/misc/cloth")->wear();
}
