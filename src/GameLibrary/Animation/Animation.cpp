
#include <GameLibrary/Animation/Animation.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	Animation::Frame::Frame(TextureImage* image)
		: image(image),
		rows(1),
		cols(1),
		x(0),
		y(0)
	{
		//
	}

	Animation::Frame::Frame(TextureImage* image, unsigned int rows, unsigned int cols, unsigned int row, unsigned int col)
		: image(image),
		rows(rows),
		cols(cols),
		x(col),
		y(row)
	{
		//
	}

	TextureImage* Animation::Frame::getImage() const
	{
		return image;
	}

	RectangleU Animation::Frame::getSourceRect() const
	{
		if(image == nullptr)
		{
			return RectangleU(0, 0, 0, 0);
		}
		else
		{
			unsigned int width = ((unsigned int)image->getWidth()/cols);
			unsigned int height = ((unsigned int)image->getHeight()/rows);
			return RectangleU((x*width), (y*height), width, height);
		}
	}

	Animation::Animation()
		: Animation(1.0f)
	{
		//
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
	
	Animation::Animation(float fps, const ArrayList<Animation::Frame>& frames_arg)
		: Animation(fps)
	{
		frames = frames_arg;
	}

	Animation::Animation(float fps, TextureImage* image)
		: Animation(fps)
	{
		addFrame(image);
	}

	Animation::Animation(float fps, TextureImage* image, unsigned int rows, unsigned int cols)
		: Animation(fps)
	{
		addFrames(image, rows, cols);
	}

	Animation::Animation(float fps, TextureImage* image, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence)
		: Animation(fps)
	{
		addFrames(image, rows, cols, sequence);
	}

	Animation::~Animation()
	{
		//
	}

	void Animation::addFrame(TextureImage* image)
	{
		if(image==nullptr) {
			throw IllegalArgumentException("image", "cannot be null");
		}
		frames.add(Frame(image));
	}
	
	void Animation::addFrame(const Animation::Frame& frame)
	{
		frames.add(frame);
	}

	void Animation::addFrames(TextureImage* image, unsigned int rows, unsigned int cols)
	{
		if(image==nullptr) {
			throw IllegalArgumentException("image", "cannot be null");
		}
		else if(rows==0) {
			throw IllegalArgumentException("rows", "must be greater than 0");
		}
		else if(cols==0) {
			throw IllegalArgumentException("cols", "must be greater than 0");
		}

		frames.reserve(frames.size() + (size_t)(rows*cols));
		for(unsigned int y=0; y<rows; y++) {
			for(unsigned int x=0; x<cols; x++) {
				size_t index = frames.size();
				frames.add(Frame(image, rows, cols, y, x));
			}
		}
	}

	void Animation::addFrames(TextureImage* image, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence)
	{
		if(image==nullptr) {
			throw IllegalArgumentException("image", "cannot be null");
		}
		else if(rows==0) {
			throw IllegalArgumentException("rows", "must be greater than 0");
		}
		else if(cols==0) {
			throw IllegalArgumentException("cols", "must be greater than 0");
		}
		else if(sequence.size()==0) {
			return;
		}

		// validate sequence
		for(size_t i=0; i<sequence.size(); i++) {
			auto& point = sequence[i];
			if(point.x >= cols || point.y >= rows) {
				//TODO replace with more specific exception type
				throw IllegalArgumentException("sequence", (String)"frame at index "+i+" is outside the bounds of the given rows/cols");
			}
		}

		// add frames
		frames.reserve(frames.size() + sequence.size());
		for(auto& point : sequence) {
			frames.add(Frame(image, rows, cols, point.y, point.x));
		}
	}

	void Animation::addFrames(const ArrayList<Animation::Frame>& frames_arg)
	{
		frames.reserve(frames.size() + frames_arg.size());
		for(auto frame : frames_arg) {
			frames.add(frame);
		}
	}
	
	const Animation::Frame& Animation::getFrame(size_t index) const
	{
		return frames.get(index);
	}
	
	void Animation::removeFrame(size_t index)
	{
		frames.remove(index);
	}

	void Animation::removeAllFrames()
	{
		frames.clear();
	}
	
	size_t Animation::getFrameCount() const
	{
		return frames.size();
	}
	
	const ArrayList<Animation::Frame>& Animation::getFrames() const
	{
		return frames;
	}
	
	void Animation::setFPS(float fps_arg)
	{
		if(fps_arg < 0.0f) {
			throw IllegalArgumentException("fps", "cannot be negative");
		}
		fps = fps_arg;
	}
	
	float Animation::getFPS() const
	{
		return fps;
	}
	
	Animation* Animation::setMirroredHorizontal(bool mirror)
	{
		mirroredHorizontal = mirror;
		return this;
	}

	Animation* Animation::setMirroredVertical(bool mirror)
	{
		mirroredVertical = mirror;
		return this;
	}

	bool Animation::isMirroredHorizontal() const
	{
		return mirroredHorizontal;
	}

	bool Animation::isMirroredVertical() const
	{
		return mirroredVertical;
	}
	
	RectangleD Animation::getRect(size_t frameIndex) const
	{
		const Frame& animFrame = frames.get(frameIndex);
		auto img = animFrame.getImage();
		if(img == nullptr)
		{
			return RectangleD(0,0,0,0);
		}
		else
		{
			auto sourceRect = animFrame.getSourceRect();
			auto width = (double)sourceRect.width;
			auto height = (double)sourceRect.height;
			auto left = -(width/2);
			auto top = -(height/2);
			return RectangleD(left, top, width, height);
		}
	}

	void Animation::drawFrame(Graphics& graphics, size_t frameIndex) const
	{
		if(frameIndex >= frames.size())
		{
			return;
		}
		drawFrame(graphics, frameIndex, getRect(frameIndex));
	}
	
	void Animation::drawFrame(Graphics& graphics, size_t frameIndex, const RectangleD& dstRect) const
	{
		if(frameIndex >= frames.size())
		{
			return;
		}
		auto& animFrame = frames.get(frameIndex);
		auto srcRect = animFrame.getSourceRect();
		
		double dst_left = dstRect.x;
		double dst_top = dstRect.y;
		double dst_right = dst_left + dstRect.width;
		double dst_bottom = dst_top + dstRect.height;

		unsigned int src_left = (unsigned int)srcRect.x;
		unsigned int src_top = (unsigned int)srcRect.y;
		unsigned int src_right = (unsigned int)(src_left + srcRect.width);
		unsigned int src_bottom = (unsigned int)(src_top + srcRect.height);

		auto image = animFrame.getImage();
		if(mirroredHorizontal)
		{
			if(mirroredVertical)
			{
				graphics.drawImage(image, dst_right, dst_bottom, dst_left, dst_top, src_left, src_top, src_right, src_bottom);
			}
			else
			{
				graphics.drawImage(image, dst_right, dst_top, dst_left, dst_bottom, src_left, src_top, src_right, src_bottom);
			}
		}
		else
		{
			if(mirroredVertical)
			{
				graphics.drawImage(image, dst_left, dst_bottom, dst_right, dst_top, src_left, src_top, src_right, src_bottom);
			}
			else
			{
				graphics.drawImage(image, dst_left, dst_top, dst_right, dst_bottom, src_left, src_top, src_right, src_bottom);
			}
		}
	}
}
