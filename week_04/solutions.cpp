#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>

class Hero;
class CardModifier;
class Card;

class CardModifier {
	enum class Type { Additive, Multiplicative };

	union Value {
		int	  additiveValue;
		float multiplicativeValue;
	};

	Type  type;
	Value value;

   public:
	CardModifier &setAdditive(int val);
	CardModifier &setMultiplicative(float val);
	int			  apply(int baseValue) const;
};

class Card {
	int			 damage;
	int			 defense;
	CardModifier modifier;

   public:
	Card() : damage(0), defense(0), modifier(CardModifier().setAdditive(0)) {}
	Card(int dmg, int def, const CardModifier &mod) : damage(dmg), defense(def), modifier(mod) {}

	void play(Hero &player, Hero &target);
	void play(Hero &player, Hero *targets, int targetCount);
};

class Hero {
   public:
	enum class Type { Aggressive, Passive, Merchant, Player };
	using PlayerModifier = std::function<int(int)>;

	Hero(Type t, const char *n, int hp);

	void takeDamage(int dmg);
	void increaseDefense(int def);
	void addCard(const Card &card);

	const PlayerModifier *getModifiers() const { return modifiers; }
	int					  getModifierCount() const { return modifierCount; }
	int					  getHealth() const { return health; }
	int					  getDefense() const { return defense; }

   private:
	Type type;	   /// за сега не се ползва за нищо
	char name[32];
	int	 health;
	int	 defense;
	Card cards[16];
	int	 cardCount;

	PlayerModifier modifiers[4];
	int			   modifierCount;
};

CardModifier &CardModifier::setAdditive(int val) {
	type				= Type::Additive;
	value.additiveValue = val;
	return *this;
}

CardModifier &CardModifier::setMultiplicative(float val) {
	type					  = Type::Multiplicative;
	value.multiplicativeValue = val;
	return *this;
}

int CardModifier::apply(int baseValue) const {
	if (type == Type::Additive) {
		return baseValue + value.additiveValue;
	} else if (type == Type::Multiplicative) {
		return baseValue * value.multiplicativeValue;
	}
	assert(false && "Invalid CardModifier type");
}

void Card::play(Hero &player, Hero &target) {
	int modifiedDamage = modifier.apply(damage);

	for (int i = 0; i < player.getModifierCount(); ++i) {
		modifiedDamage = player.getModifiers()[i](modifiedDamage);
	}

	target.takeDamage(modifiedDamage);
	player.increaseDefense(defense);
}
void Card::play(Hero &player, Hero *targets, int targetCount) {
	for (int i = 0; i < targetCount; ++i) {
		play(player, targets[i]);
	}
}

Hero::Hero(Hero::Type t, const char *n, int hp) : type(t), health(hp), defense(0), cardCount(0), modifierCount(0) {
	strncpy(name, n, sizeof(name) - 1);
	name[sizeof(name) - 1] = '\0';
}

void Hero::takeDamage(int dmg) {
	int effectiveDamage = std::max(0, dmg - defense);
	health -= effectiveDamage;
}

void Hero::increaseDefense(int def) { defense += def; }

void Hero::addCard(const Card &card) {
	if (cardCount < 16) {
		cards[cardCount++] = card;
	} else {
		std::cerr << "Cannot add more cards to hero " << name << std::endl;
		exit(1);
	}
}

int main() {
	Hero player(Hero::Type::Player, "Player1", 100);
	Hero enemy(Hero::Type::Aggressive, "Enemy1", 50);

	CardModifier mod = CardModifier().setAdditive(5);

	Card card(20, 5, mod);
	player.addCard(card);

	card.play(player, enemy);

	std::cout << "Enemy health after attack: " << enemy.getHealth() << std::endl;
	std::cout << "Enemy defense after attack: " << enemy.getDefense() << std::endl;

	std::cout << "Player health after attack: " << player.getHealth() << std::endl;
	std::cout << "Player defense after attack: " << player.getDefense() << std::endl;

	return 0;
}
