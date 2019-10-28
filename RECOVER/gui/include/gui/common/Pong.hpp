#ifndef PONG_HPP
#define PONG_HPP

#include <touchgfx\containers\Container.hpp>
#include <touchgfx\widgets\canvas\Canvas.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <ctgmath>
#include <cstdlib>
#include <cmath>

using namespace touchgfx;

class Pong : public Container
{
public:
	Pong();
	virtual ~Pong();

	virtual void handleTickEvent();

private:	

	virtual void handleClickEvent(const ClickEvent& evt);
	virtual void handleDragEvent(const DragEvent& evt);		

	void SetupGame();
	void UpdateScore();
	
	Box Enemy;
	Box Player;
	Box Puck;		

	const int padding = 20;
	const int speed = 8;
	const int enemySpeed = 4;
	const int bounceModifier = 55;
	bool isPlaying = false;

	int puckDirection;
	double AddDegrees(double degrees, double add);

	bool Collides(int x, int y, int oWidth, int oHeight, int xTwo, int yTwo, int oTwoWidth, int oTwoHeight);

	int enemyScore = 0;
	int playerScore = 0;

	TextAreaWithOneWildcard txtEnemyScore;
	TextAreaWithOneWildcard txtPlayerScore;	

	static const uint16_t ENEMY_SIZE = 4;
	touchgfx::Unicode::UnicodeChar enemyBuffer[ENEMY_SIZE];

	static const uint16_t PLAYER_SIZE = 4;
	touchgfx::Unicode::UnicodeChar playerBuffer[PLAYER_SIZE];


};

#endif