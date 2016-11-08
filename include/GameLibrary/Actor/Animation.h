
#pragma once

#include <GameLibrary/Application/ApplicationData.h>
#include <GameLibrary/Application/AssetManager.h>
#include <GameLibrary/Screen/Drawable.h>
#include <GameLibrary/Utilities/ArrayList.h>
#include <GameLibrary/Utilities/String.h>
#include <GameLibrary/Utilities/Geometry/Rectangle.h>

namespace fgl
{
	/*! Holds Animation data for the SpriteActor class*/
	class Animation
	{
	public:
		/*! The direction the Animation should move in when animating.
			\see fgl::SpriteActor::changeAnimation(const fgl::String&,const fgl::Animation::Direction&)*/
		typedef enum
		{
			/*! Animation direction stays the same as its previous value, which may be Direction::FORWARD, Direction::BACKWARD, or Direction::STOPPED*/
			NO_CHANGE,
			/*! frames iterate forward as time progresses*/
			FORWARD,
			/*! frames iterate backward as time progresses*/
			BACKWARD,
			/*! frames do not iterate as time progresses*/
			STOPPED
		} Direction;
		
		/*! copy constructor*/
		Animation(const Animation&);
		/*! Constructs an Animation with a specified frame rate.
			\param fps the frame rate in frames per second
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		explicit Animation(float fps);
		/*! Constructs an Animation with a frame rate and a first frame.
			\param fps the frame rate in frames per second
			\param assetManager the assetManager to load and get the TextureImage. If null, or fails to load the image, the first frame's image is set to null, until Animation::reloadFrames is called to attempt to reload the images.
			\param file the path to the image file of the first frame
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		Animation(float fps, AssetManager* assetManager, const String& file);
		/*! Constructs an Animation with a frame rate and frames
			\param fps the frame rate in frames per second
			\param rows divides the image into rows of frames. If 0, no frames are added.
			\param cols divides the image into columns of frames. If 0, no frames are added.
			\param assetManager the assetManager to load and get the TextureImage. If null, or fails to load the image, the first frame's image is set to null, until Animation::reloadFrames is called to attempt to reload the images.
			\param file the path to the image file to use for the frames
			\note through this function, frames are added by looping through each row and adding the columns in each row. If you want a specific order, use the sequence parameter.
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		Animation(float fps, unsigned int rows, unsigned int cols, AssetManager* assetManager, const String& file);
		/*! Constructs an Animation with a frame rate and frames
			\param fps the frame rate in frames per second
			\param rows divides the image into rows of frames. If 0, no frames are added.
			\param cols divides the image into columns of frames. If 0, no frames are added.
			\param assetManager the assetManager to load and get the TextureImage. If null, or fails to load the image, the first frame's image is set to null, until Animation::reloadFrames is called to attempt to reload the images.
			\param file the path to the image file to use for the frames
			\param sequence the specific sequence to order the frames. the x value of the Vector2d represents the column, and the y value represents the row; rows and columns start from 0
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		Animation(float fps, unsigned int rows, unsigned int cols, AssetManager*assetManager, const String& file, const ArrayList<Vector2u>& sequence);
		/*! virtual destructor
			\note this does not unload the images loaded during construction. You must manually unload the contents of the assetManager used.*/
		~Animation();
		/*! assignment operator*/
		Animation& operator=(const Animation&);
		
		
		/*! Called by fgl::SpriteActor::draw if Animation is the current Animation. Draws the animation to the screen using the specified Graphics object
			\param graphics the graphics object used to draw the Animation
			\param frameNum the frame number to draw */
		void drawFrame(Graphics graphics, size_t frameNum) const;
		
		
		/*! Draws the specified frame
			\param graphics the graphics object used to draw the Animation
			\param frameNum the frame number to draw
			\param dstRect the rectangle that the animation will be drawn within */
		void drawFrame(Graphics& graphics, size_t frameNum, const RectangleD& dstRect) const;
		
		
		/*! Loads and re-stores the images for each frame of the Animation.
			\param assetManager the AssetManager to load the images from. If null, the images for each frame are set to null.*/
		void reloadFrames(AssetManager*assetManager);
		
		
		/*! Removes all frames stored for the Animation.*/
		void clear();
		
		
		/*! Sets the Animation to draw horizontally inverted.
			\param mirror true to make the Animation draw horizontally mirrored, and false to make the Animation draw normal horizontally*/
		void setMirroredHorizontal(bool mirror);
		/*! Sets the Animation to draw vertically inverted.
			\param mirror true to make the Animation draw vertically mirrored, and false to make the Animation draw normal vertically*/
		void setMirroredVertical(bool mirror);
		/*! Tells whether the Animation is mirrored horizontally.
			\see fgl::Animation::setMirroredHorizontal(bool)
			\returns true if the Animation is mirrored horizontally, false if otherwise*/
		bool isMirroredHorizontal() const;
		/*! Tells whether the Animation is mirrored vertically.
			\see fgl::Animation::setMirroredVertical(bool)
			\returns true if the Animation is mirrored vertically, false if otherwise*/
		bool isMirroredVertical() const;
		
		
		/*! Gets the width of the current frame. No relation to Animation::getFrame().
			\returns the width of the current frame*/
		unsigned int getFrameWidth() const;
		/*! Gets the width of the specified frame. No relation to Animation::getFrame().
			\param frameNum the index of the frame
			\returns the width of the specified frame*/
		unsigned int getFrameWidth(size_t frameNum) const;
		/*! Gets the height of the current frame. No relation to Animation::getFrame().
			\returns the height of the current frame*/
		unsigned int getFrameHeight() const;
		/*! Gets the height of the specified frame. No relation to Animation::getFrame().
			\param frameNum the index of the frame
			\returns the height of the specified frame*/
		unsigned int getFrameHeight(size_t frameNum) const;
		
		
		/*! Adds a single frame.
			\param assetManager the AssetManager to load the TextureImage for the frame
			\param file the path to the image file to use*/
		void addFrame(AssetManager*assetManager, const String&file);
		/*! Adds frames.
			\param assetManager the AssetManager to load the TextureImage for the frames
			\param file the path to the image file to use
			\param rows the number of rows to divide the image into
			\param cols the number of columns to divide the image into
			\note through this function, frames are added by looping through each row and adding the columns in each row. If you want a specific order, use the sequence parameter.*/
		void addFrames(AssetManager*assetManager, const String&file, unsigned int rows, unsigned int cols);
		/*! Adds frames.
			\param assetManager the AssetManager to load the TextureImage for the frames
			\param file the path to the image file to use
			\param rows the number of rows to divide the image into
			\param cols the number of columns to divide the image into
			\param sequence the specific sequence to order the frames. the x value of the Vector2d represents the column, and the y value represents the row*/
		void addFrames(AssetManager*assetManager, const String&file, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>&sequence);
		
		
		/*! Gets the total amount of frames stored in the Animation.
			\returns the total amount of frames*/
		size_t getTotalFrames() const;
		
		
		/*! Sets the frame rate.
			\param fps the frame rate in frames per second
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		void setFPS(float fps);
		/*! Gets the frame rate.
			\returns an unsigned int representing the frame rate in frames per second*/
		float getFPS() const;
		
		
		/*! Gets the image being used in the specified frame.
			\param frameNum the frame index
			\returns the TextureImage for the specified frame*/
		TextureImage* getImage(size_t frameNum) const;
		/*! Gets the source Rectangle for the TextureImage at the specified frame.
			\param frameNum the frame index
			\returns the source Rectangle for the specified frame*/
		RectangleU getImageSourceRect(size_t frameNum) const;

		/*! Gets the frame (bounding box) of the Animation at the specified frame (animation frame).
			\param frameNum the frame index
			\returns the bounding box of the Animation*/
		RectangleD getRect(size_t frameNum) const;
		
	private:
		class AnimationFrame
		{
		public:
			String file;
			unsigned int rows;
			unsigned int cols;
			unsigned int x;
			unsigned int y;
			TextureImage* img;

			AnimationFrame();
			AnimationFrame(const AnimationFrame&);
			AnimationFrame(const String&file, TextureImage*img=nullptr);
			AnimationFrame(const String&file, unsigned int rows, unsigned int cols, unsigned int row, unsigned int col, TextureImage*img=nullptr);
			
			RectangleU getSourceRect() const;
		};

		ArrayList<AnimationFrame> frames;

		float fps;

		bool mirroredHorizontal;
		bool mirroredVertical;
	};
}
