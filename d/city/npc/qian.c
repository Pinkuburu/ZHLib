// qian.c
// originally coded by xiang

inherit NPC;
inherit F_BANKER;
void create()
{
        set_name("钱眼开", ({"qian yankai", "qian", "yankai"}));
        set("title", "钱庄掌柜");
        set("nickname", "铁公鸡");
        set("gender", "男性");
        set("age", 34);
        set("str", 22);
        set("int", 24);
        set("dex", 18);
        set("con", 18);
        set("qi", 500);
        set("max_qi", 500);
        set("jing", 100);
        set("max_jing", 100);
        set("shen", 0);

        set("combat_exp", 50000);
        set("shen_type", 1);
        set("attitude", "friendly");
        set("env/wimpy", 50);
        set("chat_chance", 2);
        set("chat_msg", ({
        	"钱眼开骄傲的说道：本银号已经有上百年的历史，在江南可以说"
		"是第一家。\n钱眼开笑着说道：在本店存钱无利息，取钱收十分"
		"之一手续费，客官您看着办吧。\n"
        }));
        set_skill("unarmed", 50);
        set_skill("dodge", 50);
        set_temp("apply/attack", 100);
        set_temp("apply/defense", 100);
        set_temp("apply/damage", 40);
        setup();
}

void init()
{
        object ob;
        
        ::init();        
        if (base_name(environment()) != this_object()->query("startroom")) 
                return;
                
        if (interactive(ob = this_player()) && ! is_fighting()) 
        {
                remove_call_out("greeting");
                call_out("greeting", 0, ob);
        }
        add_action("do_check", "check");
        add_action("do_check", "chazhang");
        add_action("do_convert", "convert");
        add_action("do_convert", "duihuan");
        add_action("do_deposit", "deposit");
        add_action("do_deposit", "cun");
        add_action("do_withdraw", "withdraw");
        add_action("do_withdraw", "qu");
        add_action("do_tax", "tax");
        delete_temp("busy");
}
void greeting(object ob)
{
        string bunch;
        int money;
        
        ob = this_player();
        
        if (! stringp(bunch = ob->query("bunch/bunch_name")))            
        {
                return;
        }
        
        if (ob->query("bunch/level") != 9)
        {
                message_vision("$N笑咪咪的对着$n道：“贵帮帮主何时来此处转帐啊？”\n", this_object(), ob);   
                return;
        }        

        money = BUNCH_D->query(bunch + "/bangzhu_money");
        if (money < 1)
        {
                message_vision("$N对$n陪笑道：“『" + bunch + "』目前无帐可转。”\n", this_object(), ob);
                return;
        }

        BUNCH_D->set(bunch + "/bangzhu_money", 0);        
        BUNCH_D->save();
        
        if ((int)ob->query("balance") + money < 2000000000)
                ob->add("balance", money);
        ob->save();
        
        message_vision("$N将『" + bunch + "』最近的收帐" + MONEY_D->money_str(money) + 
                        "转到了$n的帐下。\n", this_object(), ob);

}


