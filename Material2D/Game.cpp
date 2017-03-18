#define _USE_MATH_DEFINES
#include <cmath>

#include "Game.h"
#include "LuaWrapper/LuaWrapper.h"

Game::~Game()
{
	Destroy();
}

void Game::Destroy()
{
	m_levelManager.Clear();
    LuaWrapper::GetInstance().Clear();
    [m_scene removeAllChildren];
	//m_beatSound.resetBuffer();
}

void Game::Start(SKScene* scene)
{
	m_gameState = GameState::Initializing;
    m_scene = scene;
	m_levelManager.LoadLevel("Scripts/Levels/Tutorial.lua", scene);
	m_columns = m_levelManager.GetColumns();
	m_rows = m_levelManager.GetRows();

	m_beatPause = m_levelManager.GetBeatPause();
	m_beatBuffer = m_levelManager.GetBeatBuffer();

	m_on = true;
	m_gameState = GameState::Playing;
}

void Game::SetScreenSize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

void Game::SetControlScheme(ControlScheme scheme)
{
    m_controlScheme = scheme;
}

void Game::SetMaterial(MaterialType type)
{
    m_levelManager.SetMaterial(type);
}

void Game::Update(double dt)
{
    int frames = 0;
    
    m_remainingTime += dt - m_lastTime;
    m_timeSinceBeat += dt - m_lastTime;
    m_lastTime = dt;
    
    while ((m_remainingTime > m_minTimestep) && (frames < m_maxFrames))
    {
        
        ProcessEvents();
        
        m_remainingTime -= m_minTimestep;
        
        frames++;
        
        if (m_gameState == GameState::Paused)
            continue;
        m_levelManager.UpdateRender();
    }
    
    if (m_gameState == GameState::Paused)
        return;
    
    if (m_timeSinceBeat >= m_beatPause)
    {
        m_timeSinceBeat = 0;
        m_levelManager.Update();
    }
}

void Game::AbilityUsed(int row, int column, bool tap, double angle)
{
    if (m_enableBeat && (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatPause - m_beatBuffer))
    {
        std::cout << "OFF BEAT: " << m_timeSinceBeat << std::endl;
        return;
    }
    
    if(m_controlScheme == SplitScreen)
        m_levelManager.UseAbility(tap, angle, false);
    else
        m_levelManager.UseAbility(tap, 0, true);
}

void Game::RotateHero(double direction)
{
    if (m_enableBeat && (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatPause - m_beatBuffer))
    {
        std::cout << "OFF BEAT: " << m_timeSinceBeat << std::endl;
        return;
    }

	m_levelManager.RotateHero(direction);
}

void Game::MoveHero(int row, int column, double direction)
{
	if (m_enableBeat &&  (m_timeSinceBeat > m_beatBuffer && m_timeSinceBeat < m_beatPause - m_beatBuffer))
	{
        std::cout << "OFF BEAT: " << m_timeSinceBeat << std::endl;
		return;
	}
	m_levelManager.MoveToPoint(row, column);
}

void Game::ProcessEvents()
{
	/*sf::Event event;

	while (m_renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_gameState = Exiting;
		else if (event.type == sf::Event::MouseButtonPressed)
			MousePressEvent(event);
		else if (event.type == sf::Event::MouseMoved)
			MouseMoveEvent(event);
		else if (event.type == sf::Event::MouseButtonReleased)
			MouseReleaseEvent(event);
		else if (event.type == sf::Event::KeyPressed)
			KeyPressEvent(event);
	}*/
}

void Game::Draw()
{
	if (m_gameState == GameState::Paused)
		return;

	/*m_renderWindow.clear();
	
	m_levelManager.Draw(m_renderWindow);

	m_renderWindow.display();*/
}

//void Game::Swipe(double direction)
//{
//    MoveHero(0, 0, direction);
//}

//void Game::SwipeCurve(bool isClockwise)
//{
//    if(isClockwise)
//        RotateHero(M_PI_2);
//    else
//        RotateHero(-M_PI_2);
//}

//void Game::Tap()
//{
//    AbilityUsed(0, 0, true, 0);
//}

//void Game::DoubleTap()
//{
//    AbilityUsed(0, 0, false, 0);
//}

void Game::MousePressEvent(const glm::vec2& pos)
{
    m_trackMouse = true;
    m_firstClick = pos;
    MoveHero((int)pos.x, (int)pos.y, 0);
}

void Game::MouseMoveEvent(const glm::vec2& pos)
{
    if(m_trackMouse)
        m_mousePositions.push_back(pos - m_firstClick);
}

void Game::MouseReleaseEvent(const glm::vec2& pos, bool doubleTap)
{
    if(m_controlScheme == Regular)
        HandleRegularSchemeInput(pos, doubleTap);
    else
        HandleSplitScreenInput(pos, doubleTap);
    
    m_mousePositions.clear();
    m_trackMouse = false;
}

void Game::HandleRegularSchemeInput(const glm::vec2& pos, bool doubleTap)
{
    //if (!doubleTap)
    //{
       // MoveHero((int)pos.x, (int)pos.y, 0);
        /*if (m_mousePositions.size() <= 3)
            AbilityUsed((int)pos.y, (int)pos.x, true, 0);
        else
        {
            glm::vec2 finalVec(pos - m_firstClick);
            double averageDistance = 0;
            
            for (unsigned int i = 0; i < m_mousePositions.size(); ++i)
                averageDistance += PointDistanceToVec(m_mousePositions[i], finalVec);
            
            averageDistance /= m_mousePositions.size();
            if (averageDistance <= 20)
            {
                double angle = std::atan2(finalVec.y, finalVec.x);
                
                if (angle < 0)
                    angle += 2 * M_PI;
                
         
            }*/
            /*
            else
            {
                double direction = -M_PI_2;
                glm::vec2 first(m_mousePositions[1] - m_firstClick);
                glm::vec2 last(m_mousePositions[m_mousePositions.size() - 2] - m_firstClick);
                first = glm::normalize(first);
                last = glm::normalize(last);
                
                double cross = first.x * last.y - first.y * last.x;
                if (cross < 0)
                    direction = M_PI_2;
                
                RotateHero(direction);
            }
        }*/
   // }
   // else if (m_mousePositions.size() <= 3)
      //  AbilityUsed((int)pos.y, (int)pos.x, false, 0);
    

}

void Game::HandleSplitScreenInput(const glm::vec2& pos, bool doubleTap)
{
    if(m_firstClick.x < m_windowWidth/2)
        HandleLeftScreenInput(pos, doubleTap);
    else
        HandleRightScreenInput(pos, doubleTap);
}

void Game::HandleLeftScreenInput(const glm::vec2& pos, bool doubleTap)
{
    glm::vec2 finalVec(pos - m_firstClick);
    double angle = std::atan2(finalVec.y, finalVec.x);
    
    AbilityUsed((int)pos.y, (int)pos.x, doubleTap, angle);
}

void Game::HandleRightScreenInput(const glm::vec2& pos, bool doubleTap)
{
    if (m_mousePositions.size() <= 3)
        return;
    
    
    glm::vec2 finalVec(pos - m_firstClick);
    double averageDistance = 0;
    
    for (unsigned int i = 0; i < m_mousePositions.size(); ++i)
        averageDistance += PointDistanceToVec(m_mousePositions[i], finalVec);
    averageDistance /= m_mousePositions.size();
    if (averageDistance > 20)
    {
        double direction = -M_PI_2;
        glm::vec2 first(m_mousePositions[1] - m_firstClick);
        glm::vec2 last(m_mousePositions[m_mousePositions.size() - 2] - m_firstClick);
        first = glm::normalize(first);
        last = glm::normalize(last);
        
        double cross = first.x * last.y - first.y * last.x;
        if (cross < 0)
            direction = M_PI_2;
        
        RotateHero(direction);
        
    }
    else if(averageDistance > 0)
    {
        double angle = std::atan2(finalVec.y, finalVec.x);
        
        if (angle < 0)
            angle += 2 * M_PI;
        
        MoveHero((int)pos.y, (int)pos.x, angle);
    }
    else
    {
        
    }
    
}

glm::vec2 Game::MapCoordToGrid(double x, double y)
{
	glm::vec2 result;

	result.x = (float)std::floor(x / (m_windowWidth / m_columns));
	result.y = (float)std::floor(y / (m_windowHeight / m_rows));

	return result;
}

double Game::PointDistanceToVec(const glm::vec2& point, const glm::vec2& line)
{
	double result;

	result = std::abs((point.x * line.y - point.y * line.x) / glm::length(line));

	return result;
}

void Game::Restart()
{
    Destroy();
	Start(m_scene);
}