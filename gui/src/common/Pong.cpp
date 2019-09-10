#include <gui\common\Pong.hpp>

Pong::Pong()
{
	setTouchable(true);

	setWidth(800);
	setHeight(480);	
	
	Enemy.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
	Player.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
	Puck.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));

	Enemy.setHeight(100);
	Enemy.setWidth(20);
	Player.setHeight(100);
	Player.setWidth(20);
	Puck.setWidth(10);
	Puck.setHeight(10);

	txtEnemyScore.setWidth(200);
	txtEnemyScore.setHeight(100);
	txtEnemyScore.setXY((getWidth() / 2) - (txtEnemyScore.getWidth()), 100);
	txtEnemyScore.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
	txtEnemyScore.setTypedText(TypedText(T_WILDCARD));
	txtEnemyScore.setAlpha(125);
	enemyBuffer[0] = 0;
	txtEnemyScore.setWildcard(enemyBuffer);	

	txtPlayerScore.setWidth(200);
	txtPlayerScore.setHeight(100);
	txtPlayerScore.setXY((getWidth() / 2), 100);
	txtPlayerScore.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
	txtPlayerScore.setTypedText(TypedText(T_WILDCARD));
	txtPlayerScore.setAlpha(125);
	playerBuffer[0] = 0;
	txtPlayerScore.setWildcard(playerBuffer);	

	SetupGame();

	add(txtEnemyScore);
	add(txtPlayerScore);

	add(Enemy);
	add(Player);
	add(Puck);
}

Pong::~Pong()
{
}

void Pong::handleClickEvent(const ClickEvent & evt)
{
	if (evt.getType() == ClickEvent::PRESSED)
		isPlaying = true;

	if (evt.getType() == ClickEvent::RELEASED)
		return; // isPlaying = false;
}

void Pong::handleDragEvent(const DragEvent & evt)
{		

	//Update Player Location
	int y = evt.getNewY() - (Player.getHeight() / 2);
	int maxHeight = getHeight() - Player.getHeight();

	if (y < 0) y = 0;
	if (y > maxHeight) y = maxHeight;		

	Player.setY(y);

	invalidate();
}

void Pong::SetupGame()
{
	//Reset Score if too High
	if (playerScore > 99 || enemyScore > 99)
	{
		playerScore = 0;
		enemyScore = 0;
	}

	//Pause until tap
	isPlaying = false;


	//Reset to default positions
	int centerY = getHeight() / 2;
	int centerX = getWidth() / 2;
	
	Enemy.setXY(padding, centerY - (Enemy.getHeight() / 2));	
	Player.setXY(getWidth() - Player.getWidth() - padding, centerY - (Enemy.getHeight() / 2));
	Puck.setXY(centerX - (Puck.getWidth() / 2), centerY - (Puck.getHeight() / 2));	
		
	//Randomise Puck start direction
	puckDirection = std::rand() % 90;
	puckDirection = AddDegrees(puckDirection, -45);

	//Update the score
	UpdateScore();

	invalidate();	
}

void Pong::UpdateScore()
{
	Unicode::snprintf(playerBuffer, PLAYER_SIZE, "%d", playerScore);
	Unicode::snprintf(enemyBuffer, ENEMY_SIZE, "%d", enemyScore);	
}

double Pong::AddDegrees(double degrees, double add)
{	
	degrees += add;
	if (degrees > 359)	
		degrees -= 360;
	if (degrees < 0)
		degrees += 360;	

	//isPlaying = false;
	return degrees;
}

bool Pong::Collides(int x, int y, int oWidth, int oHeight, int xTwo, int yTwo, int oTwoWidth, int oTwoHeight)
{
	if (x + oWidth < xTwo || x > xTwo + oTwoWidth) return false;
	if (y + oHeight < yTwo || y > yTwo + oTwoHeight) return false;

	return true;
}

void Pong::handleTickEvent()
{	
	if (!isPlaying)
		return;

	//Workout new puck location
	double puckX = Puck.getX();
	double puckY = Puck.getY();	
	
	//Move puck in specified direction
	double angleRadians = (M_PI * puckDirection) / 180;
	puckX += (speed * (float)std::cos(angleRadians));
	puckY += (speed * (float)std::sin(angleRadians));	

	//If the pucks about to go off the top or bottom
	if (puckY > (getHeight() - Puck.getHeight()) || puckY < 0)
	{
		if (puckY < 0)
			puckY = 0;
		else
			puckY = getHeight() - Puck.getHeight();		

		int angleToY = 90 - puckDirection;			
		puckDirection = AddDegrees(puckDirection, 180 + (angleToY * 2));								
	}

	//If the puck was missed
	if (puckX < 0)
	{		
		playerScore++;
		SetupGame();
		return;
	}
	if (puckX > getWidth() - Puck.getWidth())
	{		
		enemyScore++;
		SetupGame();
		return;
	}

	//If the puck hit the player paddle
	if (Collides(puckX, puckY, Puck.getWidth(), Puck.getHeight(), Player.getX(), Player.getY(), Player.getWidth(), Player.getHeight()))
	{
		//Bounce off paddle
		int angleToX = 180 - puckDirection;
		puckDirection = AddDegrees(puckDirection, 180 + (angleToX * 2));
		puckX = getWidth() - padding - Player.getWidth() - Puck.getWidth();		

		//Work out extra angle from positioning on paddle
		float distanceFromCenterOfPaddle = Player.getY() + (Player.getHeight() / 2) - Puck.getY();
		float percentageToHeight = distanceFromCenterOfPaddle / (Player.getHeight() / 2);
		puckDirection = AddDegrees(puckDirection, bounceModifier * percentageToHeight);

		//Restrict Puck Direction
		if (puckDirection < 105 && puckDirection > 75)
			puckDirection = 105;
		if (puckDirection < 285 && puckDirection > 260)
			puckDirection = 260;
	}

	//If the puck hit the enemy paddle
	if (Collides(puckX, puckY, Puck.getWidth(), Puck.getHeight(), Enemy.getX(), Enemy.getY(), Enemy.getWidth(), Enemy.getHeight()))
	{
		//Bounce off paddle
		int angleToY = 180 - puckDirection;
		puckDirection = AddDegrees(puckDirection, 180 + (angleToY * 2));
		puckX = padding + Enemy.getWidth();

		//Work out extra angle from positioning on paddle
		float distanceFromCenterOfPaddle = Enemy.getY() + (Enemy.getHeight() / 2) - Puck.getY();
		float percentageToHeight = distanceFromCenterOfPaddle / (Enemy.getHeight() / 2);
		puckDirection = AddDegrees(puckDirection, bounceModifier * percentageToHeight);

		//Restrict Puck Direction
		if (puckDirection < 105 && puckDirection > 75)
			puckDirection = 75;
		if (puckDirection < 285 && puckDirection > 260)
			puckDirection = 285;

	}

	//Update Puck Location
	Puck.setX(puckX);
	Puck.setY(puckY);
	

	//Workout Enemy Location
	int enemyY = Enemy.getY();
	int centerEnemy = enemyY + (Enemy.getHeight() / 2);
	int centerPuck = Puck.getY() + (Puck.getHeight() / 2);
	
	if (centerEnemy > centerPuck)
	{
		//If distance is less than the speed then only move the required amount so that it doesn't judder about
		int distance = centerPuck - centerEnemy;
		if (std::abs((double)distance) < enemySpeed)		
			enemyY += enemySpeed;
		
		enemyY -= enemySpeed;
	}
	if (centerEnemy < centerPuck)
	{
		//If distance is less than the speed then only move the required amount so that it doesn't judder about
		int distance = centerPuck - centerEnemy;
		if (std::abs((double)distance) < enemySpeed)
			enemyY -= enemySpeed;

		enemyY += enemySpeed;
	}	


	//Restrict so it doesn't go off the screen
	int maxEnemyHeight = getHeight() - (Enemy.getHeight());
	if (enemyY < 0)
		enemyY = 0;
	if (enemyY > maxEnemyHeight)
		enemyY = maxEnemyHeight;

	//Set enemy location
	Enemy.setY(enemyY);

	//Update UI
	invalidate();
}
