/*
** Written by MtgCore
** Rewritten by Poszer & Talamortis https://github.com/poszer/ & https://github.com/talamortis/
** AzerothCore 2019 http://www.azerothcore.org/
** Cleaned and made into a module by Micrah https://github.com/milestorme/
*/

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

class Npc_Services : public CreatureScript
{
public:
        Npc_Services() : CreatureScript("Npc_Services") { }

        bool OnGossipHello(Player *player, Creature *creature)
        {
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Spell_Nature_Regenerate:40:40:-18|t Восстановить здоровье и ману", GOSSIP_SENDER_MAIN, 1);			// Restore Health and Mana
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_BG_winAB_underXminutes:40:40:-18|t Обновить все подземелья", GOSSIP_SENDER_MAIN, 2);	// Reset Instances
		AddGossipItemFor(player, 10, "|TInterface\\icons\\SPELL_HOLY_BORROWEDTIME:40:40:-18|t Выполнить перезарядку умений", GOSSIP_SENDER_MAIN, 3);				// Reset Cooldowns
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_BG_AB_defendflags:40:40:-18|t Выйти из боя", GOSSIP_SENDER_MAIN, 4);			// Leave Combat
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Spell_Shadow_DeathScream:40:40:-18|t Устранить слабость", GOSSIP_SENDER_MAIN, 5);		    // Remove Sickness
		AddGossipItemFor(player, 10, "|TInterface\\icons\\INV_Hammer_24:40:40:-18|t Починить все предметы", GOSSIP_SENDER_MAIN, 6);							// Repair Items
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_WorldEvent_Lunar:40:40:-18|t Сбросить таланты", GOSSIP_SENDER_MAIN, 7);			// Reset Talents
		AddGossipItemFor(player, 10, "|TInterface/Icons/INV_Misc_Bag_07:40:40:-18|t Перейти в банк", GOSSIP_SENDER_MAIN, 8);                                   // Open Bank
		AddGossipItemFor(player, 10, "|TInterface/Icons/INV_Letter_11:40:40:-18|t Перейти в почту", GOSSIP_SENDER_MAIN, 9);                                     // Open Mailbox
        AddGossipItemFor(player, 10, "|TInterface/Icons/achievement_general:40:40:-18|t Выучить двойную специализацию", GOSSIP_SENDER_MAIN, 10);                // Learn Dualspec

		SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
	}
	bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{
		case 1: // Restore HP and MP
				CloseGossipMenuFor(player);
			if (player->IsInCombat())
			{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("Вы находитесь в бою!");
				return false;
				}
				else if(player->getPowerType() == POWER_MANA)
						player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

				player->SetHealth(player->GetMaxHealth());
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFЗдоровье и мана востановлены!");
				player->CastSpell(player, 31726);
				break;

	    case 2: // Reset Instances
				CloseGossipMenuFor(player);
				for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
				{
                    BoundInstancesMap const& m_boundInstances = sInstanceSaveMgr->PlayerGetBoundInstances(player->GetGUID(), Difficulty(i));
                    for (BoundInstancesMap::const_iterator itr = m_boundInstances.begin(); itr != m_boundInstances.end();)
                    {
                        if (itr->first != player->GetMapId())
                        {
                            sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), itr->first, Difficulty(i), true, player);
                            itr = m_boundInstances.begin();
                        }
                        else
                            ++itr;
                    }
				}

				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFВсе подземелья обновлены!");
				player->CastSpell(player, 59908);
                return true;
				break;

		case 3: // Reset Cooldowns
				CloseGossipMenuFor(player);
				if (player->IsInCombat())
				{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("Вы находитесь в бою!");
				return false;
				}

				player->RemoveAllSpellCooldown();
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFСброс перезарядки умений завершён!");
				player->CastSpell(player, 31726);
				break;

		case 4: // Leave Combat
				CloseGossipMenuFor(player);
				player->CombatStop();
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFПерсонаж вышел из боя!");
				player->CastSpell(player, 31726);
				break;

		case 5: // Remove Sickness
				CloseGossipMenuFor(player);
				if(player->HasAura(15007))
				player->RemoveAura(15007);
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFВы снова ощущаете себя полным сил!");
				player->CastSpell(player, 31726);
				break;

		case 6: // Repair Items
				CloseGossipMenuFor(player);
				player->DurabilityRepairAll(false, 0, false);
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFВсе предметы починены!");
				player->CastSpell(player, 31726);
				break;

	   case 7: // Reset Talents
				CloseGossipMenuFor(player);
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->GetSession()->SendNotification("|cffFFFF00Elgracia \n |cffFFFFFFВсе таланты сброшены!");
				player->CastSpell(player, 31726);
				break;

		case 8:	// BANK
				CloseGossipMenuFor(player);
				player->GetSession()->SendShowBank(player->GetGUID());
				break;

		case 9: // MAIL
				CloseGossipMenuFor(player);
				player->GetSession()->SendShowMailBox(player->GetGUID());
				break;

		case 10: // Learn Dual Talent Specialization
				CloseGossipMenuFor(player);
				if (player->IsInCombat())
				{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("Вы находитесь в бою!");
				return false;
				}

                                player->learnSpell(63644);
                                player->CastSpell(player, 31726);
                                player->CastSpell(player, 63624);
                                player->learnSpell(63645);
                                player->UpdateSpecCount(2);
                                player->GetSession()->SendNotification("|cffFFFF00NPC SERVICES \n |cffFFFFFFДвойная специализация выучена!");
				return true;
				break;

             }
                return true;
        }
};

void AddSC_Npc_Services()
{
   new Npc_Services();
}
