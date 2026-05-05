#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <limits>
#include <random>
#include <string>
#include <vector>
using namespace std;

void checkCin()
{
  if (!cin)
  {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}

struct cards
{
  string rarity;
  string type;
  string name;
  int attack;
  int defense;
};

struct cardRarity
{
  vector<cards> all;
  vector<cards> common;
  vector<cards> rare;
};

struct cardTypes
{
  cardRarity rogue;
  cardRarity hero;
};

struct playerInfo
{
  string playerName;
  int playerMoney;
};

cardTypes getData(const string &filename)
{
  vector<cards> cardDatabase;
  ifstream file(filename);
  cardTypes splitCardDatabase;
  string rarity;
  string type;
  string name;
  int attack, defense;
  if (!file)
  {
    cout << "Error opening file for reading.\n";
    return splitCardDatabase;
  }
  while (file >> rarity >> type >> name >> attack >> defense)
  {
    cardDatabase.push_back({rarity, type, name, attack, defense});
  }
  copy_if(cardDatabase.begin(), cardDatabase.end(),
          back_inserter(splitCardDatabase.rogue.all),
          [](const cards &a)
          { return a.type == "Rogue"; });
  copy_if(cardDatabase.begin(), cardDatabase.end(),
          back_inserter(splitCardDatabase.hero.all),
          [](const cards &a)
          { return a.type == "Hero"; });

  copy_if(splitCardDatabase.rogue.all.begin(),
          splitCardDatabase.rogue.all.end(),
          back_inserter(splitCardDatabase.rogue.common),
          [](const cards &a)
          { return a.rarity == "Common"; });
  copy_if(splitCardDatabase.rogue.all.begin(),
          splitCardDatabase.rogue.all.end(),
          back_inserter(splitCardDatabase.rogue.rare),
          [](const cards &a)
          { return a.rarity == "Rare"; });

  copy_if(splitCardDatabase.hero.all.begin(), splitCardDatabase.hero.all.end(),
          back_inserter(splitCardDatabase.hero.common),
          [](const cards &a)
          { return a.rarity == "Common"; });
  copy_if(splitCardDatabase.hero.all.begin(), splitCardDatabase.hero.all.end(),
          back_inserter(splitCardDatabase.hero.rare),
          [](const cards &a)
          { return a.rarity == "Rare"; });
  return splitCardDatabase;
}

void saveData(const string &filename, const cardTypes &data)
{
  ofstream file(filename);
  if (!file)
  {
    cout << "Error opening file for writing.\n";
    return;
  }
  for (const auto &card : data.rogue.all)
  {
    file << card.rarity << " " << card.type << " " << card.name << " "
         << card.attack << " " << card.defense << "\n";
  }
  for (const auto &card : data.hero.all)
  {
    file << card.rarity << " " << card.type << " " << card.name << " "
         << card.attack << " " << card.defense << "\n";
  }
  cout << "\nData Saved Successfully\n";
  return;
}

playerInfo getPlayerInfo(const string &filename)
{
  string playerName;
  int playerMoney;

  ifstream file(filename);
  if (!file)
  {
    cout << "Error opening file for reading.\n";
    return playerInfo{"", 0};
  }
  else
  {
    getline(file, playerName);
    file >> playerMoney;
    cout << "\nData Read Successfully\n";
    return playerInfo{playerName, playerMoney};
  }
  return playerInfo{"", 0};
};

void saveInfo(const string &filename, const playerInfo &info)
{
  ofstream file(filename);
  if (!file)
  {
    cout << "Error opening file for writing.\n";
    return;
  }
  file << info.playerName << "\n"
       << info.playerMoney << "\n";
  cout << "\nData Saved Successfully\n";
  return;
}

bool addCards(cardRarity &target, const cards &card)
{
  if (card.rarity == "Common")
  {
    for (const auto &a : target.common)
    {
      if (a.name == card.name)
      {
        cout << "\nDuplicate Card Detected, Rerolling\n";
        return false;
      }
    }
  }
  else
  {
    for (const auto &a : target.rare)
    {
      if (a.name == card.name)
      {
        cout << "\nDuplicate Card Detected, Rerolling\n";
        return false;
      }
    }
  }
  target.all.push_back(card);
  if (card.rarity == "Common")
  {
    target.common.push_back(card);
  }
  else
  {
    target.rare.push_back(card);
  }

  return true;
}

void cleanDeck(cardRarity &target)
{
  target.all.clear();
  target.common.clear();
  target.rare.clear();
}

void shuffleDeck(cardRarity &source, cardRarity &target, int amount, mt19937 &rng)
{
  vector<cards> temp = source.all;
  cleanDeck(target);
  shuffle(temp.begin(), temp.end(), rng);
  for (int i = 0; i < amount && i < temp.size(); i++)
  {
    addCards(target, temp[i]);
  }
  return;
}

void checkInventory(cardTypes &playerData)
{
  if (playerData.rogue.all.empty() && playerData.hero.all.empty())
  {
    cout << "\nYou have No Cards\n";
    return;
  }
  else
  {
    if (!playerData.rogue.all.empty())
    {
      if (!playerData.rogue.common.empty() && !playerData.rogue.rare.empty())
      {
        cout << left << setw(22) << "\nCommon Rogue Cards" << setw(7) << "Attack" << setw(11) << "Defense"
             << setw(21)
             << "Rare Rogue Cards" << setw(7) << "Attack" << setw(7) << "Defense\n";

        for (int i = 0; i < max(playerData.rogue.common.size(), playerData.rogue.rare.size()); i++)
        {
          if (i < playerData.rogue.common.size())
          {
            cout << left << setw(22) << playerData.rogue.common[i].name << setw(7) << playerData.rogue.common[i].attack << setw(10) << playerData.rogue.common[i].defense;
          }
          else
          {
            cout << left << setw(22) << " " << setw(7) << " " << setw(10) << " ";
          }
          if (i < playerData.rogue.rare.size())
          {
            cout << left << setw(22) << playerData.rogue.rare[i].name << setw(7) << playerData.rogue.rare[i].attack << setw(7) << playerData.rogue.rare[i].defense;
          }
          cout << "\n";
        }
      }
      else
      {
        if (!playerData.rogue.common.empty())
        {
          cout << "\nCommon Rogue Cards : \n";
          for (const auto &card : playerData.rogue.common)
          {
            cout << card.name << " Attack : " << card.attack
                 << " Defense : " << card.defense << "\n";
          }
        }
        if (!playerData.rogue.rare.empty())
        {
          cout << "\nRare Rogue Cards : \n";
          for (const auto &card : playerData.rogue.rare)
          {
            cout << card.name << " Attack : " << card.attack
                 << " Defense : " << card.defense << "\n";
          }
        }
      }
    }
    else
    {
      cout << "\nYou Have No Rogue Cards\n";
    }
    if (!playerData.hero.all.empty())
    {
      if (!playerData.hero.common.empty() && !playerData.hero.rare.empty())
      {
        cout << left << setw(22) << "\nCommon Hero Cards" << setw(7) << "Attack" << setw(11) << "Defense"
             << setw(21)
             << "Rare Hero Cards" << setw(7) << "Attack" << setw(7) << "Defense\n";

        for (int i = 0; i < max(playerData.hero.common.size(), playerData.hero.rare.size()); i++)
        {
          if (i < playerData.hero.common.size())
          {
            cout << left << setw(22) << playerData.hero.common[i].name << setw(7) << playerData.hero.common[i].attack << setw(10) << playerData.hero.common[i].defense;
          }
          else
          {
            cout << left << setw(22) << " " << setw(7) << " " << setw(10) << " ";
          }
          if (i < playerData.hero.rare.size())
          {
            cout << left << setw(22) << playerData.hero.rare[i].name << setw(7) << playerData.hero.rare[i].attack << setw(7) << playerData.hero.rare[i].defense;
          }
          cout << "\n";
        }
      }
      else
      {
        if (!playerData.hero.common.empty())
        {
          cout << "\nCommon Hero Cards : \n";
          for (const auto &card : playerData.hero.common)
          {
            cout << card.name << " Attack : " << card.attack
                 << " Defense : " << card.defense << "\n";
          }
        }
        if (!playerData.hero.rare.empty())
        {
          cout << "\nRare Hero Cards : \n";
          for (const auto &card : playerData.hero.rare)
          {
            cout << card.name << " Attack : " << card.attack
                 << " Defense : " << card.defense << "\n";
          }
        }
      }
    }
    else
    {
      cout << "\nYou Have No Hero Cards\n";
    }
  }
  return;
}

void battleInitialize(cardTypes &playerDeck, cardTypes &enemyDeck, cardTypes &playerData, cardTypes &enemyData, bool rogueMode, mt19937 &rng)
{
  cleanDeck(playerDeck.rogue);
  cleanDeck(playerDeck.hero);
  cleanDeck(enemyDeck.rogue);
  cleanDeck(enemyDeck.hero);
  if (rogueMode)
  {
    shuffleDeck(playerData.rogue, playerDeck.rogue, 10, rng);
    shuffleDeck(enemyData.hero, enemyDeck.hero, 10, rng);
  }
  else
  {
    shuffleDeck(playerData.hero, playerDeck.hero, 10, rng);
    shuffleDeck(enemyData.rogue, enemyDeck.rogue, 10, rng);
  }
  cout << "\nYour Randomized Deck : \n";
  checkInventory(playerDeck);
  cout << "\nEnemy Randomized Deck : \n";
  checkInventory(enemyDeck);
  return;
}

int battle(bool &rogueMode, cardTypes &playerDeck, cardTypes &enemyDeck, mt19937 &rng)
{
  int menu = 0;
  int index = 0;
  int point = 0;
  int playerAttack = 0, playerDefense = 0, enemyAttack = 0, enemyDefense = 0;
  if (rogueMode)
  {
    // Player Turn
    for (int i = 1; i <= 5; i++)
    {
      cout << "\nChoose A Card To Play\n";
      for (int i = 0; i < playerDeck.rogue.all.size(); i++)
      {
        cout << i + 1 << ". " << playerDeck.rogue.all[i].name << " Attack : " << playerDeck.rogue.all[i].attack << " Defense : " << playerDeck.rogue.all[i].defense << "\n";
      }
      do
      {
        cin >> index;
        checkCin();
        if (index < 1 || index > playerDeck.rogue.all.size())
        {
          cout << "\nInvalid Choice, Try Again\n";
          continue;
        }
        else
        {
          cout << "What Position Do You Want To Play Your Card ?\n1. Attack\n2. Defense\n";
          do
          {
            cin >> menu;
            checkCin();
            if (menu == 1)
            {
              playerAttack += playerDeck.rogue.all[index - 1].attack;
              cout << "\nYou Played " << playerDeck.rogue.all[index - 1].name << " In Attack Position\n";
            }
            else if (menu == 2)
            {
              playerDefense += playerDeck.rogue.all[index - 1].defense;
              cout << "\nYou Played " << playerDeck.rogue.all[index - 1].name << " In Defense Position\n";
            }
            else
            {
              cout << "\nInvalid Choice, Try Again\n";
              continue;
            }
            playerDeck.rogue.all.erase(playerDeck.rogue.all.begin() + index - 1);
          } while (menu != 1 && menu != 2);
        }
      } while (index < 1 || index > playerDeck.rogue.all.size());
    }
    // Bot Turn
    for (int i = 1; i <= 5; i++)
    {
      uniform_int_distribution<int> enemyCardRand(0, enemyDeck.hero.all.size() - 1);
      index = enemyCardRand(rng);
      if (enemyDeck.hero.all[index].attack > enemyDeck.hero.all[index].defense)
      {
        enemyAttack += enemyDeck.hero.all[index].attack;
        cout << "\nEnemy Played " << enemyDeck.hero.all[index].name << " In Attack Position\n";
      }
      else
      {
        enemyDefense += enemyDeck.hero.all[index].defense;
        cout << "\nEnemy Played " << enemyDeck.hero.all[index].name << " In Defense Position\n";
      }
      enemyDeck.hero.all.erase(enemyDeck.hero.all.begin() + index);
    }
  }
  else
  {
    // Player Turn
    for (int i = 1; i <= 5; i++)
    {
      cout << "\nChoose A Card To Play\n";
      for (int a = 0; a < playerDeck.hero.all.size(); a++)
      {
        cout << a + 1 << ". " << playerDeck.hero.all[a].name << " Attack : " << playerDeck.hero.all[a].attack << " Defense : " << playerDeck.hero.all[a].defense << "\n";
      }
      do
      {
        cin >> index;
        checkCin();
        if (index < 1 || index > playerDeck.hero.all.size())
        {
          cout << "\nInvalid Choice, Try Again\n";
        }
        else
        {
          cout << "What Position Do You Want To Play Your Card ?\n1. Attack\n2. Defense\n";
          do
          {
            cin >> menu;
            checkCin();
            if (menu == 1)
            {
              playerAttack += playerDeck.hero.all[index - 1].attack;
              cout << "\nYou Played " << playerDeck.hero.all[index - 1].name << " In Attack Position\n";
            }
            else if (menu == 2)
            {
              playerDefense += playerDeck.hero.all[index - 1].defense;
              cout << "\nYou Played " << playerDeck.hero.all[index - 1].name << " In Defense Position\n";
            }
            else
            {
              cout << "\nInvalid Choice, Try Again\n";
              continue;
            }
            playerDeck.hero.all.erase(playerDeck.hero.all.begin() + index - 1);
          } while (menu != 1 && menu != 2);
        }
      } while (index < 1 || index > playerDeck.hero.all.size());
    }
    // Bot Turn
    for (int i = 1; i <= 5; i++)
    {
      uniform_int_distribution<int> enemyCardRand(0, enemyDeck.rogue.all.size() - 1);
      index = enemyCardRand(rng);
      if (enemyDeck.rogue.all[index].attack > enemyDeck.rogue.all[index].defense)
      {
        enemyAttack += enemyDeck.rogue.all[index].attack;
        cout << "\nEnemy Played " << enemyDeck.rogue.all[index].name << " In Attack Position\n";
      }
      else
      {
        enemyDefense += enemyDeck.rogue.all[index].defense;
        cout << "\nEnemy Played " << enemyDeck.rogue.all[index].name << " In Defense Position\n";
      }
      enemyDeck.rogue.all.erase(enemyDeck.rogue.all.begin() + index);
    }
  }
  // Battle Phase
  cout << "\nPlayer Total Attack : " << playerAttack << " Player Total Defense : " << playerDefense << "\n";
  cout << "Enemy Total Attack : " << enemyAttack << " Enemy TotalDefense : " << enemyDefense << "\n";
  if (playerAttack - enemyDefense > enemyAttack - playerDefense)
  {
    cout << "\nYou Win This Battle\n";
    point = 2;
  }
  else if (playerAttack - enemyDefense < enemyAttack - playerDefense)
  {
    cout << "\nYou Lose This Battle\n";
    point = 0;
  }
  else
  {
    cout << "\nThis Battle Is A Draw\n";
    point = 1;
  }
  return point;
}

int main()
{
  cardTypes cardDatabase = getData("globalDatabase.txt");
  cardTypes playerData = getData("playerDatabase.txt");
  playerInfo playerInfo = getPlayerInfo("playerData.txt");
  cardTypes playerDeck = {};
  cardTypes enemyDeck = {};
  bool rogueMode = true;
  int point = 0;
  mt19937 rng(time(NULL));
  int menu = 0;

  int rogueGacha = 0, heroGacha = 0;
  bool noDuplicate = false;
  if (!cardDatabase.rogue.all.empty() && !cardDatabase.hero.all.empty())
  {
    cout << "Data is Loaded, "
         << cardDatabase.hero.all.size() + cardDatabase.rogue.all.size()
         << " Cards Detected\n";
  }
  else
  {
    cout << "Data Is Empty\n";
  }
  if (playerInfo.playerName.empty())
  {
    cout << "\nEnter Your Name : ";
    getline(cin, playerInfo.playerName);
    playerInfo.playerMoney = 50000;
    cout << "\nYou have been given 50000 Money to Start Your Journey\n";
    saveInfo("playerData.txt", playerInfo);
  }
  while (menu != 4)
  {
    menu = 0;
    cout << "\nWelcome to RogueLight, " << playerInfo.playerName << "!\nMoney : " << playerInfo.playerMoney
         << "\n1. Random Battle\n2. Inventory\n3. "
            "Shop\n4. Exit\n";
    cin >> menu;
    checkCin();
    switch (menu)
    {
    case 1:
      if (playerData.rogue.all.size() >= 10 && playerData.hero.all.size() >= 10)
      {
        point = 0;
        cout << "\nWhat Mode Do You Want To Play ?\n1. Rogue Mode (Play With Rogue Cards)\n2. Hero Mode (Play With Hero Cards)\n3. Exit\n";
        cin >> menu;
        checkCin();
        if (menu == 1)
        {
          rogueMode = true;
          battleInitialize(playerDeck, enemyDeck, playerData, cardDatabase, rogueMode, rng);
          point = battle(rogueMode, playerDeck, enemyDeck, rng);
          playerInfo.playerMoney += point * 2500;
          cout << "\nYou Earned " << point * 2500 << " Money From This Battle\n";
        }
        else if (menu == 2)
        {
          rogueMode = false;
          battleInitialize(playerDeck, enemyDeck, playerData, cardDatabase, rogueMode, rng);
          point = battle(rogueMode, playerDeck, enemyDeck, rng);
          playerInfo.playerMoney += point * 2500;
          cout << "\nYou Earned " << point * 2500 << " Money From This Battle\n";
        }
        else
        {
          menu = 0;
          break;
        }
        saveInfo("playerData.txt", playerInfo);
      }
      else
      {
        cout << "\nYou Need At Least 10 Cards Of Each Type To Enter A Battle\n";
        break;
      }
      menu = 0;
      break;
    case 2:
      checkInventory(playerData);
      break;
    case 3:
      menu = 0;
      while (menu != 3)
      {
        cout << "\nWelcome to the Shop\n1. Common Pack (6000)\n2. Rare "
                "Pack(10000)\n3. Exit\n";
        cin >> menu;
        checkCin();
        switch (menu)
        {
        case 1:
          if (playerInfo.playerMoney >= 6000)
          {
            if (!cardDatabase.rogue.common.empty() &&
                !cardDatabase.hero.common.empty())
            {
              uniform_int_distribution<int> rogueRand(
                  0, cardDatabase.rogue.common.size() - 1);
              uniform_int_distribution<int> heroRand(
                  0, cardDatabase.hero.common.size() - 1);
              playerInfo.playerMoney -= 6000;
              if (playerData.rogue.common.size() <
                  cardDatabase.rogue.common.size())
              {
                do
                {
                  rogueGacha = rogueRand(rng);
                  noDuplicate = addCards(playerData.rogue,
                                         cardDatabase.rogue.common[rogueGacha]);
                } while (!noDuplicate);
                cout << "\nYou Got : \n"
                     << cardDatabase.rogue.common[rogueGacha].name
                     << " Attack : "
                     << cardDatabase.rogue.common[rogueGacha].attack
                     << " Defense : "
                     << cardDatabase.rogue.common[rogueGacha].defense << "\n";
              }
              else
              {
                cout << "\nCommon Rogue Card Is Complete, Refunding Half Of "
                        "Price\n";
                playerInfo.playerMoney += 3000;
              }
              if (playerData.hero.common.size() <
                  cardDatabase.hero.common.size())
              {
                do
                {
                  heroGacha = heroRand(rng);
                  noDuplicate = addCards(playerData.hero,
                                         cardDatabase.hero.common[heroGacha]);
                } while (!noDuplicate);
                cout << "\n"
                     << cardDatabase.hero.common[heroGacha].name << " Attack : "
                     << cardDatabase.hero.common[heroGacha].attack
                     << " Defense : "
                     << cardDatabase.hero.common[heroGacha].defense << "\n";
              }
              else
              {
                cout << "\nCommon Hero Card Is Complete, Refunding Half Of "
                        "Price\n";
                playerInfo.playerMoney += 3000;
              }
            }
            else
            {
              cout << "\nCommon Card Database Is Empty\n";
              break;
            }
          }
          else
          {
            cout << "\nNot Enough Money\n";
            break;
          }
          saveData("playerDatabase.txt", playerData);
          saveInfo("playerData.txt", playerInfo);
          break;
        case 2:
          if (playerInfo.playerMoney >= 10000)
          {
            if (!cardDatabase.rogue.rare.empty() && !cardDatabase.hero.rare.empty())
            {
              uniform_int_distribution<int> rogueRand(0, cardDatabase.rogue.rare.size() - 1);
              uniform_int_distribution<int> heroRand(0, cardDatabase.hero.rare.size() - 1);
              playerInfo.playerMoney -= 10000;
              if (playerData.rogue.rare.size() < cardDatabase.rogue.rare.size())
              {
                do
                {
                  rogueGacha = rogueRand(rng);
                  noDuplicate = addCards(playerData.rogue, cardDatabase.rogue.rare[rogueGacha]);
                } while (!noDuplicate);
                cout << "\nYou Got : \n"
                     << cardDatabase.rogue.rare[rogueGacha].name << " Attack : " << cardDatabase.rogue.rare[rogueGacha].attack << " Defense : " << cardDatabase.rogue.rare[rogueGacha].defense << "\n";
              }
              else
              {
                cout << "\nRare Rogue Card Is Complete, Refunding Half Of "
                        "Price\n";
                playerInfo.playerMoney += 5000;
              }
              if (playerData.hero.rare.size() < cardDatabase.hero.rare.size())
              {
                do
                {
                  heroGacha = heroRand(rng);
                  noDuplicate = addCards(playerData.hero, cardDatabase.hero.rare[heroGacha]);
                } while (!noDuplicate);
                cout << "\n"
                     << cardDatabase.hero.rare[heroGacha].name << " Attack : " << cardDatabase.hero.rare[heroGacha].attack << " Defense : " << cardDatabase.hero.rare[heroGacha].defense << "\n";
              }
              else
              {
                cout << "\nRare Hero Card Is Complete, Refunding Half Of "
                        "Price\n";
                playerInfo.playerMoney += 5000;
              }
            }
            else
            {
              cout << "\nRare Card Database Is Empty\n";
              break;
            }
          }
          else
          {
            cout << "\nNot Enough Money\n";
            break;
          }
          saveData("playerDatabase.txt", playerData);
          saveInfo("playerData.txt", playerInfo);
          break;
        default:
          break;
        }
      }
      break;
    default:
      break;
    }
  }
  return 0;
}
