
#include <GameLibrary/Actor/Animation.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	Animation::Frame::Frame(const String& file, TextureImage* img)
		: file(file),
		rows(1),
		cols(1),
		x(0),
		y(0),
		img(img)
	{
		//
	}

	Animation::Frame::Frame(const String& file, unsigned int rows, unsigned int cols, unsigned int row, unsigned int col, TextureImage* img)
		: file(file),
		rows(rows),
		cols(cols),
		x(col),
		y(row),
		img(img)
	{
		//
	}

	bool Animation::Frame::operator==(const fgl::Animation::Frame& frame) const
	{
		if(file==frame.file && rows==frame.rows && cols==frame.cols && x==frame.x && y==frame.y && img==frame.img)
		{
			return true;
		}
		return false;
	}

	bool Animation::Frame::operator!=(const fgl::Animation::Frame& frame) const
	{
		return !operator==(frame);
	}

	RectangleU Animation::Frame::getSourceRect() const
	{
		if(img == nullptr)
		{
			return RectangleU(0, 0, 0, 0);
		}
		else
		{
			unsigned int width = ((unsigned int)img->getWidth()/cols);
			unsigned int height = ((unsigned int)img->getHeight()/rows);
			return RectangleU((x*width), (y*height), width, height);
		}
	}

	Animation::Animation() : Animation(1.0f)
	{
		//
	}

	Animation::Animation(float fps_arg)
	{
		if(fps_arg == 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be 0");
		}
		else if(fps_arg < 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be negative");
		}
		fps = fps_arg;
		mirroredHorizontal = false;
		mirroredVertical = false;
	}

	Animation::Animation(float fps, AssetManager* assetManager, const String& file) : Animation(fps)
	{
		addFrame(assetManager, file);
	}

	Animation::Animation(float fps, unsigned int rows, unsigned int cols, AssetManager* assetManager, const String& file) : Animation(fps)
	{
		addFrames(assetManager, file, rows, cols);
	}

	Animation::Animation(float fps, unsigned int rows, unsigned int cols, AssetManager* assetManager, const String& file, const ArrayList<Vector2u>& sequence) : Animation(fps)
	{
		addFrames(assetManager, file, rows, cols, sequence);
	}

	Animation::~Animation()
	{
		//
	}

	bool Animation::operator==(const fgl::Animation& animation) const
	{
		if(frames.size() != animation.frames.size())
		{
			return false;
		}
		for(size_t i=0; i<frames.size(); i++)
		{
			if(frames[i]!=animation.frames[i])
			{
				return false;
			}
		}
		if(fps!=animation.fps || mirroredHorizontal!=animation.mirroredHorizontal || mirroredHorizontal!=animation.mirroredVertical)
		{
			return false;
		}
		return true;
	}

	bool Animation::operator!=(const fgl::Animation& animation) const
	{
		return !operator==(animation);
	}

	void Animation::reloadFrames(AssetManager* assetManager)
	{
		for(size_t i=0; i<frames.size(); i++)
		{
			Frame& frame = frames.get(i);
			if(assetManager == nullptr)
			{
				frame.img = nullptr;
			}
			else
			{
				frame.img = assetManager->loadTexture(frame.file);
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

	Vector2u Animation::getFrameSize(size_t frameNum) const
	{
		const Frame& animFrame = frames.get(frameNum);
		TextureImage* img = animFrame.img;
		if(img == nullptr)
		{
			return Vector2u(0, 0);
		}
		else
		{
			unsigned int imgwidth = (unsigned int)img->getWidth();
			unsigned int imgheight = (unsigned int)img->getHeight();
			return Vector2u(imgwidth/animFrame.cols, imgheight/animFrame.rows);
		}
	}

	bool Animation::addFrame(AssetManager* assetManager, const String& file, String* error)
	{
		if(assetManager==nullptr)
		{
			throw IllegalArgumentException("assetManager", "cannot be null");
		}
		else if(file.length()==0)
		{
			throw IllegalArgumentException("file", "cannot be an empty string");
		}

		auto img = assetManager->loadTexture(file);
		if(img == nullptr)
		{
			return false;
		}
		frames.add(Frame(file, img));
		return true;
	}

	bool Animation::addFrames(AssetManager* assetManager, const String& file, unsigned int rows, unsigned int cols, String* error)
	{
		if(assetManager==nullptr)
		{
			throw IllegalArgumentException("assetManager", "cannot be null");
		}
		else if(file.length()==0)
		{
			throw IllegalArgumentException("file", "cannot be an empty string");
		}
		else if(rows==0)
		{
			throw IllegalArgumentException("rows", "must be greater than 0");
		}
		else if(cols==0)
		{
			throw IllegalArgumentException("cols", "must be greater than 0");
		}

		auto img = assetManager->loadTexture(file, error);
		if(img == nullptr)
		{
			return false;
		}

		for(unsigned int y=0; y<rows; y++)
		{
			for(unsigned int x=0; x<cols; x++)
			{
				frames.add(Frame(file, rows, cols, y, x, img));
			}
		}
		return true;
	}

	bool Animation::addFrames(AssetManager* assetManager, const String& file, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence, String* error)
	{
		if(assetManager==nullptr)
		{
			throw IllegalArgumentException("assetManager", "cannot be null");
		}
		else if(file.length()==0)
		{
			throw IllegalArgumentException("file", "cannot be an empty string");
		}
		else if(rows==0)
		{
			throw IllegalArgumentException("rows", "must be greater than 0");
		}
		else if(cols==0)
		{
			throw IllegalArgumentException("cols", "must be greater than 0");
		}
		else if(sequence.size()==0)
		{
			return true;
		}

		for(size_t i=0; i<sequence.size(); i++)
		{
			auto& point = sequence[i];
			if(point.x >= cols || point.y >= rows)
			{
				//TODO replace with more specific exception type
				throw IllegalArgumentException("sequence", (String)"frame at index "+i+" is outside the bounds of the given rows/cols");
			}
		}

		auto img = assetManager->loadTexture(file, error);
		if(img == nullptr)
		{
			return false;
		}

		for(auto& point : sequence)
		{
			frames.add(Frame(file, rows, cols, point.y, point.x, img));
		}
		return true;
	}

	void Animation::addFrames(const ArrayList<Frame>& frames_arg)
	{
		frames.addAll(frames_arg);
	}

	void Animation::removeAllFrames()
	{
		frames.clear();
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
	
	void Animation::setFPS(float fps_arg)
	{
		if(fps_arg == 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be 0");
		}
		else if(fps_arg < 0.0f)
		{
			throw IllegalArgumentException("fps", "cannot be negative");
		}
		fps = fps_arg;
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
		const Frame& animFrame = frames.get(frameNum);
		TextureImage* img = animFrame.img;
		if(img == nullptr)
		{
			return RectangleD(0,0,0,0);
		}
		else
		{
			unsigned int imgwidth = (unsigned int)img->getWidth();
			unsigned int imgheight = (unsigned int)img->getHeight();
			double width = ((double)imgwidth/(double)animFrame.cols);
			double height = ((double)imgheight/(double)animFrame.rows);
			double left = -(width/2);
			double top = -(height/2);
			return RectangleD(left, top, width, height);
		}
	}

	const ArrayList<Animation::Frame>& Animation::getFrames() const
	{
		return frames;
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
		const Frame& animFrame = frames.get(frameNum);
		
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
