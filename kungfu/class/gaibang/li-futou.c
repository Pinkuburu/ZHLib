// gaibang npc code

inherit NPC;

void create()
{
        set_name("李斧头", ({"li futou", "li", "futou"}));
        set("title", "丐帮六袋弟子");
        set("gender", "男性");
        set("class", "beggar");
        set("age", 20);
        set("long",
                "这是位丐帮六袋弟子，看来是个深藏不露的高手。\n");
        set("attitude", "peaceful");
        set("rank",6);
        
        set("str", 27);
        set("int", 25);
        set("con", 24);
        set("dex", 27);
        set("max_qi", 250);
        set("max_jing", 220);
        set("neili",300);
        set("max_neili", 300);
        set("jiali",30);
        set("combat_exp", 320000);

        set_skill("force", 120);
        set_skill("jiaohua-neigong", 120);
        set_skill("unarmed", 120);
        set_skill("changquan", 120);
        set_skill("dodge", 120);
        set_skill("feiyan-zoubi", 120);
        set_skill("parry", 120);
        set_skill("begging", 50);
        set_skill("checking", 50);
        map_skill("force", "jiaohua-neigong");
        map_skill("unarmed", "changquan");
        map_skill("dodge", "feiyan-zoubi");
        
        create_family("丐帮", 19, "弟子");
        setup();

        carry_object("/d/gaibang/obj/budai")->set_amount(5);
        carry_object("/d/gaibang/obj/budai")->wear();
}

#include "/kungfu/class/gaibang/promote.h"

