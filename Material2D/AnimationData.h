#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

//Based on GCC4 implementation
inline void SplitString(const std::string& str, std::vector<std::string>& targetVec, char delimeter)
{
	targetVec.clear();
	size_t strLength = str.size();

	if (strLength == 0)
		return;

	size_t startIndex = 0;
	size_t delimIndex = str.find_first_of(delimeter, startIndex);
	while (delimIndex != std::string::npos)
	{
		targetVec.push_back(str.substr(startIndex, delimIndex - startIndex));
		startIndex = delimIndex + 1;
		if (startIndex > strLength)
			break;
		delimIndex = str.find_first_of(delimeter, startIndex);
	}
	if (startIndex < strLength)
		targetVec.push_back(str.substr(startIndex));
}

//Second arguement is the relativePath, first is the path to the file containing relative path
inline std::string ConvertRelativePathToStatic(const std::string& filePath, const std::string& relativePath)
{
	std::string result;

	std::vector<std::string> relativeSplit;
	SplitString(relativePath, relativeSplit, '/');
	std::vector<std::string> fileSplit;
	SplitString(filePath, fileSplit, '/');
	
	if (relativeSplit.size() > fileSplit.size())
		std::cout << " ERROR: AnimationData.h, ConvertRelativePathToStatic, relative paht longer than filepath" << std::endl;

	for (unsigned int i = 0; i < fileSplit.size() - relativeSplit.size(); ++i)
		result += fileSplit[i] + '/';

	result += relativeSplit.back();
	return result;
}


//Taken from RPGEngine
namespace Animation
{
    struct IntRect
    {
        int top;
        int left;
        int width;
        int height;
    };
    
	//Contains the bounds for all the frames of the animation that shares its name with dirName
	//NOTE: SHOULD SPRITE JUST BE REPlACED WITH THIS??
	struct SpriteDefinition
	{
		std::string dirName; //Name of parent directory
		std::map<std::string, IntRect> spriteBounds;//Keeps bounds by name of the sprite
	};

	//Actual sprites in the animation, 
	//NOTE: NO REAL POINT TO THIS FOR NOW, CANT HAVE MORE THAN ONE SPRITE ANYWAY
	struct Sprite
	{
		std::string spriteDir; //Name of the directiory contianing the bounds for this sprite
		std::string spriteName; //Name of sprite
		int x; //X coordinate of sprite in the frame
		int y; //Y coordinate of sprite in the frame
		int z; //Z coordinate of sprite in the frame
	};

	//Contains delay fro the frame as well as all the sprites in the frame. 
	//NOTE: ONLY ONE SPRITE PER FRAME UNTIL I CAN FIGURE OUT HOW TO ADD MORE
	struct Frame
	{
		int maxDelay; //How much to wait before moving to next frame
		int currDelay = 0; //How many have been waited
		std::vector<Sprite> sprites; //Sprites in frame. NOTE: NO POINT ONLY ONE SPRITE PER FRAME FOR NOW
	};

	//Contians name, number of loops, index of current frame and the frames of the animaiton
	struct Animation
	{
		int loops; //Number of times animation should loop
		std::string name; //Name of animation

		int GetCurrentFrameIndex()
		{
			return currentFrame;
		}

		void NextFrame() //Increments frame, safer to use this than increment currentFrame manually
		{
            if(currentFrame >= frames.size())
                return;
			//Increment frame delay counter
			frames[currentFrame].currDelay++;
			//If we still need to wait, return
			if (frames[currentFrame].currDelay < frames[currentFrame].maxDelay)
				return;
			//Otherwise, reset the delay counter and move ahead
			frames[currentFrame].currDelay = 0;
			currentFrame++;
			if (currentFrame >= frames.size())
				currentFrame = 0;
		}
		std::vector<Frame> frames; //Contianer for the frames in animation

	private:
		//Current frame of animation
		unsigned int currentFrame = 0;
	};
}

#endif