// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>

#pragma optimize
#pragma save_binary

inherit F_SAVE;
inherit F_DBASE;

// 帮派声望最大是10亿
#define MAX_BUNCH_FAME         1000000000

// 每个帮派最多有100个仇人
#define MAX_HATRED_PERSON       100

// 当仇人超过的时候每次移除多少个
#define HATREDP_REMOVED         10

mapping bunch_fame;
mapping last_bunch_fame;

// area's base_name & short
mapping area_fame;

string *bad_weather=({
        "受干旱的影响，$N亏空",
        "由于暴雨袭击，$N收成跌了",
        "$N遭到了倭寇的袭击，被劫",
        "官府加重对$N的征税，$N亏空",
        "疾病在$N肆虐，损失惨重",
        "龙卷风袭卷了$N，结果造成巨大损失",
        "$N发生暴动，人员伤亡惨重，损失",
        "$N突发地震，人员伤亡惨重，花费巨大",
});

// 帮派对仇人仇恨度的排序
private int sort_hatred(string id1, string id2, mapping hatred);
public mixed manage_all_area();
public void dismiss_bunch(string fname);

void create()
{
        seteuid(getuid());
        restore();
        set_heart_beat(3600); // 二个小时
        if (! mapp(bunch_fame)) bunch_fame = ([ ]);
        if (! mapp(area_fame)) area_fame = ([ ]);
}

void remove()
{
        save();
}
// 要在 CMDS:REBOOT 里面加上调用
public void mud_shutdown()
{
        save();
}

// 心跳：维护帮派的通常信息
private void heart_beat()
{
        int t;
        int last;
        string *all_fam;
        string fam;

        t = time();
        last = query("last_check");
        if ((t / 86400) != (last / 86400))
        {
                // 天数发生了变化：复制帮派的名望信息
                all_fam = keys(bunch_fame) - ({ 0 });
                last_bunch_fame = ([ ]);

                foreach (fam in all_fam)
                        last_bunch_fame[fam] = bunch_fame[fam];                                   
        }
        set("last_check", t);
        
        // 这里作地盘盘点处理
        manage_all_area();     

        // 保存帮派的最新信息
        save();
}

public mixed manage_all_area()
{
        mapping data = ([ ]);
        string *area_npc, *item;        
        string *all_area, *all_bunch;
        string area, bunch, bunch_name, str;
        string *lost;
        string file, bad_area;
        object room, player;
        object npc;
        int weiwang, jizhi, kaifa, zhongcheng, count, i;
        int area_money, bad_money, rest_money, npc_money, cost;
        
        all_area = keys(area_fame) - ({ 0 });   
        
        if (! arrayp(all_area) || sizeof(all_area) < 1)
                return;  

        if (random(sizeof(all_area)) > 10)
        {
                bad_area = area_fame[all_area[random(sizeof(all_area))]];
                room = get_object(bad_area);
                if (! room->query("area/ziyuan"))
                        bad_area = "";   
        }
        else
                bad_area = "";
                        
        foreach (area in all_area)
        {
                reset_eval_cost();
              
                room = get_object(area);
                
                if (! stringp(bunch = room->query("bunch/bunch_name"))) 
                        continue;
                        
                if (! stringp(file = room->query("bunch/npc_file")))
                        continue;
                
                // 盘点时候帮派 NPC 不在时候则无收入
                if (! stringp(room->query("bunch/npc_id")) ||
                    ! objectp(npc = present(room->query("bunch/npc_id"), room)))
                        continue;
                
                if (! mapp(query(bunch)))
                {
                        message("channel:rumor", HIM"【中华帮会】由于帮派瓦解，" +
                                room->query("short") + "宣布脱离帮会" + "「" + bunch + 
                                "」" + "的控制！\n" NOR, users()); 
                                
                        room->delete("bunch");
                        npc->delete("bunch"); 
                        room->save();                                                                                        
                        continue;
                }
                                
                if (room->query("bunch/money") < -1000000 ||
                    query(bunch + "/money") < -100000000)
                {
                        message("channel:rumor", HIM"【中华帮会】由于经营不善，" +
                                room->query("short") + "宣布脱离帮会" + "「" + bunch + 
                                "」" + "的控制！\n" NOR, users());   
                        
                        if (! arrayp(area_npc = query(bunch + "/area")))   
                                area_npc = ({ });
                        else     
                                area_npc -= ({ area, 0 });                                      
                        set(bunch + "/area", area_npc);     
                        
                        if (! arrayp(area_npc = query(bunch + "/npc")))
                                area_npc = ({ });
                        else
                                area_npc -= ({ file, 0 });
                        set(bunch + "/npc", area_npc);
                        
                        // 调整帮派威望
                        weiwang = bunch_fame[bunch];
                        weiwang -= 10000;
                        if (weiwang < 0) weiwang = 0;
                        bunch_fame[bunch] = weiwang;
                        
                        room->delete("bunch");
                        npc->delete("bunch");
                        room->save();
                        save();
                        continue;
                }         
                        
                zhongcheng = room->query("bunch/zhongcheng");
                if (! intp(zhongcheng) || zhongcheng < 10) 
                        zhongcheng = 10;
                if (zhongcheng > npc->query("bunch/max_zhongcheng"))
                        zhongcheng = npc->query("bunch/max_zhongcheng");         
                zhongcheng -= random(2);
                npc->set("bunch/zhongcheng", zhongcheng < 10 ? 10 : zhongcheng);
                room->set("bunch/zhongcheng", zhongcheng < 10 ? 10 : zhongcheng);
                
                if (! room->query("area/ziyuan"))
                {
                        room->save();
                        continue;
                }

                jizhi = room->query("bunch/jizhi");
                kaifa = room->query("bunch/kaifa");
                jizhi += random(2);
                kaifa -= random(2);
                
                area_money = room->query("bunch/money");
                if (! intp(area_money)) area_money = 0;
                if (! intp(jizhi) || jizhi < 6) jizhi = 6;
                if (jizhi > 60) jizhi = 60;
                if (! intp(kaifa) || kaifa < 20) kaifa = 20;
                room->set("bunch/kaifa", kaifa);
                room->set("bunch/jizhi", jizhi);              
                
                if (area_fame[area] == bad_area)   
                {
                        if (jizhi && kaifa < 80)    
                        {
                                bad_money = kaifa * jizhi * 1000;
                                area_money -= bad_money;
                        } else
                        { // 开发度高的，一般来说自然灾害影响不大
                                bad_area = ""; 
                                area_money += (kaifa / 2) * (jizhi / 2) * 100;
                        }
                } else
                        area_money += (kaifa / 2) * (jizhi / 2) * 200;
                
                room->set("bunch/money", area_money);
                
                rest_money = area_money - 1000000;  
                if (rest_money < 0) rest_money = 0;
                
                room->add("bunch/money", -rest_money);
                room->set("bunch/last_money", rest_money);                 
                
                room->save();       
                data[bunch] += rest_money;
        }         
        
        all_bunch = keys(bunch_fame);       
        
        if (! arrayp(all_bunch) || sizeof(all_bunch) < 1)
                return;         
                
        lost = ({ });
        foreach (bunch in all_bunch)   
        {
                reset_eval_cost();
                
                npc_money = query(bunch + "/npc_money"); 
                set(bunch + "/last_npc_money", npc_money);
                set(bunch + "/npc_money", 0);  
                
                set(bunch + "/last_area_money", data[bunch]); 
                
                data[bunch] += npc_money; 
                if (data[bunch] < 0) data[bunch] = 0;
                
                data[bunch] /= 2;
                
                if (query(bunch + "/money") < 2000000000)                
                        add(bunch + "/money", data[bunch]);
                        
                cost = 10000 * (sizeof(query(bunch + "/npc")) + 
                                sizeof(query(bunch + "/area")) + 
                                sizeof(query(bunch + "/member")));
                          
                add(bunch + "/money", -cost);
                
                data[bunch] /= 2;
                
                add(bunch + "/bangzhu_money", data[bunch]);
                set(bunch + "/last_bangzhu_money", data[bunch]);
                
                if (query(bunch + "/money") < -100000000) 
                {
                        message("channel:rumor", HIW "【中华帮会】由于帮派资产长期严重亏损，帮会「" + 
                                                 bunch + "」无法维持日常开支，土崩瓦解了！\n" NOR, users()); 
                        lost += ({ bunch });
                        // bunch = 0;
                        map_delete(data, bunch);
                }
                                
                save();
        }
        
        // all_bunch -= ({ 0 });
        if (arrayp(lost) && sizeof(lost) > 0)
        {
                foreach (bunch_name in lost)
                        dismiss_bunch(bunch_name);
        }
        lost = ({ });

        if (! arrayp(users()) || ! sizeof(users())) return;
                
        // give all online player banghui's money
        item = keys(data);
        
        for (i = 0; i < sizeof(item); i++)     
        {
                reset_eval_cost();
                
                count = 0;
                
                foreach (player in users())
                {
                        reset_eval_cost();
                        
                        if (! playerp(player)) continue;
                        
                        if (! player->query("bunch/bunch_name") ||
                            player->query("bunch/bunch_name") != item[i])
                                continue;
                        
                        count++;
                }
                               
                if (count == 0) count = 1;
                data[item[i]] /= count;
        }
        
        foreach (player in users())
        {
                reset_eval_cost();
                
                if (! playerp(player)) continue;
                
                if (! player->query("bunch/bunch_name")) continue;
                
                if (! data[player->query("bunch/bunch_name")]) continue;
                
                tell_object(player, HIG "帮派「" + player->query("bunch/bunch_name") + "」" + "发饷，你的存款增加了" +
                                    MONEY_D->money_str(data[player->query("bunch/bunch_name")]) + "！\n" NOR);
                
                if ((int)player->query("balance") > 2000000000)  
                        tell_object(player, RED "你在钱庄的钱已达到二十万两黄金，快点花吧！\n" NOR);
                else                
                        player->add("balance", data[player->query("bunch/bunch_name")]);
        }
        
        if (bad_area == "")     
                message("channel:rumor", HIM "【帮派盘点】某人：各地盘收入良好，请各位帮主速来钱庄转帐！\n" NOR, 
                        users());
        else 
        {
                str = bad_weather[random(sizeof(bad_weather))];
                str = replace_string(str, "$N", bad_area);
                
                message("channel:rumor", HIM "【帮派盘点】某人：" + str +
                                         MONEY_D->money_str(bad_money) + "！\n" NOR, users());
        }                
}       

public mixed query_area_fame(mixed ob)
{
        string *all_area;

        if (objectp(ob) || stringp(ob))
                return BUNCH_D->query_areas(ob);

        all_area = keys(area_fame) - ({ 0 });   
        
        return all_area;
}

// 返回帮派声望：如果参数为空，返回mapping类型，包含了所有同
// 盟的声望；如果参数是人物， 则返回该人物所在的那个帮派的声
// 望；如果参数是帮派，则返回该帮派的声望。
public mixed query_bunch_fame(mixed ob)
{
        string fname;

        if (stringp(ob))
                return bunch_fame[ob];

        if (objectp(ob))
        {
                if (stringp(fname = ob->query("bunch/bunch_name")))
                        return bunch_fame[fname];
                else
                        return 0;
        }

        return bunch_fame;
}

// 查询昨天的帮派名望信息
public mapping query_all_last_bunch_fame()
{
        if (! mapp(last_bunch_fame))
                // 没有昨天的信息，返回今天的
                return bunch_fame;

        // 返回昨天的帮派声望信息
        return last_bunch_fame;
}

// 查询帮派的仇恨信息：输入的ob可以是帮派的名字，也可是
// 中的人物。
public mapping query_bunch_hatred(mixed ob)
{
        mapping hatred;
        string fname;

        if (objectp(ob))
                fname = ob->query("bunch/bunch_name");
        else
        if (stringp(ob))
                fname = ob;
        else
                fname = 0;

        if (! stringp(fname) || ! mapp(hatred = query(fname + "/hatred")))
                return 0;

        return hatred;
}

// 变化帮派声望：输入的ob可以是帮派的名字，也可是帮派中的人物。
public void add_bunch_fame(mixed ob, int n)
{
        int new_fame;
        string fname;

        if (objectp(ob))
                fname = ob->query("bunch/bunch_name");
        else
        if (stringp(ob))
                fname = ob;
        else
                fname = 0;

        if (! stringp(fname) || undefinedp(query(fname + "/member")))
                return;

        // 计算新的帮派声望
        new_fame = bunch_fame[fname] + n;
        if (new_fame < 0) new_fame = 0;
        if (new_fame > MAX_BUNCH_FAME)
                new_fame = MAX_BUNCH_FAME;
        bunch_fame[fname] = new_fame;
}

// 帮派间仇杀
public void bunch_kill(object killer, object victim)
{
        int kexp;
        int vexp;
        string vfam;
        string kfam;
        string kid;
        int fame_delta;
        mapping hatred;
        mixed *d;

        // 只有玩家之间的仇杀才计算在内
        if (! objectp(killer) || ! playerp(killer) ||
            ! objectp(victim) || ! playerp(victim))
                return;

        // 巫师之间的比划可不能算数
        if (wizardp(killer) || wizardp(victim))
                return;

        // 查看这两个玩家所处的帮派
        kfam = killer->query("bunch/bunch_name");
        vfam = victim->query("bunch/bunch_name");

        if (! stringp(kfam) && ! stringp(vfam))
                // 都不在帮派内，不必继续了
                return;

        kexp = killer->query("combat_exp");
        vexp = victim->query("combat_exp");

        if (kfam == vfam)
        {
                if (! killer->is_killing(victim->query("id")))
                        // 失手所杀，不予理会
                        return;

                // 帮派内残杀？不与理会，直接扣除帮派和个人1/10威望。
                add_bunch_fame(kfam, -bunch_fame[kfam] / 10);
                killer->add("weiwang", -killer->query("weiwang") / 10);
                return;
        }

        if (kexp < vexp * 3 && vexp >= 100000)
        {
                // 杀手的经验不是远远的大于对方，并且被杀的
                // 人有一定的经验，这将导致帮派声望的降低。
                fame_delta = vexp + killer->query("score") * 2 +
                             killer->query("weiwang") * 10;
                fame_delta /= 1000;
        } else
                // 对手经验太少，或是差距太大，不影响声望
                fame_delta = 0;

        // 查看杀手所在的帮派是否仇恨死者：如果仇恨，则能够
        // 带动声望的变化。
        if (stringp(kfam))
        {
                string path;

                path = kfam + "/hatred/"+ victim->query("id");
                d = query(path);
                if (arrayp(d) && sizeof(d) >= 2 && intp(d[1]))
                {
                        // 仇恨死者，帮派获得额外的声望
                        if (d[1] > 2000)
                                CHANNEL_D->do_channel(this_object(), "rumor",
                                        "听说" + killer->name(1) + "击毙了" +
                                        victim->name(1) + "，为" +
                                        kfam + "出了一口恶气。");
                        fame_delta += d[1] / 3;

                        // 对此人的仇恨降低(降低的要比增加的要多)，
                        // 具体可以看fame_delta 与仇恨度公司的差异
                        d[1] -= fame_delta;
                        if (d[1] <= 0)
                                delete(path);
                        else
                                set(path, d);
                }
        }

        // 调整两个帮派的声望
        add_bunch_fame(killer,  fame_delta);
        add_bunch_fame(victim, -fame_delta);

        // 统计该杀手对本门的残害程度
        if (! stringp(vfam))
                return;

        // 仇恨程度和声望的变化都是在一个数量级上(K经验)，但
        // 是仇恨程度低于声望的变化。
        vexp = vexp / 1000 + 1;
        if (vexp > 5000)
                vexp = (vexp - 5000) / 16 + 2000;
        else
        if (vexp > 1000)
                vexp = (vexp - 1000) / 4 + 1000;

        kid = killer->query("id");
        if (! mapp(hatred = query(vfam + "/hatred")))
        {
                set(vfam + "/hatred/" + kid,
                    ({ killer->name(), vexp }));
                return;
        }

        // 每个帮派最多记录若干个仇人
        if (! undefinedp(d = hatred[kid]))
        {
                if (! arrayp(d) || sizeof(d) < 2 ||
                    ! intp(d[1]) || ! stringp(d[0]))
                {
                        // 这个ID的数据出了故障
                        d = 0;
                }
        } else
        if (sizeof(hatred) >= MAX_HATRED_PERSON)
        {
                string *ids;
                int i;

                // 过滤去掉一些人，为什么不去掉一个？这是为
                // 了防止过滤频繁的进行过滤操作。
                ids = sort_array(keys(hatred),
                                 (: sort_hatred :), hatred);
                for (i = 0; i < sizeof(ids) && i < HATREDP_REMOVED; i++)
                        map_delete(hatred, ids[i]);
        }

        if (! arrayp(d))
                d = ({ killer->name(1), vexp });
        else
        {
                d[0] = killer->name(1);
                d[1] += vexp;
        }

        // 记录这个人的信息
        hatred[kid] = d;
}

// 去掉所有帮派对某个人的仇恨信息
public void remove_hatred(string id)
{
        mapping dbase, bunch, hatred;
        string fam;

        if (! mapp(dbase = query_entire_dbase()))
                // 现在还没有仇恨信息
                return;

        // 查阅所有的
        foreach (fam in keys(dbase))
        {
                reset_eval_cost();

                if (! mapp(bunch = dbase[fam]))
                        continue;

                if (mapp(hatred = bunch["hatred"]))
                        // 去掉该帮派对某人的仇恨信息
                        map_delete(hatred, id);

                if (! mapp(hatred) || sizeof(hatred) < 1)
                        // 这个帮派已经没有仇恨信息
                        map_delete(bunch, "hatred");
        }

        save();
}


// 查看是否可以创建这个
public mixed valid_new_bunch(string fname)
{
        if (query(fname + "/member"))
                return "人家早就有叫这个的啦，你就别凑热闹了。\n";

        if (! undefinedp(FAMILY_D->query_family_fame(fname))
                || ! undefinedp(LEAGUE_D->query_league_fame(fname)))
                return "江湖赏已经有" + fname + "了，你还想做什么？\n";

        return 0;
}

// 创建 U
public void create_bunch(string fname, int base, object *obs)
{
        mapping bunch;
        mapping data;
        string *channels;
        object ob;

        bunch_fame[fname] = base;
        if (! mapp(last_bunch_fame))
                last_bunch_fame = ([ fname : base ]);
        else
                last_bunch_fame[fname] = base;

        data = ([ "member" : obs->query("id"),    
                  "money"  : 100000000,                 
                  "time"   : time() ]);                     

        foreach (ob in obs)
        {
                bunch = ([ "time"  : time(),
                           "bunch_name"  : fname ]);
                if (ob->is_team_leader())
                {
                        data += ([ "bangzhu" : ob->query("id") ]);
                        bunch += ([ "level" : 9 ]);
                        bunch += ([ "title" : "帮主" ]);
                } else
                {
                        bunch += ([ "level" : 1 ]);
                        bunch += ([ "title" : "帮众" ]);                        
                } 
                
                ob->set("bunch", bunch);
                        
                channels = ob->query("channels");
                if (! arrayp(channels) || ! sizeof(channels))
                        channels = ({ "party" });
                else
                if (member_array("bunch", channels) == -1)
                        channels += ({ "party" });

                ob->set("channels", channels);
                ob->set("bunch", bunch);
                ob->save();
        }
        
        set(fname, data);
	save();
}

// 解散 U
public void dismiss_bunch(string fname)
{
        string *ids;
        string id;
        object ob, npc;

        // 清除名望信息
        map_delete(bunch_fame, fname);
        if (mapp(last_bunch_fame)) map_delete(last_bunch_fame, fname);

        // 清除帮派中的所有玩家的相关信息
        ids = query(fname + "/member");

        if (arrayp(ids))
        {
                // 帮派中还有玩家，清除他们的信息
                foreach (id in ids)
                {
                        // 处理中
                        reset_eval_cost();
                        UPDATE_D->clear_user_data(id, "bunch");
                }
        }

        ids = query(fname + "/area");
        
        if (arrayp(ids))
        {
                // 帮派中还有地盘，清除他们的信息
                foreach (id in ids)
                {
                        // 处理中
                        reset_eval_cost();
                        ob = get_object(id);
                        if (! ob) continue;

                        if (stringp(ob->query("bunch/npc_id")) &&
                            objectp(npc = present(ob->query("bunch/npc_id"), ob)))
                                npc->delete("bunch/bunch_name");
                                            
                        ob->delete("bunch");
                        ob->save();                       
                }
        }        
        
        // 清除帮派的所有信息
        delete(fname);
}

// 查询帮派中的弟兄 U
public string *query_members(mixed ob)
{
        string *member;
        string fname;

        if (objectp(ob))
                fname = ob->query("bunch/bunch_name");
        else
        if (stringp(ob))
                fname = ob;
        else
                fname = 0;

        if (! stringp(fname) || ! arrayp(member = query(fname + "/member")))
                return 0;

        return member;
}

// 查询帮派中的地盘 U
public string *query_areas(mixed ob)
{
        string *area;
        string fname;

        if (objectp(ob))
                fname = ob->query("bunch/bunch_name");
        else
        if (stringp(ob))
                fname = ob;
        else
                fname = 0;

        if (! stringp(fname) || ! arrayp(area = query(fname + "/area")))
                return 0;

        return area;
}

// 查询帮派中的ＮＰＣ U
public string *query_npcs(mixed ob)
{
        string *npc;
        string fname;

        if (objectp(ob))
                fname = ob->query("bunch/bunch_name");
        else
        if (stringp(ob))
                fname = ob;
        else
                fname = 0;

        if (! stringp(fname) || ! arrayp(npc = query(fname + "/npc")))
                return 0;

        return npc;
}

// 从帮派中去掉一个人 U
public varargs void remove_member_from_bunch(mixed fname, string id)
{
        mapping bunch;
        string *member;    
        string *areas;
        string area;
        object room, npc;
        
        if (objectp(fname))
        {
                // fname is user object
                id = fname->query("id");
                if (! stringp(fname = fname->query("bunch/bunch_name")))
                        return;
        } else
        if (! stringp(fname))
                // or fname must be a bunch name
                return;

        if (! mapp(bunch = query(fname)) ||
            ! arrayp(member = bunch["member"]))
                // no such bunch or no member in the 
                return 0;

        member -= ({ id, 0 });
        if (sizeof(member) < 1)
        {
                CHANNEL_D->do_channel(this_object(), "rumor",
                        "听说" + fname + "人才凋零，昔日帮众尽皆散去，从此江湖再无此帮派了。");
                
                map_delete(bunch_fame, fname);
                if (mapp(last_bunch_fame)) map_delete(last_bunch_fame, fname);

                areas = query(fname + "/area");
        
                if (arrayp(areas))
                {
                        // 帮派中还有地盘，清除他们的信息
                        foreach (area in areas)
                        {
                                // 处理中
                                reset_eval_cost();
                                room = get_object(area);
                                if (! room) continue;

                                if (stringp(room->query("bunch/npc_id")) &&
                                    objectp(npc = present(room->query("bunch/npc_id"), room)))
                                        npc->delete("bunch/bunch_name");
                                            
                                room->delete("bunch");
                                room->save();                       
                        }
                }        
                delete(fname);
        } else
                bunch["member"] = member;
}

// 在帮派中增加一个人 U
public void add_member_into_bunch(string fname, string id)
{
        string *member;

        if (! arrayp(member = query(fname + "/member")))
                return 0;

        if (member_array(id, member) != -1)
                return 0;

        member += ({ id });
        set(fname + "/member", member);
        save();
}

// 在帮派中去掉一个地盘 U
public void remove_area_into_bunch(string fname, string id, object ob)
{
        mapping bunch;
        string *area;        
        string *npc;
        string file;
        object room;
                
        if (! mapp(bunch = query(fname)) ||
            ! arrayp(area = bunch["area"]))
                // no such bunch or no area in the 
                return 0;              

        area -= ({ id, 0 });
        
        if (sizeof(area) < 1)
                delete(fname + "/area");
        else        
                bunch["area"] = area;                

        room = get_object(id);        
        if (stringp(room->query("bunch/npc_id")) &&
            room->query("bunch/npc_id") == ob->query("id"))
                ob->delete("bunch/bunch_name");
        
        file = base_name(ob);        
        if (room->query("bunch/npc_file") != file)
                file = room->query("bunch/npc_file");
                                        
        if (arrayp(npc = query(fname + "/npc")))
                npc -= ({ file, 0 });        
        bunch[npc] = npc; 

        message("channel:rumor",  HIM "【中华帮会】某人：" + room->short() +
                "宣布脱离帮会「" + fname + "」的控制！\n" NOR, users());
                        
        room->delete("bunch");
        room->save();
        save();
}

// 在帮派中增加一个地盘 U
public void add_area_into_bunch(string fname, string id, object ob)
{
        mapping bunch;
        string *area;        
        string *npc;
        string file;
        object room;

        if (! arrayp(area = query(fname + "/area")))
                area = ({ });
         
        if (member_array(id, area) != -1)
                return 0;
                
        area += ({ id });
        
        set(fname + "/area", area);
        
        room = get_object(id);
        
        area_fame[id] = room->short(); // 地盘数据 file : short

        file = base_name(ob);
 
        bunch = ([ "bunch_name" : fname,
                   "npc_id"     : ob->query("id"),  
                   "npc_name"   : ob->query("name"),
                   "npc_file"   : file,
                   "zhongcheng" : 10,
                   "money"      : 0 ]);
        room->delete("bunch");                
        room->set("bunch", bunch);
        room->save();
        ob->set("bunch/bunch_name", fname);
        ob->set("bunch/zhongcheng", 10);

        if (! arrayp(npc = query(fname + "/npc")))
        {       
                npc = ({ file });
        } else
        {
                if (member_array(file, npc) == -1)
                        npc += ({ file });
        } 
                        
        set(fname + "/npc", npc);

        message("channel:rumor", HIM "【中华帮会】某人：" + ob->query("name") +
                "加入帮会「" + fname + "」！\n" NOR, users());
                
        message("channel:rumor", HIM "【中华帮会】某人：" + room->short() +
                "被帮会「" + fname + "」吞并！\n" NOR, users());
                                
        save();
}

// 在帮派中变动其他参数，例如帮派资产、帮主等。
public void add_info_into_bunch(string fname, string info, mixed n)
{
        if (! mapp(query(fname)))
                return;
                
        if (intp(n))
                add(fname + "/" + info, n);
        else
        if (stringp(n))
                set(fname + "/" + info, n);
}

// 排序：升序
private int sort_hatred(string id1, string id2, mapping hatred)
{
        mixed *d1, *d2;

        if (! arrayp(d1 = hatred[id1]) || sizeof(d1) < 2 || ! intp(d1[1]))
                return -1;

        if (! arrayp(d2 = hatred[id2]) || sizeof(d2) < 2 || ! intp(d2[1]))
                return 1;

        return d1[1] - d2[1];
}

public string query_save_file() { return DATA_DIR "bunchd"; }
