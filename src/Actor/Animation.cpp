
#include <GameLibrary/Actor/Animation.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	Animation::AnimationFrame::AnimationFrame()
	{
		file = "";
		rows = 1;
		cols = 1;
		x = 0;
		y = 0;
		img = nullptr;
	}

	Animation::AnimationFrame::AnimationFrame(const Animation::AnimationFrame&animationFrame)
	{
		file = animationFrame.file;
		rows = animationFrame.rows;
		cols = animationFrame.cols;
		x = animationFrame.x;
		y = animationFrame.y;
		img = animationFrame.img;
	}

	Animation::AnimationFrame::AnimationFrame(const String&filepath, TextureImage*tximg)
	{
		file = filepath;
		rows = 1;
		cols = 1;
		x = 0;
		y = 0;
		img = tximg;
	}

	Animation::AnimationFrame::AnimationFrame(const String&filepath, unsigned int rnum, unsigned int cnum, unsigned int r, unsigned int c, TextureImage*tximg)
	{
		file = filepath;
		rows = rnum;
		cols = cnum;
		x = c;
		y = r;
		img = tximg;
	}

	RectangleU Animation::AnimationFrame::getSourceRect() const
	{
		if(img == nullptr)
		{
			return RectangleU(0,0,0,0);
		}
		else
		{
			unsigned int width = (img->getWidth()/cols);
			unsigned int height = (img->getHeight()/rows);
			return RectangleU((x*width), (y*height), width, height);
		}
	}

	Animation::Animation(const Animation& animation)
	{
		frames = animation.frames;
		fps = animation.fps;
		mirroredHorizontal = animation.mirroredHorizontal;
		mirroredVertical = animation.mirroredVertical;
	}
	
	Animation& Animation::operator=(const Animation& animation)
	{
		frames = animation.frames;
		fps = animation.fps;
		mirroredHorizontal = animation.mirroredHorizontal;
		mirroredVertical = animation.mirroredVertical;

		return *this;
	}

	Animation::Animation(float fps_arg)
	{
		if(fps_arg < 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be negative");
		}
		fps = fps_arg;
		mirroredHorizontal = false;
		mirroredVertical = false;
	}

	Animation::Animation(float fps, AssetManager*assetManager, const String&file) : Animation(fps)
	{
		addFrame(assetManager, file);
	}

	Animation::Animation(float fps, unsigned int rows, unsigned int cols, AssetManager*assetManager, const String&file) : Animation(fps)
	{
		addFrames(assetManager, file, rows, cols);
	}

	Animation::Animation(float fps, unsigned int rows, unsigned int cols, AssetManager*assetManager, const String&file, const ArrayList<Vector2u>& sequence) : Animation(fps)
	{
		addFrames(assetManager, file, rows, cols, sequence);
	}

	Animation::~Animation()
	{
		//
	}
	
	void Animation::reloadFrames(AssetManager*assetManager)
	{
		for(size_t i=0; i<frames.size(); i++)
		{
			AnimationFrame& frame = frames.get(i);
			if(assetManager == nullptr)
			{
				frame.img = nullptr;
			}
			else
			{
				assetManager->loadTexture(frame.file);
				frame.img = assetManager->getTexture(frame.file);
			}
		}
	}

	void Animation::clear()
	{
		frames.clear();
	}

	void Animation::setMirroredHorizontal(bool toggle)
	{
		mirroredHorizontal = toggle;
	}

	void Animation::setMirroredVertical(bool toggle)
	{
		mirroredVertical = toggle;
	}

	bool Animation::isMirroredHorizontal() const
	{
		return mirroredHorizontal;
	}

	bool Animation::isMirroredVertical() const
	{
		return mirroredVertical;
	}

	unsigned int Animation::getFrameWidth(size_t frameNum) const
	{
		const AnimationFrame& animFrame = frames.get(frameNum);
		TextureImage* img = animFrame.img;
		if(img == nullptr)
		{
			return 0;
		}
		else
		{
			unsigned int imgwidth = img->getWidth();
			return (imgwidth/animFrame.cols);
		}
	}

	unsigned int Animation::getFrameHeight(size_t frameNum) const
	{
		const AnimationFrame& animFrame = frames.get(frameNum);
		TextureImage* img = animFrame.img;
		if(img == nullptr)
		{
			return 0;
		}
		else
		{
			unsigned int imgheight = img->getHeight();
			return (imgheight/animFrame.rows);
		}
	}

	void Animation::addFrame(AssetManager* assetManager, const String& file)
	{
		if(file.length()>0)
		{
			TextureImage* img = nullptr;
			if(assetManager != nullptr && assetManager->loadTexture(file))
			{
				img = assetManager->getTexture(file);
			}
			
			frames.add(AnimationFrame(file, img));
		}
	}

	void Animation::addFrames(AssetManager* assetManager, const String& file, unsigned int rows, unsigned int cols)
	{
		unsigned int total = rows*cols;
		if(total>0 && file.length()>0)
		{
			TextureImage* img = nullptr;
			if(assetManager != nullptr)
			{
				if(assetManager->loadTexture(file))
				{
					img = assetManager->getTexture(file);
				}
			}

			for(unsigned int x=0; x<cols; x++)
			{
				for(unsigned int y=0; y<rows; y++)
				{
					frames.add(AnimationFrame(file, rows, cols, y, x, img));
				}
			}
		}
	}

	void Animation::addFrames(AssetManager* assetManager, const String& file, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence)
	{
		unsigned int total = rows*cols;
		if(total > 0 && file.length()>0)
		{
			TextureImage* img = nullptr;
			if(assetManager != nullptr)
			{
				if(assetManager->loadTexture(file))
				{
					img = assetManager->getTexture(file);
				}
			}

			for(size_t i=0; i<sequence.size(); i++)
			{
				const Vector2u& point = sequence.get(i);
				if(point.x >= cols || point.y >= rows)
				{
					//TODO replace with more specific exception type
					throw Exception("Animation frame (x,y) cannot be outside of the specified rows/cols");
				}
				else
				{
					frames.add(AnimationFrame(file, rows, cols, point.y, point.x, img));
				}
			}
		}
	}
	
	size_t Animation::getTotalFrames() const
	{
		return frames.size();
	}

	bool Animation::didLoadAllImages() const
	{
		for(size_t frames_size=frames.size(), i=0; i<frames_size; i++)
		{
			if(frames[i].img==nullptr)
			{
				return false;
			}
		}
		return true;
	}
	
	void Animation::setFPS(float framerate)
	{
		if(framerate < 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be negative");
		}
		fps = framerate;
	}
	
	float Animation::getFPS() const
	{
		return fps;
	}
	
	TextureImage* Animation::getImage(size_t fNum) const
	{
		size_t frameNum = fNum;
		size_t totalFrames = frames.size();
		if(frameNum > totalFrames)
		{
			if(totalFrames>0)
			{
				frameNum = (totalFrames-1);
			}
			else
			{
				frameNum = 0;
			}
		}
		return frames.get(frameNum).img;
	}

	RectangleU Animation::getImageSourceRect(size_t fNum) const
	{
		size_t frameNum = fNum;
		size_t totalFrames = frames.size();
		if(frameNum > totalFrames)
		{
			if(totalFrames>0)
			{
				frameNum = (totalFrames-1);
			}
			else
			{
				frameNum = 0;
			}
		}
		return frames.get(frameNum).getSourceRect();
	}
	
	RectangleD Animation::getRect(size_t frameNum) const
	{
		const AnimationFrame& animFrame = frames.get(frameNum);
		TextureImage* img = animFrame.img;
		if(img == nullptr)
		{
			return RectangleD(0,0,0,0);
		}
		else
		{
			unsigned int imgwidth = img->getWidth();
			unsigned int imgheight = img->getHeight();
			double width = ((double)imgwidth/(double)animFrame.cols);
			double height = ((double)imgheight/(double)animFrame.rows);
			double left = -(width/2);
			double top = -(height/2);
			return RectangleD(left, top, width, height);
		}
	}

	void Animation::drawFrame(Graphics& graphics, size_t frameNum) const
	{
		size_t totalFrames = frames.size();
		if(totalFrames == 0)
		{
			return;
		}
		if(frameNum > totalFrames)
		{
			if(totalFrames>0)
			{
				frameNum = (totalFrames-1);
			}
			else
			{
				frameNum = 0;
			}
		}

		RectangleD dstRect = getRect(frameNum);
		
		drawFrame(graphics, frameNum, dstRect);
	}
	
	void Animation::drawFrame(Graphics& graphics, size_t frameNum, const RectangleD& dstRect) const
	{
		const AnimationFrame& animFrame = frames.get(frameNum);
		
		RectangleU srcRect = animFrame.getSourceRect();
		
		double dst_left = dstRect.x;
		double dst_top = dstRect.y;
		double dst_right = dst_left + dstRect.width;
		double dst_bottom = dst_top + dstRect.height;

		unsigned int src_left = (unsigned int)srcRect.x;
		unsigned int src_top = (unsigned int)srcRect.y;
		unsigned int src_right = (unsigned int)(src_left + srcRect.width);
		unsigned int src_bottom = (unsigned int)(src_top + srcRect.height);

		if(mirroredHorizontal)
		{
			if(mirroredVertical)
			{
				graphics.drawImage(animFrame.img, dst_right, dst_bottom, dst_left, dst_top, src_left, src_top, src_right, src_bottom);
			}
			else
			{
				graphics.drawImage(animFrame.img, dst_right, dst_top, dst_left, dst_bottom, src_left, src_top, src_right, src_bottom);
			}
		}
		else
		{
			if(mirroredVertical)
			{
				graphics.drawImage(animFrame.img, dst_left, dst_bottom, dst_right, dst_top, src_left, src_top, src_right, src_bottom);
			}
			else
			{
				graphics.drawImage(animFrame.img, dst_left, dst_top, dst_right, dst_bottom, src_left, src_top, src_right, src_bottom);
			}
		}
	}
}
