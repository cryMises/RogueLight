# 🎮 RogueLight

A simple **C++ console card game** where you collect cards, build decks, and battle against a randomized enemy.

---

## 🚀 Features

* 🃏 Card collection system (Rogue & Hero)
* 🎲 Gacha-style shop with random rewards
* ⚔️ Turn-based battle system
* 💾 Save & load player data

---

## 🛠️ Built With

* C++
* STL (`vector`, `algorithm`, `random`)
* File handling (`fstream`)

---

## 📁 Files

```id="tree_simple"
main.cpp               # Game logic
globalDatabase.txt     # Card database
playerDatabase.txt     # Player cards (auto-created)
playerData.txt         # Player info (auto-created)
```

---

## ▶️ How to Run

### Compile

```bash id="compile_simple"
g++ main.cpp -o RogueLight
```

### Run

```bash id="run_simple"
./RogueLight
```

---

## 🎮 How to Play

1. Enter your name (first run)
2. Buy cards from the shop
3. Build your collection
4. Battle to earn money
5. Repeat

---

## ⚔️ Battle System

* 5 turns per player
* Choose a card each turn
* Decide:

  * Attack
  * Defense

Winner is based on total stats

---

## 📊 Card Format

```id="format_simple"
[Rarity] [Type] [Name] [Attack] [Defense]
```

Example:

```id="example_simple"
Common Rogue Shadowblade 3200 3800
```

---

## ⚠️ Notes

* Card names must be one word
* Enemy plays randomly
* Save files are created automatically

---

## 👤 Author

cryMises

---

## 📜 License

Licensed under the MIT License
